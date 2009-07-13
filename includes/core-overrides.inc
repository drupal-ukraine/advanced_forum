<?php
// $Id$

/**
 * @file
 * These are functions that are overriding core directly (not theme related.)
 */


/**
 * Menu callback; prints a forum listing.
 *
 * This is copied from the forum module and adapted.
 */
function advanced_forum_page($tid = 0) {
  if (!is_numeric($tid)) {
    return MENU_NOT_FOUND;
  }
  $tid = (int)$tid;

  _advanced_forum_add_files();

  $topics = '';
  $forum_per_page = variable_get('forum_per_page', 25);
  $sortby = variable_get('forum_order', 1);

  $forums = advanced_forum_get_forums($tid);
  $parents = taxonomy_get_parents_all($tid);
  if ($tid && !in_array($tid, variable_get('forum_containers', array()))) {
    $topics = advanced_forum_get_topics($tid, $sortby, $forum_per_page);
  }

  return theme('forums', $forums, $topics, $parents, $tid, $sortby, $forum_per_page);
}

/**
 * Returns a list of all forums for a given taxonomy id
 *
 * This is copied from the forum module and adapted.
 *
 * Forum objects contain the following fields
 * -num_topics Number of topics in the forum
 * -num_posts Total number of posts in all topics
 * -last_post Most recent post for the forum
 *
 * @param $tid
 *   Taxonomy ID of the vocabulary that holds the forum list.
 * @return
 *   Array of object containing the forum information.
 */
function advanced_forum_get_forums($tid = 0) {

  $forums = array();
  $vid = variable_get('forum_nav_vocabulary', '');
  $_forums = taxonomy_get_tree($vid, $tid);

  if (count($_forums)) {

    $counts = array();

    $sql = "SELECT r.tid, COUNT(n.nid) AS topic_count, SUM(l.comment_count) AS comment_count
            FROM {node} n
            INNER JOIN {node_comment_statistics} l ON n.nid = l.nid
            INNER JOIN {term_node} r ON n.vid = r.vid
            WHERE n.status = 1
            GROUP BY r.tid";
    $sql = db_rewrite_sql($sql);
    $_counts = db_query($sql);
    while ($count = db_fetch_object($_counts)) {
      $counts[$count->tid] = $count;
    }
  }

  foreach ($_forums as $forum) {
    // Check if this term is a container
    if (in_array($forum->tid, variable_get('forum_containers', array()))) {
      $forum->container = 1;
    }

    if (!empty($counts[$forum->tid])) {
      $forum->num_topics = $counts[$forum->tid]->topic_count;
      $forum->num_posts = $counts[$forum->tid]->topic_count + $counts[$forum->tid]->comment_count;
    }
    else {
      $forum->num_topics = 0;
      $forum->num_posts = 0;
    }

    // This query does not use full ANSI syntax since MySQL 3.x does not support
    // table1 INNER JOIN table2 INNER JOIN table3 ON table2_criteria ON table3_criteria
    // used to join node_comment_statistics to users.
    $sql = "SELECT n.nid, n.title, n.type,
                   ncs.last_comment_timestamp,
                   IF (ncs.last_comment_uid != 0, u2.name, ncs.last_comment_name) AS last_comment_name,
                   ncs.last_comment_uid
            FROM {node} n
            INNER JOIN {users} u1 ON n.uid = u1.uid
            INNER JOIN {term_node} tn ON n.vid = tn.vid
            INNER JOIN {node_comment_statistics} ncs ON n.nid = ncs.nid
            INNER JOIN {users} u2 ON ncs.last_comment_uid=u2.uid
            WHERE n.status = 1 AND tn.tid = %d
            ORDER BY ncs.last_comment_timestamp DESC";
    $sql = db_rewrite_sql($sql);
    $topic = db_fetch_object(db_query_range($sql, $forum->tid, 0, 1));

    $last_post = new stdClass();
    if (!empty($topic->last_comment_timestamp)) {
      $last_post->timestamp = $topic->last_comment_timestamp;
      $last_post->name = $topic->last_comment_name;
      $last_post->uid = $topic->last_comment_uid;
      $last_post->nid = $topic->nid;
      // Note, we call it "node_title" otherwise it gets picked up on the
      // topic list as well.
      $last_post->node_title = $topic->title;
      $last_post->type = $topic->type;
    }
    $forum->last_post = $last_post;

    $forums[$forum->tid] = $forum;
  }

  return $forums;
}

/**
 * This is copied from the forum module and adapted.
 */
function advanced_forum_get_topics($tid, $sortby, $forum_per_page) {
  global $user, $forum_topic_list_header;

  $forum_topic_list_header = array(
    NULL,
    array('data' => t('Topic'), 'field' => 'n.title'),
    array('data' => t('Replies'), 'field' => 'l.comment_count'),
    array('data' => t('Created'), 'field' => 'n.created'),
    array('data' => t('Last reply'), 'field' => 'l.last_comment_timestamp'),
  );

  $order = _forum_get_topic_order($sortby);
  for ($i = 0; $i < count($forum_topic_list_header); $i++) {
    if ($forum_topic_list_header[$i]['field'] == $order['field']) {
      $forum_topic_list_header[$i]['sort'] = $order['sort'];
    }
  }

  $term = taxonomy_get_term($tid);

  $sql = db_rewrite_sql("SELECT n.nid, r.tid, n.title, n.type, n.sticky, u.name, u.uid, n.created AS timestamp, n.comment AS comment_mode, l.last_comment_timestamp, IF(l.last_comment_uid != 0, cu.name, l.last_comment_name) AS last_comment_name, l.last_comment_uid, l.comment_count AS num_comments, f.tid AS forum_tid FROM {node_comment_statistics} l INNER JOIN {node} n ON n.nid = l.nid INNER JOIN {users} cu ON l.last_comment_uid = cu.uid INNER JOIN {term_node} r ON n.vid = r.vid INNER JOIN {users} u ON n.uid = u.uid INNER JOIN {forum} f ON n.vid = f.vid WHERE n.status = 1 AND r.tid = %d");
  $sql .= tablesort_sql($forum_topic_list_header, 'n.sticky DESC,');
  $sql .= ', n.created DESC';  // Always add a secondary sort order so that the news forum topics are on top.

  $sql_count = db_rewrite_sql("SELECT COUNT(n.nid) FROM {node} n INNER JOIN {term_node} r ON n.vid = r.vid AND r.tid = %d WHERE n.status = 1");

  $result = pager_query($sql, $forum_per_page, 0, $sql_count, $tid);
  $topics = array();
  while ($topic = db_fetch_object($result)) {
    if ($user->uid) {
      // folder is new if topic is new or there are new comments since last visit
      if ($topic->tid != $tid) {
        $topic->new = 0;
      }
      else {
        $history = _forum_user_last_visit($topic->nid);
        $topic->new_replies = advanced_forum_reply_num_new($topic->nid, $history);
        $topic->new = $topic->new_replies || ($topic->timestamp > $history);
      }
    }
    else {
      // Do not track "new replies" status for topics if the user is anonymous.
      $topic->new_replies = 0;
      $topic->new = 0;
    }

    if ($topic->num_comments > 0) {
      $last_reply = new stdClass();
      $last_reply->timestamp = $topic->last_comment_timestamp;
      $last_reply->name = $topic->last_comment_name;
      $last_reply->uid = $topic->last_comment_uid;
      $topic->last_reply = $last_reply;
    }
    $topics[] = $topic;
  }

  return $topics;
}

