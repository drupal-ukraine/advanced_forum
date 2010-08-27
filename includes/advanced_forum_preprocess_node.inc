<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_node(&$variables) {
  // Define this to avoid notices when not using Node Comments
  $variables['css_id'] = "";
  
  // Start a variable that will hold classes to attach to the containing div.
  $classes = array();
  
  // Some themes use $node_classes instead of $classes.
  if (!empty($variables['node_classes'])) {
    $classes[] = $variables['node_classes'];
  }
  
  // Add in our classes.
  $classes[] = 'forum-post clear-block';

  /* Determine the template file to use for the node. */
  if (empty($variables['node']->comment_target_nid) && isset($_GET['page']) && $_GET['page'] > 0) {
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

  /* Nodecomment only */
  if (!empty($variables['node']->comment_target_nid)) {
    $variables['reply_id'] = $variables['node']->nid;
    
    // We need to add this because nodecomment puts this in the template
    // that we are overriding whereas the comment module adds it in code
    // outside of the template and it will be there twice if we always add it.
    $variables['comment_anchor'] = '<a id="comment-' . $variables['node']->nid . '"></a>';
  }

  /* Topic header */
  // Check if this node is the first post, aka topic head. When using the
  // comment module, this will always be true because replies are not nodes
  // but nodecomment will use this same preprocess for all posts.
  if (empty($variables['node']->comment_target_nid)) {
    $variables['top_post'] = TRUE;

    // Build the topic header
    $variables['topic_header'] = theme('advanced_forum_topic_header', $variables['node'], $variables['comment_count']);
    
    // Add to the classes
    $classes[] = 'top-post';
    $variables['css_id'] = "node-" . $variables['node']->nid;
  } 

  /* User information / author pane */
  $variables['account'] = user_load(array('uid' => $variables['node']->uid));
  // Only do this if post rendering is supported and it's not preview of nodecomment:
  if (!$variables['top_post'] && method_exists('views_plugin_cache', 'post_render') && $variables['node']->build_mode != NODE_BUILD_PREVIEW) {    
    // Place a token that will later be replaced by the author pane, which
    // will keep the pane fresh even when the view result is cached.
    $variables['author_pane'] = '<!--post:author-pane-' . $variables['node']->uid . '-->';
  }
  else {
    $variables['author_pane'] = theme('author_pane', $variables['account'], 'advanced_forum', variable_get('advanced_forum_user_picture_preset', ''), $variables['node'], TRUE);
  }
  
  /* Signatures */
  // Load the signature.
  if (module_exists('signature_forum')) {
    // If Signature For Forums is installed, use that
    $variables['signature'] = signature_forum_get_signature($variables['node']);
  }
  elseif (variable_get('user_signatures', 0)) {
    if ($variables['account']->signature) {
      // Otherwise load Drupal's built in one, if enabled.
      $variables['signature'] = check_markup($variables['account']->signature, $variables['account']->signature_format, FALSE);
    }
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
    
    $variables['css_id'] = 'reply-' . $variables['node']->nid;
  }
  
  // Fetch the current language
  global $language;
  $classes[] = $language->language;

  // Node classes
  if (empty($variables['classes'])) {
    $variables['classes'] = implode(' ', $classes);
  }
  else {
    $variables['classes'] .= ' ' . implode(' ', $classes);  
  }
}