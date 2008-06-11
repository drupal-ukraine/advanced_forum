<?php
/**
 * @file
 *   This file provides advanced forum process functions for the userpoints module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function userpoints_preprocess_forum_user(&$variables) {
  $variables['points_raw'] = userpoints_get_current_points($variables['accountid']);
  $variables['points'] = '<div class="user-points"><strong>' . t('Points: ') . '</strong>' . $variables['points_raw'] . '</div>';
}