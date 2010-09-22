<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_node(&$variables) {
  /* Shortcut variables */
  $node = $variables['node'];

  // This will be empty if not a Node Comment node.
  $node_parent_id = $variables['node']->comment_target_nid;

  /* Determine the template file to use for the node. */
  if (!$node_parent_id && isset($_GET['page']) && $_GET['page'] > 0) {
    // This is the repeated node on the top of subsequent pages.
    // We send this to a special .tpl so people can adjust it to their needs.
    advanced_forum_add_template_suggestions("post-repeated", $variables['template_files']);
  }
  elseif (arg(1) == 'reply' || (arg(0) == 'node' && arg(1) == 'add') || $variables['node']->build_mode == NODE_BUILD_PREVIEW) {
    // 'reply' deals with the first post being shown when adding a comment.
    // 'node/add' is when previewing a new forum post.
    advanced_forum_add_template_suggestions("post-preview", $variables['template_files']);
  }
  else {
    // Use our combined node/comment template file
    advanced_forum_add_template_suggestions("post", $variables['template_files']);
  }

  /* Topic header */
  // Check if this node is the first post, aka topic head. When using the
  // comment module, this will always be true because replies are not nodes
  // but nodecomment will use this same preprocess for all posts.
  if (!$node_parent_id) {
    $variables['top_post'] = TRUE;

    // Build the topic header
    $variables['topic_header'] = theme('advanced_forum_topic_header', $node, $variables['comment_count']);
  }

  /* Assemble all the classes & id that go on the main div. */
  $classes = array();

  // Add in our classes.
  $classes[] = 'forum-post clear-block';

  // Add the current language to the classes for image handling.
  global $language;
  if (!empty($language->language)) {
    $classes[] = $language->language;
  }

   // Add the poster's UID
  $classes[] = "posted-by-$node->uid";

  // Add class if the poster is the viewer.
  global $user;
  if ($user->uid > 0 && $user->uid == $comment->uid) {
    $classes[] = "post-by-viewer";
  }

  // Add class if the poster is the topic starter (only on NC node).
  if ($node_parent_id && $node->uid > 0 && $node->uid == $comment->uid) {
    $classes[] = "post-by-starter";
  }

  // Create the template variable.
  $variables['advanced_forum_classes'] = implode(' ', $classes);

  // Set the post ID for theming / targetting
  $variables['post_id'] = "post-$node->nid";


  /* Coment anchor */
  if ($node_parent_id) {
    // We need to add this because nodecomment puts this in the template
    // that we are overriding whereas the comment module adds it in code
    // outside of the template and it will be there twice if we always add it.
    $variables['comment_anchor'] = '<a id="comment-' . $node->nid . '"></a>';
  }

  /* Linked post number */
  if (!isset($post_number)) {
    static $post_number = 1;
  }

  $page_number = !empty($_GET['page']) ? $_GET['page'] : 0;

  if (!$node_parent_id) {
    // If this is the topic starting node, we need to start off the numbering.
    $variables['post_link'] = l('#1', "node/$node->nid");
  }
  else {
    // This is a nodecomment node so we want to do the comment counting.
    $parent_node = node_load($node_parent_id);
    $post_per_page = _comment_get_display_setting('comments_per_page', $parent_node);
    $post_number++;
    $linktext = '#' . (($page_number * $post_per_page) + $post_number);
    $linkpath = "node/$parent_node->nid";
    $query = ($page_number) ? 'page=' . $page_number : NULL;
    $fragment = 'comment-' . $node->nid;

    $variables['post_link'] = l($linktext, $linkpath, array('query' => $query, 'fragment' => $fragment));

  }

  /* User information / author pane */
  $variables['account'] = user_load(array('uid' => $node->uid));

  // Only do this if post rendering is supported and it's not preview of nodecomment:
  if (!$variables['top_post'] && method_exists('views_plugin_cache', 'post_render') && $variables['node']->build_mode != NODE_BUILD_PREVIEW) {
    // Place a token that will later be replaced by the author pane, which
    // will keep the pane fresh even when the view result is cached.
    $variables['author_pane'] = '<!--post:author-pane-' . $node->uid . '-->';
  }
  elseif (module_exists('author_pane')) {
    $variables['author_pane'] = theme('author_pane', $variables['account'], 'advanced_forum', variable_get('advanced_forum_user_picture_preset', ''), $node, TRUE);
  }
  else {
    $variables['author_pane'] = theme('advanced_forum_simple_author_pane', $node);
  }

  /* Signatures */
  // Load the signature.
  if (module_exists('signature_forum')) {
    // If Signature For Forums is installed, use that
    $variables['signature'] = signature_forum_get_signature($node);
  }
  elseif (variable_get('user_signatures', 0)) {
    if ($variables['account']->signature) {
      // Otherwise load Drupal's built in one, if enabled.
      $variables['signature'] = check_markup($variables['account']->signature, $variables['account']->signature_format, FALSE);
    }
  }

  /* Reply link */
  if (!$node_parent_id) {
    // Build the reply link / button. This isn't used in the default template
    // but is provided as a convenience to people who want to customize theirs.
    $variables['reply_link'] = theme('advanced_forum_reply_link', $node);
  }
}