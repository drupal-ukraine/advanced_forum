<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the contact module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function contact_preprocess_author_pane(&$variables) {
  global $user;
  $account = $variables['account'];
  $account_id = $account->uid;
  
  if (($account->contact) && ($account_id != $user->uid) && ($user->uid != 0)) {
    $variables['contact'] =   
      l(theme('image', advanced_forum_path_to_images() . '/contact.png', 
              t('Contact user'), t('Contact user'), NULL, TRUE), 
        "user/$account_id/contact",
        array(), NULL, NULL, FALSE, TRUE); 
    
    $variables['contact_link'] = l("Contact user", "user/$account_id/contact");
  }
}
