<?php
/**
 * @file
 *   This file provides advanced forum process functions for the user_stats module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function user_stats_preprocess_forum_user(&$variables) {
  $variables['posts_raw'] = user_stats_get_stats('post_count', $variables['accountid']);
  $variables['posts'] = '<div class="user-posts"><strong>' . t('Posts: ') . '</strong>' . $variables['posts_raw'] . '</div>';

  // IP is only visible if the viewer has access, so do an extra check
  $ip = user_stats_get_stats('ip_address', $variables['accountid']);
  if (!empty($ip)) {
    $variables['ip_raw'] = $ip;
    $variables['ip'] = '<div class="user-ip"><strong>' . t('IP: ') . '</strong>' . $variables['ip_raw'] . '</div>';
  }
}