<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for buddylist (1) module
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function buddylist_preprocess_forum_user(&$variables) {
  $accountid = $variables['account']->uid;

  if (user_access('maintain buddy list') && @in_array($accountid, array_keys(buddylist_get_buddies($user->uid)))) {
    // Remove buddy
    $variables['buddylist_class'] = "buddy-remove";
    $variables['buddylist_icon'] = theme('image', advanced_forum_get_icon_directory() . 'group_delete.png', 'Remove Buddy', 'Remove Buddy', NULL, TRUE);
    $variables['buddylist_text'] = t('Remove buddy');
    $variables['buddylist_link'] = "buddy/delete/$accountid";
  } 
  else {
    // Add buddy
    if ($accountid != $user->uid && user_access('maintain buddy list')) {
      $variables['buddylist_class'] = "buddy-add";
      $variables['buddylist_icon'] = theme('image', advanced_forum_get_icon_directory() . 'group_add.png', 'Add to buddy list', 'Add to buddy list', NULL, TRUE);
      $variables['buddylist_text'] = t('Add buddy');
      $variables['buddylist_link'] = "buddy/add/$accountid";
    }
  }

  $variables['buddylist'] = '<div class="' . $variables['buddylist_class'] . '">' .l($variables['buddylist_icon'] . ' '  . $variables['buddylist_text'], $variables['buddylist_link'], NULL, NULL, NULL, NULL, TRUE) .'</div>';
}