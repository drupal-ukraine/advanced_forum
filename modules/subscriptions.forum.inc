<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the subscriptions module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function subscriptions_preprocess_forum_user(&$variables) {
  if (user_access('subscribe to content types')) {
    // BUG: D6 - this assumes forum type.
    $variables['subscribe_link'] = "subscriptions/add/type/forum/" . $variables['accountid'];
    $variables['subscribe'] = l(t("Subscribe"), $variables['subscribe_link'], array('title' => 'Subscribe to forum posts by this user'));
  }
}