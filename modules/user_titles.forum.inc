<?php
/**
 * @file
 *   This file provides advanced forum process functions for the user_titles module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function user_titles_preprocess_forum_user(&$variables) {
  $account_id = $variables['account']->uid;
  if ($account_id != 0) {
    $variables['user_title'] = user_titles_get_user_title($account_id);
  }  
}
