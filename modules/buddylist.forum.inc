<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for buddylist (1) module
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function buddylist_preprocess_author_pane(&$variables) {
  global $user;
  $account_id = $variables['account']->uid;
  if (!user_access('maintain buddy list') || $user->uid == $account_id) {
    // User doesn't have access to buddy list or user is poster.
    return;
  }
  
  if (@in_array($account_id, array_keys(buddylist_get_buddies($user->uid)))) {
    // Remove buddy
    $variables['buddylist'] =   
      l(theme('image', advanced_forum_path_to_images() . '/buddy-remove.png', 
              t('Remove from buddylist'), t('Remove from buddylist'), NULL, TRUE), 
        "buddy/delete/$account_id",
        array(), NULL, NULL, FALSE, TRUE); 
    
    $variables['buddylist_link'] = l("Remove buddy", "buddy/delete/$account_id");
  } 
  else {
    // Add buddy
    $variables['buddylist'] =   
      l(theme('image', advanced_forum_path_to_images() . '/buddy-add.png', 
              t('Add to buddylist'), t('Add to buddylist'), NULL, TRUE), 
        "buddy/add/$account_id",
        array(), NULL, NULL, FALSE, TRUE); 
    
    $variables['buddylist_link'] = l("Add buddy", "buddy/add/$account_id");
  }
}