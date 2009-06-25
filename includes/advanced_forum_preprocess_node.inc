<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to easy memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_node(&$variables) {
  /* Determine the template file to use for the node. */
  if (empty($variables['node']->comment_target_nid) && isset($_GET['page']) && $_GET['page'] > 0) {
    // This is the repeated node on the top of subsequent pages.
    // We send this to a special .tpl so people can adjust it to their needs.
    $variables['template_files'][] = "advanced_forum-post-repeated";
  }
  elseif (arg(1) == 'reply' || (arg(0) == 'node' && arg(1) == 'add')) {
    // 'reply' deals with the first post being shown when adding a comment.
    // 'node/add' is when previewing a new forum post.
    $variables['template_files'][] = "advanced_forum-post-preview";
  }
  else {
    // Use our combined node/comment template file
    $variables['template_files'][] = "advanced_forum-post";
  }

  /* Reply ID - Nodecomment only */
  if (!empty($variables['node']->comment_target_nid)) {
    $variables['reply_id'] = $variables['node']->nid;
  }

  /* Topic header */
  // Check if this node is the first post, aka topic head. When using the
  // comment module, this will always be true because replies are not nodes
  // but nodecomment will use this same preprocess for all posts.
  if (empty($variables['node']->comment_target_nid)) {
    $variables['top_post'] = TRUE;

    // Build the topic header
    $variables['topic_header'] = theme('advanced_forum_topic_header', $variables['node'], $variables['comment_count']);
  }

  /* Link section */
  // Change first post from "add comment" to "Reply"
  if (!empty($variables['node']->links) && !empty($variables['node']->links['comment_add'])) {
    $variables['node']->links['comment_add']['title'] = t('Reply');
  }

  // Remake the links with our changes
  $variables['links'] = theme('links', $variables['node']->links, array('class' => 'links inline forum-links'));

  // Make an array version of $links
  $variables['links_array'] = $variables['node']->links;

  /* User information / author pane */
  $variables['account'] = user_load(array('uid' => $variables['node']->uid));
  // Only do this if post rendering is supported:
  if (method_exists('views_plugin_cache', 'post_render')) {
    // Place a token that will later be replaced by the author pane, which
    // will keep the pane fresh even when the view result is cached.
    $variables['author_pane'] = '<!--post:author-pane-' . $variables['node']->uid . '-->';
  }
  else {
    $variables['author_pane'] = theme('author_pane', $variables['account'], '', 'advanced_forum-author-pane');
  }
  /* Signatures */
  if (module_exists('signature_forum')) {
    // If Signature For Forums is installed, use that
    $variables['signature'] = signature_forum_get_signature($variables['node']);
  }
  elseif (variable_get('user_signatures', 0)) {
    // Otherwise load Drupal's built in one, if enabled.
    $variables['signature'] = check_markup($variables['account']->signature, $variables['node']->format);
  }

  // Node-comment specific variables
  if (!empty($variables['node']->comment_target_nid)) {
    static $post_number = 0;
    $parent_node = node_load($variables['node']->comment_target_nid);
    $post_per_page = _comment_get_display_setting('comments_per_page', $parent_node);
    $page_number = !empty($_GET['page']) ? $_GET['page'] : 0;
    $post_number++;

    $fragment = 'comment-' . $variables['node']->nid;
    $query = ($page_number) ? 'page=' . $page_number : NULL;
    $linktext = '#' . (($page_number * $post_per_page) + $post_number);
    $linkpath = 'node/' . $parent_node->nid;
    $variables['comment_link'] = l($linktext, $linkpath, array('query' => $query, 'fragment' => $fragment));
  }
}