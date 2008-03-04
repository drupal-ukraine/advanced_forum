<?php

/*****************************************************************************/
/********************************* TEMPLATE PREPROCESS ***********************/
/*****************************************************************************/

/**
 * Process variables for forums.tpl.php
 *
 * The $variables array contains the following arguments:
 * - $forums
 * - $topics
 * - $parents
 * - $tid
 * - $sortby
 * - $forum_per_page
 *
 * @see forums.tpl.php
 */
function template_preprocess_forums(&$variables) {
  global $user;

  $vid = variable_get('forum_nav_vocabulary', '');
  $vocabulary = taxonomy_vocabulary_load($vid);
  $title = !empty($vocabulary->name) ? $vocabulary->name : '';

  // Breadcrumb navigation:
  $breadcrumb[] = l(t('Home'), NULL);
  if ($variables['tid']) {
    $breadcrumb[] = l($vocabulary->name, 'forum');
  }
  if ($variables['parents']) {
    $variables['parents'] = array_reverse($variables['parents']);
    foreach ($variables['parents'] as $p) {
      if ($p->tid == $variables['tid']) {
        $title = $p->name;
      }
      else {
        $breadcrumb[] = l($p->name, 'forum/'. $p->tid);
      }
    }
  }
  drupal_set_breadcrumb($breadcrumb);
  drupal_set_title(check_plain($title));

  if ($variables['forums_defined'] = count($variables['forums']) || count($variables['parents'])) {
    // Format the "post new content" links listing.
    $forum_types = array();

    // Loop through all node types for forum vocabulary.
    foreach ($vocabulary->nodes as $type) {
      // Check if the current user has the 'create' permission for this node type.
      if (node_access('create', $type)) {
        // Fetch the "General" name of the content type;
        // Push the link with title and url to the array.
        $forum_types[$type] = array('title' => t('Post new @node_type', array('@node_type' => node_get_types('name', $type))), 'href' => 'node/add/'. str_replace('_', '-', $type) .'/'. $variables['tid']);
      }
    }

    if (empty($forum_types)) {
      // The user is logged-in; but denied access to create any new forum content type.
      if ($user->uid) {
        $forum_types['disallowed'] = array('title' => t('You are not allowed to post new content in forum.'));
      }
      // The user is not logged-in; and denied access to create any new forum content type.
      else {
        $title = t('<a href="@login">Login to post a new forum topic</a>',array('@login' => url('user/login', drupal_get_destination())));
        $forum_types['login'] = array('title' => $title, 'html' => TRUE);
   
      }
    }
    $variables['links'] = $forum_types;
    if (!empty($variables['forums'])) {
      $variables['forums'] = theme('forum_list', $variables['forums'], $variables['parents'], $variables['tid']);
    }
    else {
      $variables['forums'] = '';
    }

    if ($variables['tid'] && !in_array($variables['tid'], variable_get('forum_containers', array()))) {
      $variables['topics'] = theme('forum_topic_list', $variables['tid'], $variables['topics'], $variables['sortby'], $variables['forum_per_page']);
      drupal_add_feed(url('taxonomy/term/'. $variables['tid'] .'/0/feed'), 'RSS - '. $title);
    }
    else {
      $variables['topics'] = '';
    }

    // Provide separate template suggestions based on what's being output. Topic id is also accounted for.
    // Check both variables to be safe then the inverse. Forums with topic ID's take precedence.
    if ($variables['forums'] && !$variables['topics']) {
      $variables['template_files'][] = 'forums-containers';
      $variables['template_files'][] = 'forums-'. $variables['tid'];
      $variables['template_files'][] = 'forums-containers-'. $variables['tid'];
    }
    elseif (!$variables['forums'] && $variables['topics']) {
      $variables['template_files'][] = 'forums-topics';
      $variables['template_files'][] = 'forums-'. $variables['tid'];
      $variables['template_files'][] = 'forums-topics-'. $variables['tid'];
    }
    else {
      $variables['template_files'][] = 'forums-'. $variables['tid'];
    }

  }
  else {
    drupal_set_title(t('No forums defined'));
    $variables['links'] = array();
    $variables['forums'] = '';
    $variables['topics'] = '';
  }
}

/**
 * Process variables to format a forum listing.
 *
 * $variables contains the following information:
 * - $forums
 * - $parents
 * - $tid
 *
 * @see forum-list.tpl.php
 * @see theme_forum_list()
 */
