<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the private message module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function privatemsg_preprocess_author_pane(&$variables) {
  global $user;
  $account = $variables['account'];
  $account_id = $variables['account']->uid;  
  $variables['privatemsg'] = '';
  $can_access = user_access('access private messages');
  $author_allows = (isset($account->privatemsg_allow)) ? $account->privatemsg_allow : 1;
  
  // Send private message
  if ($can_access && $account_id != $user->uid && $author_allows) {
    $variables['privatemsg'] = 
      l(theme('image', advanced_forum_path_to_images() . '/private-message.png', 
              t('Send private message'), t('Send private message'), NULL, TRUE), 
        "privatemsg/new/$account_id",
        array(), NULL, NULL, FALSE, TRUE); 
       
    $variables['privatemsg_link'] = l(t('Send PM'), 'privatemsg/new/'. $account_id);
   }
}
