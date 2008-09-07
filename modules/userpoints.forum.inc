<?php
/**
 * @file
 *   This file provides advanced forum process functions for the userpoints module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function userpoints_preprocess_forum_user(&$variables) {
  $account_id = $variables['account']->uid;
  if ($account_id == 0) {
    // Stats not available for anonymous users.
    return;
  }
  
  $variables['userpoints_points_raw'] = userpoints_get_current_points($account_id);
  $variables['userpoints_points'] = '<div class="user-points"><span class="user-info-label">' . t('Points: ') . '<span>' . $variables['userpoints_points_raw'] . '</div>';
}