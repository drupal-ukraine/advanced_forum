<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the subscriptions module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function subscriptions_preprocess_author_pane(&$variables) {
  // We don't want to do this for anonymous users.
  if ($variables['account']->uid == 0) {
    return;
  }
  
  if (user_access('subscribe to content types')) {
    // BUG: D6 - this assumes forum type.
    $variables['subscribe_link'] = "subscriptions/add/type/forum/" . $variables['account']->uid;
    $variables['subscribe'] = l(t("Subscribe"), $variables['subscribe_link'], array('title' => 'Subscribe to forum posts by this user'));
  }
}