function template_preprocess_forum_list(&$variables) {
  global $user;
  $row = 0;
  // Sanitize each forum so that the template can safely print the data.
  foreach ($variables['forums'] as $id => $forum) {
    $variables['forums'][$id]->description = !empty($forum->description) ? filter_xss_admin($forum->description) : '';
    $variables['forums'][$id]->link = url("forum/$forum->tid");
    $variables['forums'][$id]->name = check_plain($forum->name);
    $variables['forums'][$id]->is_container = !empty($forum->container);
    $variables['forums'][$id]->zebra = $row % 2 == 0 ? 'odd' : 'even';
    $row++;

    $variables['forums'][$id]->new_text = '';
    $variables['forums'][$id]->new_url = '';
    $variables['forums'][$id]->new_topics = 0;
    $variables['forums'][$id]->old_topics = $forum->num_topics;
    if ($user->uid) {
      $variables['forums'][$id]->new_topics = _forum_topics_unread($forum->tid, $user->uid);
      if ($variables['forums'][$id]->new_topics) {
        $variables['forums'][$id]->new_text = format_plural($variables['forums'][$id]->new_topics, '1 new', '@count new');
        $variables['forums'][$id]->new_url = url("forum/$forum->tid", NULL, 'new');
      }
      $variables['forums'][$id]->old_topics = $forum->num_topics - $variables['forums'][$id]->new_topics;
    }
    $variables['forums'][$id]->last_reply = theme('forum_submitted', $forum->last_post);
  }
  // Give meaning to $tid for themers. $tid actually stands for term id.
  $variables['forum_id'] = $variables['tid'];
  unset($variables['tid']);
}

/**
 * Preprocess variables to format the topic listing.
 *
 * $variables contains the following data:
 * - $tid
 * - $topics
 * - $sortby
 * - $forum_per_page
 *
 * @see forum-topic-list.tpl.php
 * @see theme_forum_topic_list()
 */
function template_preprocess_forum_topic_list(&$variables) {
  global $forum_topic_list_header;

  // Create the tablesorting header.
  $ts = tablesort_init($forum_topic_list_header);
  $header = '';
  foreach ($forum_topic_list_header as $cell) {
    $cell = tablesort_header($cell, $forum_topic_list_header, $ts);
    $header .= _theme_table_cell($cell, TRUE);
  }
  $variables['header'] = $header;

  if (!empty($variables['topics'])) {
    $row = 0;
    foreach ($variables['topics'] as $id => $topic) {
      // D6 Compatability code
      $topic->forum_tid = $topic->tid;
      $variables['topics'][$id]->icon = theme('forum_icon', $topic->new, $topic->num_comments, $topic->comment_mode, $topic->sticky);
      $variables['topics'][$id]->zebra = $row % 2 == 0 ? 'odd' : 'even';
      $row++;

      // We keep the actual tid in forum table, if it's different from the
      // current tid then it means the topic appears in two forums, one of
      // them is a shadow copy.
      if ($topic->forum_tid != $variables['tid']) {
        $variables['topics'][$id]->moved = TRUE;
        $variables['topics'][$id]->title = check_plain($topic->title);
        $variables['topics'][$id]->message = l(t('This topic has been moved'), "forum/$topic->forum_tid");
      }
      else {
        $variables['topics'][$id]->moved = FALSE;
        $variables['topics'][$id]->title = l($topic->title, "node/$topic->nid");
        $variables['topics'][$id]->message = '';
      }
      $variables['topics'][$id]->created = theme('forum_submitted', $topic);
      $variables['topics'][$id]->last_reply = theme('forum_submitted', isset($topic->last_reply) ? $topic->last_reply : NULL);

      $variables['topics'][$id]->new_text = '';
      $variables['topics'][$id]->new_url = '';
      if ($topic->new_replies) {
        $variables['topics'][$id]->new_text = format_plural($topic->new_replies, '1 new', '@count new');
        $variables['topics'][$id]->new_url = url("node/$topic->nid", comment_new_page_count($topic->num_comments, $topic->new_replies, $topic), 'new');
      }

    }
  }
  else {
    // Make this safe for the template
    $variables['topics'] = array();
  }
  // Give meaning to $tid for themers. $tid actually stands for term id.
  $variables['topic_id'] = $variables['tid'];
  unset($variables['tid']);

  $variables['pager'] = theme('pager', NULL, $variables['forum_per_page'], 0);
}

/**
 * Process variables to format the icon for each individual topic.
 *
 * $variables contains the following data:
 * - $new_posts
 * - $num_posts = 0
 * - $comment_mode = 0
 * - $sticky = 0
 *
 * @see forum-icon.tpl.php
 * @see theme_forum_icon()
 */
