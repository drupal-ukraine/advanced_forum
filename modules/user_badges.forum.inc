<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the user badges module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function user_badges_preprocess_forum_user(&$variables) {
  $account_id = $variables['account']->uid;
  if ($account_id == 0) {
    return;
  }
  
  $variables['user_badges_raw'] = user_badges_for_uid($account_id);
  $variables['user_badges'] = '<div class="user-badges">' . $variables['user_badges_raw'] . '</div>';
}