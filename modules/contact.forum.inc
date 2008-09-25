<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the contact module.
 */

/**
 * Implementation of hook_preprocess_forum_user().
 */
function contact_preprocess_forum_user(&$variables) {
  global $user;
  $account = $variables['account'];

  if (($account->contact) && ($account->uid != $user->uid) && ($user->uid != 0)) {
    $variables['contact_icon'] = theme('image', advanced_forum_path_to_images() . '/contact.png', t('Email'), t('Email'), NULL, TRUE);
    $variables['contact_link'] = 'user/'. $account->uid . '/contact';
    $variables['contact'] = 
      '<div class="contact">' . 
      l($variables['contact_icon'], $variables['contact_link'], NULL, NULL, NULL, NULL, TRUE) . 
      ' ' .      
      l(t('Email'), $variables['contact_link']) . 
      '</div>';
  }
}
