<?php
/**
 * @file
 *   This file provides advanced forum process functions for the user_stats module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function user_stats_preprocess_forum_user(&$variables) {
  $account_id = $variables['account']->uid;
  if ($account_id == 0) {
    // Stats not available for anonymous users.
    return;
  }
 
  if (user_access('View statistics')) {
    $variables['user_stats_posts_raw'] = user_stats_get_stats('post_count', $account_id);
    $variables['user_stats_posts'] = '<div class="user-posts"><span class="user-info-label">' . t('Posts: ') . '</span>' . $variables['user_stats_posts_raw'] . '</div>';
  }

  // IP is only visible if the viewer has access, so do an extra check
  $ip = user_stats_get_stats('ip_address', $account_id);
  if (!empty($ip)) {
    $variables['user_stats_ip_raw'] = $ip;
    $variables['user_stats_ip'] = '<div class="user-ip"><span class="user-info-label">' . t('IP: ') . '</span>' . $variables['user_stats_ip_raw'] . '</div>';
  }
}