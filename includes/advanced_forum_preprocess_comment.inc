<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_comment(&$variables) {
  // Start a variable that will hold classes to attach to the containing div
  $classes = array();
 
  // Some themes use $comment_classes instead of $classes.
  if (!empty($variables['comment_classes'])) {
    $classes[] = $variables['comment_classes'];
  }
  
  // Add in our classes.
  $classes[] = 'forum-post clear-block';

  /* Make the topic starting node available */
  $node = node_load($variables['comment']->nid);
  $variables['first_post'] = $node;

  /* Determine the template file to use for the comment. */
  if (arg(1) == 'reply' || arg(1) == 'edit') {
    // Use the preview version
    advanced_forum_add_template_suggestions("post-preview", $variables['template_files']);
  }
  else {
    // Use our combined node/comment template file
    advanced_forum_add_template_suggestions("post", $variables['template_files']);
  }

  // This is a comment, not the node.
  $variables['top_post'] = FALSE;
  $variables['css_id'] = "reply-" . $variables['node']->nid;

  // Set the reply ID for theming / targetting
  $variables['reply_id'] = $variables['comment']->cid;

  // Assign the comment to the content variable for consistancy with nodes.
  $variables['content'] = $variables['comment']->comment;

  /* User information / author pane */
  $account_id = $variables['comment']->uid;
  if ($account_id == 0) {
    // Anonymous user. Make a fake user object for theme_username
    $variables['account']->name = $variables['comment']->name;
    $variables['account']->homepage = $variables['comment']->homepage;
    $variables['account']->email = $variables['comment']->email;    
  }
  else {
    // Load up the real user object
    $variables['account'] = user_load(array('uid' => $variables['comment']->uid));
  }

  // Create the author pane
  $variables['author_pane'] = theme('author_pane', $variables['account'], 'advanced_forum', variable_get('advanced_forum_user_picture_preset', ''));

  /* Link section */
  if (arg(1) != 'reply' && arg(1) != 'edit') {
    // Because the $links array isn't available here, we recreate it
    $links = module_invoke_all('link', 'comment', $variables['comment']);
    drupal_alter('link', $links, $node);
    unset($links['comment_parent']);

    // Add extra span tags for image replacement.
    foreach ($links AS $key => $link) {
      $links[$key]['title'] = $links[$key]['title'] . '<span class="image-replace"></span>';
      $links[$key]['html'] = TRUE;
    }

    // Remake the links with our changes
    $variables['links'] = theme('links', $links, array('class' => 'links inline forum-links'));

    // Make an array version of $links
    $variables['links_array'] = $links;
  }

  /* Title */
  if (variable_get('comment_subject_field_' . $node->type, 1) == 0) {
    // if comment titles are disabled, don't display it.
    $variables['title'] = '';
  }
  else {
    // Assign the subject to the title variable for consistancy with nodes.
    $variables['title'] = check_plain($variables['comment']->subject);
  }

  /* Linked comment number & permalink */
  if (!isset($post_number)) {
    static $post_number = 0;
  }
  _advanced_forum_topic_nid($variables['node']->nid);

  $post_per_page = _comment_get_display_setting('comments_per_page', $variables['node']);
  $page_number = !empty($_GET['page']) ? $_GET['page'] : 0;
  if (!$page_number) {
    $page_number = 0;
  }

  $post_number++;
  $fragment = 'comment-' . $variables['comment']->cid;
  $query = ($page_number) ? 'page=' . $page_number : NULL;
  $linktext = '#' . (($page_number * $post_per_page) + $post_number);
  $linkpath = 'node/' . _advanced_forum_topic_nid();
  $variables['comment_link'] = l($linktext, $linkpath, array('query' => $query, 'fragment' => $fragment));;

  // Link to page created by Comment Page module, if it exists
  $variables['page_link'] = '';
  if (!empty($variables['comment']->page_url) && !(arg(0) == 'comment' && arg(1) == $variables['comment']->cid)) {
    $variables['page_link'] = l(t('(permalink)'), $variables['comment']->page_url);
  }

  /* Signatures */
  // Load the signature.
  if (module_exists('signature_forum')) {
    // If Signature For Forums is installed, use that
    $variables['signature'] = signature_forum_get_signature($variables['comment']);
  }
  elseif (variable_get('user_signatures', 0)) {
    if (!empty($variables['account']->signature)) {
      // Otherwise load Drupal's built in one, if enabled.
      $variables['signature'] = check_markup($variables['account']->signature, $variables['account']->signature_format);
    }
  }
    
  // Fetch the current language
  global $language;
  $classes[] = $language->language;

  // Classes
  if (empty($variables['classes'])) {
    $variables['classes'] = implode(' ', $classes);
  }
  else {
    $variables['classes'] .= ' ' . implode(' ', $classes);  
  }
}