<?php
// $Id$

/**
 * Callback function to supply a list of content types.
 */
function advanced_forum_forum_most_active_poster_ctools_content_types() {
  if (module_exists('nodecomment')) {
    return array(
      'single' => TRUE,
      'title' => t('Most active forum poster'),
      'icon' => 'icon_forum.png',
      'description' => t('A block with information about the most active forum poster.'),
      'required context' => new ctools_context_required(t('Forum'), 'forum'),
      'category' => t('Forum'),
      'defaults' => array(),
    );
  }
}

/**
 * Render the content.
 */
function advanced_forum_forum_most_active_poster_content_type_render($subtype, $conf, $panel_args, $context) {
  if (!empty($context) && empty($context->data)) {
    return;
  }

  if (empty($context)) {
    $tid = 0;
  }
  else {
    $tid = $context->data->tid;
  }

  $block = new stdClass();
  $block->module = 'forum-active-poster';
  $block->delta  = $tid;
  _advanced_forum_add_files();

  $query = "SELECT n.uid, COUNT(DISTINCT n.nid) AS posts FROM {node} n ";
  $query .= "LEFT JOIN {node_comments} nc ON n.nid = nc.cid ";
  $query .= "INNER JOIN {node} n2 ON COALESCE(nc.nid, n.nid) = n2.nid ";
  $query .= "INNER JOIN {forum} f ON f.vid = n2.vid ";
  $query .= "WHERE n2.status <> 0 AND n.uid <> 0 ";
  $args = array();
  if ($tid) {
    $forum = taxonomy_get_term($tid);
    $block->title = t('Most active poster in @forum', array('@forum' => $forum->name)); // by default this has no title.
    $query .= "AND f.tid = %d ";
    $args = array($tid);
  }
  else {
    $block->title = t('Most active poster'); // by default this has no title.
  }
  $query .= "GROUP BY n.uid ";
  $query .= "ORDER BY COUNT(DISTINCT n.nid) DESC LIMIT 1";

  $info = db_fetch_object(db_query($query, $args));

  // This can happen if there are no posts.
  if (!$info) {
    return;
  }

  $account = user_load($info->uid);

  // Now that we have a user and that user's # of posts, get # of topics for
  // this forum.
  $args = array($info->uid);
  $query = "SELECT COUNT(n.nid) AS posts FROM {node} n ";
  $query .= "INNER JOIN {forum} f ON f.vid = n.vid ";
  $query .= "WHERE n.status <> 0 AND n.uid = %d";
  if ($tid) {
    $query .= " AND f.tid = %d";
    $args[] = $tid;
  }

  $topics = db_result(db_query($query, $args));

  // And get their last post.
  $args = array($info->uid);
  $query = "SELECT n.nid FROM {node} n ";
  $query .= "LEFT JOIN {node_comments} nc ON n.nid = nc.cid ";
  $query .= "INNER JOIN {node} n2 ON COALESCE(nc.nid, n.nid) = n2.nid ";
  $query .= "INNER JOIN {forum} f ON f.vid = n2.vid ";
  $query .= "WHERE n.status <> 0 AND n.uid = %d ";
  if ($tid) {
    $query .= " AND f.tid = %d ";
    $args[] = $tid;
  }
  $query .= "ORDER BY n.created DESC LIMIT 1";

  $last_post = node_load(db_result(db_query($query, $args)));
  $block->content = theme('advanced_forum_active_poster', $tid, $account, $info->posts, $topics, $last_post);
  $block->more = array(
    'title' => t('More posts'),
    'href' => 'forum/user/' . $info->uid,
  );
  return $block;
}

/**
 * Returns an edit form for the custom type.
 */
function advanced_forum_forum_most_active_poster_content_type_edit_form(&$form, &$form_state) {
}

function advanced_forum_forum_most_active_poster_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function advanced_forum_forum_most_active_poster_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" most active poster', array('@s' => $context->identifier));
}
