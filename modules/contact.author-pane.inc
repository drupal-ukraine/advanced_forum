<?php
/**
 * @file
 *   This file provides a preprocess function for the author pane used by  
 *   Advanced Forum and Advanced Profile Kit.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function contact_preprocess_author_pane(&$variables) {
  global $user;
  $account = $variables['account'];
  $account_id = $account->uid;
  $image_path = $variables['image_path'];
  
  if (($account->contact) && ($account_id != $user->uid) && ($user->uid != 0)) {
    $variables['contact'] =   
      l(theme('image', "$image_path/contact.png", t('Contact user'), t('Contact user'), NULL, TRUE), 
        "user/$account_id/contact", array(), NULL, NULL, FALSE, TRUE); 
    
    $variables['contact_link'] = l("Contact user", "user/$account_id/contact");
  }
}
