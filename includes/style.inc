<?php
// $Id$

/**
 * @file
 * Functions relating to the style system, not including core hooks and
 * preprocess / theme functions.
 */

// Include the file that handles the plugin system adapted from panels.
include_once drupal_get_path('module', 'advanced_forum') . '/includes/plugins.inc';

function advanced_forum_advanced_forum_styles() {
  $items = array();
  $items['naked'] = array(
    'directory' => 'styles/naked',
  );

  $items['naked_stacked'] = array(
    'directory' => 'styles/naked_stacked',
  );

  $items['blue_lagoon'] = array(
    'directory' => 'styles/blue_lagoon',
  );

  $items['blue_lagoon_stacked'] = array(
    'directory' => 'styles/blue_lagoon_stacked',
  );

  $items['boxy'] = array(
    'directory' => 'styles/boxy',
  );

  $items['boxy_stacked'] = array(
    'directory' => 'styles/boxy_stacked',
  );

  $items['flatgrey'] = array(
    'directory' => 'styles/flatgrey',
  );

  return $items;
}

/**
 * Returns the name of the forum style to use.
 */
function advanced_forum_get_current_style() {
  return variable_get('advanced_forum_style', 'naked');
}

/**
 * Returns the path to actual site theme in use because path_to_theme is flaky.
 */
function advanced_forum_path_to_theme() {
  global $theme;

  if (!empty($theme)) {
    // Normally, the global theme variable is set, so we use that.
    return drupal_get_path('theme', $theme);
  }

  // For some reason I've never figured out, some users are reporting
  // that the global theme variable is not set when this is called.
  // As a band-aid solution, this will pull the default theme out of the
  // database and use that.
  $default_theme = variable_get('theme_default', 'garland');
  return drupal_get_path('theme', $default_theme);
}

/**
 * Returns the path to the advanced forum style, including the style name
 */
function advanced_forum_path_to_style() {
  static $style_path;

  if (!$style_path) {
    // Retrieve the style name
    $style = advanced_forum_get_current_style();

    // Get the path information
    $style_info = advanced_forum_get_style($style);
    $style_path = $style_info['path'];
  }

  return $style_path;
}

/**
 * Returns the path to the advanced forum image directory
 */
function advanced_forum_path_to_images() {
  static $image_path;
  global $language;

  if (!$image_path) {
    // If there is an override in settings, use it
    $image_path = variable_get('advanced_forum_image_directory', '');

    if (empty($image_path)) {
      // Otherwise assume it's in the style's directory
      $image_path = advanced_forum_path_to_style() . '/images';
    }

    if (!file_exists($image_path)) {
      // If the style doesn't have an images directory, fall back to naked.
      $image_path = drupal_get_path('module', 'advanced_forum') . '/styles/naked/images';
    }
  }

  //Add language prefix to path if exist
  $language_prefix = $language->language;
  $image_path_language_prefix = $image_path . '/' . $language_prefix;
  if (file_exists($image_path_language_prefix)) {
    $image_path = $image_path_language_prefix;
  }

  return $image_path;
}

/**
 * Wrapper around theme_image that automatically includes the path.
 */
function advanced_forum_theme_image($image_file, $alt = NULL, $title = NULL) {
  $image = advanced_forum_path_to_images() . "/$image_file";

  if ($title == NULL) {
    $title = $alt;
  }

  if (file_exists($image)) {
    return theme('image', $image, $alt, $title);
  }
  else {
    return FALSE;
  }
}

function _advanced_forum_buttonify_links(&$links) {
  $buttonify = variable_get('advanced_forum_button_links', 1);

  if ($buttonify && !empty($links)) {
    foreach ($links as $link_key => $link) {
      switch ($link_key) {
        // First, take care of the special cases
        case 'comment_add':
          $links[$link_key]['title'] = advanced_forum_theme_image(t('reply.png'), t('reply'));
          $links[$link_key]['html'] = TRUE;
          break;

        case 'comment_reply':
          $links[$link_key]['title'] = advanced_forum_theme_image(t('reply.png'), t('reply'));
          $links[$link_key]['html'] = TRUE;
          break;

        case 'comment_edit':
          $links[$link_key]['title'] = advanced_forum_theme_image(t('edit.png'), t('edit'));
          $links[$link_key]['html'] = TRUE;
          break;

        case 'comment_delete':
          $links[$link_key]['title'] = advanced_forum_theme_image(t('delete.png'), t('delete'));
          $links[$link_key]['html'] = TRUE;
          break;

        default:
          // Attempt to buttonify by key name.
          $image = advanced_forum_theme_image($link_key . '.png', $link['title']);

          if (!$image) {
            // If no luck there, try the adjusted title.
            $title = strtolower(str_replace(' ', '-', $link['title']));
            $image = advanced_forum_theme_image($title . '.png', $link['title']);
          }

          if ($image) {
            // If we found an image, use it. Otherwise leave the title alone.
            $links[$link_key]['title'] = $image;
            $links[$link_key]['html'] = TRUE;
          }
      }
    }
  }
}

