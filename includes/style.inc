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
  return variable_get('advanced_forum_style', 'cloudless_day_stacked');
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

function advanced_forum_is_styled($content, $type = 'node') {
  // This is the ID of the topic starting node
  static $master_topic_id;
  
  // Give other modules a first crack at making the decision. To keep this 
  // simple, he last one in the array wins.
  $topic_ids = module_invoke_all('advanced_forum_is_styled_alter', $content, $type);
  $topic_id = end($topic_ids);
 
  if (!$topic_id) {
    // If no other modules made a decision on this, we look at it.
    switch ($type) {
      case 'node':
        if (!empty($content->comment_target_nid) && $content->comment_target_nid == $master_topic_id) {
          // This nodecomment is attached to a node we already know is styled.
          $topic_id = $master_topic_id;
        }
        else {
          // See if the node should be styled.
          $topic_id = advanced_forum_node_is_styled($content);
        }
        break;
        
      case 'comment':
        $topic_id = advanced_forum_comment_is_styled($content, $master_topic_id);
        break;
        
      case 'comment-wrapper':
        if ($content->nid == $master_topic_id) {
          // Use our comment wrapper only if we are on a styled node.
          $topic_id = $master_topic_id;
        }
        break;
      }   
    }      
  
  if ($topic_id > 0) {
    // If we have a positive number for the topic ID, then this item is styled.
    // We need to update the master ID, add the CSS/JS files, and tell the caller
    // our decision.
    $master_topic_id = $topic_id;
    _advanced_forum_add_files();
    return TRUE;
  }
  elseif ($topic_id == -42) {
    // A -2 means all the tests passed but the node is being previewed so there
    // is no node id, yet. Add the files and return true but don't update the 
    // master topic ID.
    _advanced_forum_add_files();
    return TRUE;
  }
    
}

function advanced_forum_node_is_styled($node) {
  // Get the list of types that should have the style applied
  $styled_node_types = variable_get('advanced_forum_styled_node_types', array());

  // If this type is in the list of types that should be styled...
  if (in_array($node->type, $styled_node_types)) {
    // ...and if we are styling teasers or this isn't a teaser...
    if (variable_get('advanced_forum_style_teasers', FALSE) || empty($node->teaser)) {
      // ...and if this is not a new node being previewed...
      if (!empty($node->nid)) {      
        // ...and if we are styling non forum tagged nodes or this is forum tagged...
        if (!variable_get('advanced_forum_style_only_forum_tagged', TRUE) || advanced_forum_is_forum_tagged($node)) {
          // ... then return the node ID.
          return $node->nid;
        }
      }
      // ...or if this _is_ a new node being previewed...
      else {
        // ...and if we are styling non forum tagged nodes or this is forum tagged...
        if (!variable_get('advanced_forum_style_only_forum_tagged', TRUE) || advanced_forum_is_forum_tagged($node, TRUE)) {
          // ...Send back -42 as a special code to say that this should be styled
          // but that it isn't the actual node id.
          return -42;
        }
      }
    }
  }
}

function advanced_forum_comment_is_styled($comment, $master_topic_id) {
    if (isset($master_topic_id) && ($comment->nid == $master_topic_id)) {
      // This comment is on a node we already know is styled, or a previous comment on 
      // this node is styled, so the comment is styled.
      return $master_topic_id;
    }
    
    if (variable_get("advanced_forum_style_all_comments", 0)) {
      // All comments on this site should be styled.
      return $comment->nid;
    }
    
    // Load up the node this comment is attached to and see if it is styled.
    // This should not happen often, only in a case where the comment is displayed
    // seperately from the node.
    $node = node_load($comment->nid);
    if (advanced_forum_node_is_styled($node)) {
      return $comment->nid;
    }
    
    // Comment doesn't pass any styling test
    return -1;
}

function advanced_forum_is_forum_tagged($node, $preview = FALSE) {
  $vid = variable_get('forum_nav_vocabulary', '');  
  if ($preview) {
    foreach ($node->taxonomy as $term) {
      if ($term->vid == $vid) {
        return TRUE;
      }
    }
  }
  else {
    // Get a list of the terms attached to this node
    $terms = taxonomy_node_get_terms_by_vocabulary($node, $vid);
    if (count($terms) > 0) {
      return TRUE;
    }
  }
}

// CTOOLS INTEGRATION FOR STYLES ********************************************/
function advanced_forum_ctools_plugin_styles() {
  return array(
    'info file' => TRUE,
    'load themes' => TRUE,
  );
}
