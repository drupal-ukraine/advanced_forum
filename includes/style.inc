<?php
// $Id$

/**
 * @file
 * Functions relating to the style system, not including core hooks and
 * preprocess / theme functions.
 */

// STYLE API ****************************************************************/
/**
 * Returns the name of the forum style to use.
 */
function advanced_forum_get_current_style() {
  return variable_get('advanced_forum_style', 'blue_lagoon_stacked');
}

function advanced_forum_get_style($style) {
  ctools_include('plugins');
  return ctools_get_plugins('advanced_forum', 'styles', $style);
}

function advanced_forum_get_all_styles() {
  ctools_include('plugins') ;
  return ctools_get_plugins('advanced_forum', 'styles');
}

/**
 * Returns the path to the advanced forum style, including the style name
 */
function advanced_forum_path_to_style($requested_style = NULL) {
  // Set a static variable so this is cached after the first call.
  static $style_paths = array();

  if (empty($requested_style)) {
    // If no style is passed in, assume the current style is wanted.
    $requested_style = advanced_forum_get_current_style();
  }

  if (empty($style_path)) {
    // Get the path information
    $styles = advanced_forum_get_all_styles();
    foreach ($styles as $key => $style) {
      $style_paths[$key] = $style['path'];
    }
  }
  return $style_paths[$requested_style];
}

/**
 * Starting at a given style, return paths of it and all ancestor styles
 */
function advanced_forum_style_lineage($style = NULL) {
  static $lineages = array();
  if (empty($style)) {
    // If no style is passed in, assume the current style is wanted.
    $style = advanced_forum_get_current_style();
  }

  if (!array_key_exists($style, $lineages)) {
    $lineage = array();

    // Get an array with information from all styles.
    $all_styles = advanced_forum_get_all_styles();

    // Add the current style to the lineage first
    $lineage[$style] = $all_styles[$style]['path'];

    // Check if there is an "extra style" listed. This allows you to grab the
    // CSS of one other style and toss it in as a pseudo parent. We do not
    // follow the path up its parent. The primary use is for adding in the
    // "stacked" CSS but could be used for other purposes as well.
    if (!empty($all_styles[$style]['extra style']) && !empty($all_styles[$all_styles[$style]['extra style']]['path'])) {
      $extra_style = $all_styles[$style]['extra style'];
      $lineage[$extra_style] = $all_styles[$extra_style]['path'];
    }

    // Grab the style we are looking at. This variable starts out as the current
    // style in use on the page but will change as we move up the chain.
    $current_style = $style;

    while (!empty($all_styles[$current_style]['base style'])) {
      // There is a parent style, so move up to it.
      $current_style = $all_styles[$current_style]['base style'];

      // Make sure the listed parent style actually exists.
      if (!empty($all_styles[$current_style]['path'])) {
        // Add the style to our list.
        $lineage[$current_style] = $all_styles[$current_style]['path'];
      }
    }

    $lineages[$style] = $lineage;
  }

  return $lineages[$style];
}

/**
 * Load any .inc files related to the style so that preprocess
 * functions can run as appropriate.
 */
function advanced_forum_load_style_includes($style = NULL) {
  $lineage = advanced_forum_style_lineage($style);
  foreach ($lineage as $key => $path) {
    if (file_exists("$path/$key.inc")) {
      require_once("$path/$key.inc");
    }
  }
}
/**
 * Get the info for a style, using an additive notation to
 * include all items from the parent lineage.
 */
function advanced_forum_style_info($style = NULL) {
  static $info = array();
  if (empty($style)) {
    // If no style is passed in, assume the current style is wanted.
    $style = advanced_forum_get_current_style();
  }

  if (!array_key_exists($style, $info)) {
    $info[$style] = array();
    $lineage = advanced_forum_style_lineage();
    foreach ($lineage as $key => $dir) {
      // using the + operator is additive but will not overwrite.
      // $lineage comes out as the actual style with each
      // successive style after it, so simply adding the arrays
      // together means that all info will be combined and keys
      // in the parent info will be defaults passed down to children
      // only if they do not override them.
      $info[$style] += advanced_forum_get_style($key);
    }
  }

  return $info[$style];
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

// CTOOLS INTEGRATION FOR STYLES ********************************************/
function advanced_forum_ctools_plugin_styles() {
  return array(
    'info file' => TRUE,
    'load themes' => TRUE,
  );
}
