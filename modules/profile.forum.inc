<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the profile module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function profile_preprocess_forum_user(&$variables) {
  // We don't want to do this for anonymous users.
  if ($variables['account']->uid == 0) {
    return;
  }
  
  $variables['profile'] = profile_view_profile($variables['account']);
}