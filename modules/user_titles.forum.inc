<?php
/**
 * @file
 *   This file provides advanced forum process functions for the user_titles module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function user_titles_preprocess_forum_user(&$variables) {
  // We don't want to do this for anonymous users.
  if ($variables['account']->uid == 0) {
    return;
  }
  
  $variables['user_title_raw'] = user_titles_get_user_title($variables['account']->uid);
  $variables['user_title'] = '<div class="user-title">' . $variables['user_title_raw'] . '</div>';
}
