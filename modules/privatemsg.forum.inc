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
    $variables['privatemsg'] = 
      l(theme('image', advanced_forum_path_to_images() . '/private-message.png', 
              t('Send private message'), t('Send private message'), NULL, TRUE), 
        "privatemsg/new/$account_id",
        array(), NULL, NULL, FALSE, TRUE); 
       
    $variables['privatemsg_link'] = l(t('Send PM'), 'privatemsg/new/'. $account_id);
   }
}
