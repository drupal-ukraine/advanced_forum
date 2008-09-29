<?php
/**
 * @file
 *   This file provides advanced forum process functions for the user_stats module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function user_stats_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;
  if ($account_id != 0) {
    if (user_access('View statistics')) {
      $variables['user_stats_posts'] = user_stats_get_stats('post_count', $account_id);
    }

    $ip = user_stats_get_stats('ip_address', $account_id);
    if (!empty($ip)) {
      // Will be empty if the user doesn't have access.
      $variables['user_stats_ip'] = $ip;
    }
  }
}