/**
 * This function returns TRUE if the node/comment should be themed and
 * otherwise treated as a forum post.
 */
function advanced_forum_treat_as_forum_post($hook, $variables) {
  // Setting this static means the check only needs to be done once per thread
  static $forum_node_id;

  // Start out assuming it's not one of ours.
  $decision = FALSE;

  if ($hook == 'node') {
    if (!empty($variables['node']->comment_target_nid)) {
      // Special case for "comment" nodes
      if ($variables['node']->comment_target_nid == $forum_node_id) {
        $decision = TRUE;
      }
    } 
    else {
      $vid = variable_get('forum_nav_vocabulary', '');
      $vocabulary = taxonomy_vocabulary_load($vid);
      if (empty($vocabulary) || !in_array($variables['node']->type, $vocabulary->nodes)) {
        // No forum vocabulary or the node type cannot be used in the forums
        unset($forum_node_id);
        $decision = FALSE;
      }
      else {
        // Get a list of the terms attached to this node
        $terms = taxonomy_node_get_terms_by_vocabulary($variables['node'], $vid);
        if (count($terms) > 0 && (arg(0) == 'node' && is_numeric(arg(1)) && arg(1) == $variables['node']->nid || arg(0) == 'comment')) {
          // The node has at least one forum term attached to it and is not being
          // shown on some other page (like a view or promoted to front page)
          $forum_node_id = $variables['node']->nid;
          $decision = TRUE;
        }
        else {
          // We've hit a non forum node
          unset($forum_node_id);
          $decision = FALSE;
        }
      }
    }
  }

  if ($hook == 'comment') {
    if (isset($forum_node_id) && ($variables['comment']->nid == $forum_node_id)) {
      // We already know this comment is either part of a forum thread
      // or that comments on this thread are known exceptions.
      $decision = TRUE;
    }
    else {
      // Not part of a forum thread. Check if this site wants all comments to
      // use the forum comment template or the comment is being shown alone via
      // the comment_page module
      if (variable_get("advanced_forum_theme_all_comments", 0) == 1 ||
        (arg(0) == 'comment' && is_numeric(arg(1)))) {
        // This site wants all comments to use the forum comment template
        $forum_node_id = $variables['comment']->nid;

        $decision = TRUE;
      }
      else {
        // If we are on a comment reply page, check if the node/comment gets styled.
        if (arg(0) == 'comment' && arg(1) == 'reply') {
          $nid = arg(2);
          $variables['node'] = node_load($nid);
          $decision = advanced_forum_treat_as_forum_post('node', $variables);
        }
      }
    }
  }

  if ($hook == 'comment-wrapper') {
    if (isset($forum_node_id) && ($variables['node']->nid == $forum_node_id)) {
      // We only want to use our comment wrapper if we are on a node that
      // passed the tests.
      $decision = TRUE;
    }
  }

  // Call a hook with our decisions and the available information. If it returns
  // a number, override our decision and set the static variable to that number.
  $override_nids = module_invoke_all('treat_as_forum_post_alter', $decision, $hook, $variables);

  if (!empty($override_nids)) {
    // At least one module has returned an nid. We are making an assumption here
    // that if there was more than one the nids are the same and just grabbing
    // the first one.
    $forum_node_id = $override_nids[0];
    if ($forum_node_id == -1) {
      unset($forum_node_id);
      $decision = FALSE;
    }
    else {
      $decision = TRUE;
    }
  }

  // If we're claiming this one, there are a few things we need to do...
  if ($decision) {
    _advanced_forum_add_files();
  }

  return $decision;
}

