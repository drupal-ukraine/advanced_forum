<?php


/**
 * @file
 * Holds functions relating to the Mark Forum/All Read functionality.
 */

/**
 * Either fill a $links array or return a string version of the link to mark read.
 */
function advanced_forum_get_mark_read_link($tid = 0, &$links = array()) {
  if (advanced_forum_markasread_access() && !in_array($tid, variable_get('forum_containers', array()))) {
    if ($tid) {
      $links['mark-read']['title'] = t('Mark all topics read');
      $links['mark-read']['href'] = "forum/markasread/$tid";

      return l(t('Mark all topics read') . '<span class="image-replace"></span>', "forum/markasread/$tid", array('html' => TRUE));
    }
    else {
      $links['mark-read']['title'] = t('Mark all forums read');
      $links['mark-read']['href'] = "forum/markasread";

      return l(t('Mark all forums read') . '<span class="image-replace"></span>', "forum/markasread", array('html' => TRUE));
    }
  }
}

/**
 * Marks all posts in forums or in a given forum as read by the current user.
 */
function advanced_forum_markasread_old($current_forum_id = 0) {
  global $user;
  // See if we're on a forum or on the forum overview
  // Path will be /forum/markasread or /forum/markasread/tid
  if ($current_forum_id) {
    // Delete the current history entries so already visited nodes get updated.
    //
    // Can't write dynamic query here because D7 DeleteQuery (still) doesn't support joins
    $sql = "DELETE h
            FROM {history} AS h
              INNER JOIN {forum} AS f ON (h.nid = f.nid)
            WHERE h.uid = :uid AND f.tid = :tid";
    db_query($sql, array(':uid' => $user->uid, ':tid' => $current_forum_id));

    // Update the history table with all forum nodes newer than the cutoff.
    //
    // Can't write dynamic query here because D7 InsertQuery (still) doesn't support joins
    $sql = "INSERT INTO {history} (uid, nid, timestamp)
            SELECT DISTINCT :uid, n.nid, :req_time
            FROM {node} AS n
              INNER JOIN {forum} AS f ON (n.nid = f.nid)
              INNER JOIN {node_comment_statistics} AS ncs ON ncs.nid = n.nid
            WHERE (n.changed > :changed OR ncs.last_comment_timestamp > :timestamp) AND f.tid = :tid";

    db_query($sql, array(
      ':uid' => $user->uid,
      ':req_time' => REQUEST_TIME,
      ':changed' => NODE_NEW_LIMIT,
      ':timestamp' => NODE_NEW_LIMIT,
      ':tid' => $current_forum_id
    ));

    // Readpath integration
    if (module_exists('readpath')) {
      readpath_clear_readpath();
    }

    drupal_set_message(t('All content in this forum has been marked as read'));
    drupal_goto('forum/' . $current_forum_id);
  }

  // We are on the forum overview, requesting all forums be marked read
  $forum_vocabulary_id = variable_get('forum_nav_vocabulary', '');

  // Delete the current history entries so already visited nodes get updated.
  $sql = "DELETE h
          FROM {history} AS h
            INNER JOIN {forum} AS f ON (h.nid = f.nid)
            INNER JOIN {taxonomy_term_data} AS td ON (td.tid = f.tid)
          WHERE h.uid = :uid AND td.vid = :vid";

  db_query($sql, array(':uid' => $user->uid, ':vid' => $forum_vocabulary_id));

  // Update the history table with all forum nodes newer than the cutoff.
  $sql = "INSERT INTO {history} (uid, nid, timestamp)
            SELECT DISTINCT :uid, n.nid, :req_time
            FROM {node} AS n
              INNER JOIN {forum} AS f ON (n.nid = f.nid)
              INNER JOIN {node_comment_statistics} AS ncs ON ncs.nid = n.nid
              INNER JOIN {taxonomy_term_data} AS td ON f.tid = td.tid
            WHERE (n.changed > :changed OR ncs.last_comment_timestamp > :timestamp) AND td.vid = :vid";

   db_query($sql, array(
      ':uid' => $user->uid,
      ':req_time' => REQUEST_TIME,
      ':changed' => NODE_NEW_LIMIT,
      ':timestamp' => NODE_NEW_LIMIT,
      ':vid' => $forum_vocabulary_id
    ));

  drupal_set_message(t('All forum content been marked as read'));
  drupal_goto('forum');
}

function advanced_forum_markasread($term = NULL) {
  global $user;
  $vid = variable_get('forum_nav_vocabulary', 0);

  // Don't bother trying to mark things read for anonymous users.
  if (empty($user->uid)) {
    return;
  }

  // Delete all entries in the history table for the current $uid and
  // optionally a forum term id.

  // Subquery to find nids to delete from history table.
  $query_node = db_select('node', 'n');
  $query_node->join('forum', 'f', 'n.nid = f.nid');
  $query_node->join('taxonomy_term_data', 't', 'f.tid = t.tid');
  $query_node->join('node_comment_statistics', 'ncs', 'n.nid = ncs.nid');
  $query_node->condition(db_or()
    ->condition('n.changed', NODE_NEW_LIMIT, '>')
    ->condition('ncs.last_comment_timestamp', NODE_NEW_LIMIT, '>')
  );
  if (isset($term)) {
    $query_node->condition('f.tid', $term['tid']);
  }
  $query_node->condition('t.vid', $vid);
  $query_node->addField('n', 'nid');
  // Select query objects are one-shot, so clone for INSERT below.
  $query_history_insert = clone($query_node);
  // Delete values based upon sub-query.
  $query = db_delete('history')
    ->condition('uid', $user->uid)
    ->condition('nid', $query_node, 'IN')
    ->execute();

  // Now insert the nids into the history table.
  $query_history_insert->addExpression(':uid', 'uid', array(':uid' => $user->uid));
  $query_history_insert->addExpression(':time', 'timestamp', array(':time' => time()));

  db_insert('history')
    ->fields(array('nid', 'uid', 'timestamp'))
    ->from($query_history_insert)
    ->execute();

  drupal_goto(empty($term) ? 'forum' : 'forum/' . $term);
}
/**
 * Access callback for menus and link display.
 *
 * @TODO: D7 check if needed
 *
 * This separate function is needed because the Drupal 6 menu system doesn't
 * run hook_menu() every time and the logged-in status of the user can get
 * cached and re-used for other users.
 */
function advanced_forum_markasread_access() {
  global $user;
  return user_access('access content') && $user->uid;
}

