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
  
  if ($account_id != 0) {
    $variables['userpoints_points'] = userpoints_get_current_points($account_id);

    $categories = userpoints_get_categories();
    foreach ($categories AS $tid => $category) {
      $variables['userpoints_categories'][$category] =  userpoints_get_current_points($account_id, $tid);
    }
  }
}
  
  