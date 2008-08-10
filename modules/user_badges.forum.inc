<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the user badges module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function user_badges_preprocess_forum_user(&$variables) {
  // We don't want to do this for anonymous users.
  if ($variables['account']->uid == 0) {
    return;
  }
  
  $variables['user_badges_raw'] = user_badges_for_uid($variables['account']->uid);
  $variables['user_badges'] = '<div class="user-badges">' . $variables['user_badges_raw'] . '</div>';
}