function template_preprocess_forum_icon(&$variables) {
  $variables['hot_threshold'] = variable_get('forum_hot_topic', 15);
  if ($variables['num_posts'] > $variables['hot_threshold']) {
    $variables['icon'] = $variables['new_posts'] ? 'hot-new' : 'hot';
  }
  else {
    $variables['icon'] = $variables['new_posts'] ? 'new' : 'default';
  }

  if ($variables['comment_mode'] == COMMENT_NODE_READ_ONLY || $variables['comment_mode'] == COMMENT_NODE_DISABLED) {
    $variables['icon'] = 'closed';
  }

  if ($variables['sticky'] == 1) {
    $variables['icon'] = 'sticky';
  }
}

/**
 * Process variables to format submission info for display in the forum list and topic list.
 *
 * $variables will contain: $topic
 *
 * @see forum-submitted.tpl.php
 * @see theme_forum_submitted()
 */
function template_preprocess_forum_submitted(&$variables) {
  $variables['author'] = isset($variables['topic']->uid) ? theme('username', $variables['topic']) : '';
  $variables['time'] = isset($variables['topic']->timestamp) ? format_interval(time() - $variables['topic']->timestamp) : '';
}

/*****************************************************************************/
/******************************** TAXONOMY ***********************************/
/*****************************************************************************/

function taxonomy_vocabulary_load($vid) {
  static $vocabularies = array();

  if (!isset($vocabularies[$vid])) {
    // Initialize so if this vocabulary does not exist, we have
    // that cached, and we will not try to load this later.
    $vocabularies[$vid] = FALSE;
    // Try to load the data and fill up the object.
    $result = db_query('SELECT v.*, n.type FROM {vocabulary} v LEFT JOIN {vocabulary_node_types} n ON v.vid = n.vid WHERE v.vid = %d', $vid);
    $node_types = array();
    while ($voc = db_fetch_object($result)) {
      if (!empty($voc->type)) {
        $node_types[$voc->type] = $voc->type;
      }
      unset($voc->type);
      $voc->nodes = $node_types;
      $vocabularies[$vid] = $voc;
    }
  }

  // Return NULL if this vocabulary does not exist.
  return !empty($vocabularies[$vid]) ? $vocabularies[$vid] : NULL;
}

/*****************************************************************************/
/******************************** COMMENT ************************************/
/*****************************************************************************/

function comment_new_page_count($num_comments, $new_replies, $node) {
  $comments_per_page = _comment_get_display_setting('comments_per_page', $node);
  $mode = _comment_get_display_setting('mode', $node);
  $order = _comment_get_display_setting('sort', $node);
  $pagenum = NULL;
  $flat = in_array($mode, array(COMMENT_MODE_FLAT_COLLAPSED, COMMENT_MODE_FLAT_EXPANDED));
  if ($num_comments <= $comments_per_page || ($flat && $order == COMMENT_ORDER_NEWEST_FIRST)) {
    // Only one page of comments or flat forum and newest first.
    // First new comment will always be on first page.
    $pageno = 0;
  }
  else {
    if ($flat) {
      // Flat comments and oldest first.
      $count = $num_comments - $new_replies;
    }
    else {
      // Threaded comments. See the documentation for comment_render().
      if ($order == COMMENT_ORDER_NEWEST_FIRST) {
        // Newest first: find the last thread with new comment
        $result = db_query('(SELECT thread FROM {comments} WHERE nid = %d  AND status = 0 ORDER BY timestamp DESC LIMIT %d) ORDER BY thread DESC LIMIT 1', $node->nid, $new_replies);
        $thread = db_result($result);
        $result_count = db_query("SELECT COUNT(*) FROM {comments} WHERE nid = %d AND status = 0 AND thread > '". $thread ."'", $node->nid);
      }
      else {
        // Oldest first: find the first thread with new comment
        $result = db_query('(SELECT thread FROM {comments} WHERE nid = %d  AND status = 0 ORDER BY timestamp DESC LIMIT %d) ORDER BY SUBSTRING(thread, 1, (LENGTH(thread) - 1)) LIMIT 1', $node->nid, $new_replies);
        $thread = substr(db_result($result), 0, -1);
        $result_count = db_query("SELECT COUNT(*) FROM {comments} WHERE nid = %d AND status = 0 AND SUBSTRING(thread, 1, (LENGTH(thread) - 1)) < '". $thread ."'", $node->nid);
      }
      $count = db_result($result_count);
    }
    $pageno =  $count / $comments_per_page;
  }
  if ($pageno >= 1) {
    $pagenum = "page=". intval($pageno);
  }
  return $pagenum;
}
