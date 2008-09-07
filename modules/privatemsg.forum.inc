<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the private message module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function privatemsg_preprocess_forum_user(&$variables) {
  global $user;
  $account_id = $variables['account']->uid;
  
  // Send private message
  if ($account_id != 0 && $account_id != $user->uid && user_access('write privatemsg')) {
    $variables['privatemsg_icon'] = theme('image', advanced_forum_path_to_images() . '/privatemsg.png', t('Private Message'), t('Private Message'), NULL, TRUE);
    $variables['privatemsg_link'] = 'messages/new/'. $account_id;
    $variables['privatemsg'] = 
      '<div class="privatemsg">' .
      l($variables['privatemsg_icon'], $variables['privatemsg_link'], NULL, NULL, NULL, NULL, TRUE) . 
      ' '  . 
      l(t('Send PM'), $variables['privatemsg_link'], array('absolute' => TRUE)) . 
      '</div>';
  }
}
