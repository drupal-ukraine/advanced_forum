<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for buddylist (1) module
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function buddylist_preprocess_forum_user(&$variables) {
  global $user;
  $account_id = $variables['account']->uid;
  if (!user_access('maintain buddy list') || $user->uid == $account_id) {
    // User doesn't have access to buddy list or user is poster.
    return;
  }
  
  if (@in_array($account_id, array_keys(buddylist_get_buddies($user->uid)))) {
    // Remove buddy
    $variables['buddylist_icon'] = theme('image', advanced_forum_path_to_images() . '/buddy_remove.png', t('Remove Buddy'), t('Remove Buddy'), NULL, TRUE);
    $variables['buddylist_link'] = "buddy/delete/$account_id";
    $variables['buddylist'] = '<div class="buddylist">' . 
      l($variables['buddylist_icon'], $variables['buddylist_link'], NULL, NULL, NULL, NULL, TRUE) . 
      ' '  . 
      l(t('Remove buddy'), $variables['buddylist_link']) .
      '</div>';
  } 
  else {
    // Add buddy
    $variables['buddylist_icon'] = theme('image', advanced_forum_path_to_images() . '/buddy_add.png', t('Add to buddy list'), t('Add to buddy list'), NULL, TRUE);
    $variables['buddylist_link'] = "buddy/add/$account_id";
    $variables['buddylist'] = '<div class="buddylist">' . 
      l($variables['buddylist_icon'], $variables['buddylist_link'], NULL, NULL, NULL, NULL, TRUE) . 
      ' '  . 
      l(t('Add buddy'), $variables['buddylist_link']) .
      '</div>';
  }
}