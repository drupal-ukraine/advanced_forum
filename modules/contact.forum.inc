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
    $variables['contact_class'] = "contact";
    $variables['contact_icon'] = theme('image', advanced_forum_get_icon_directory() . 'email.png', 'Email', 'Email', NULL, TRUE);
    $variables['contact_text'] = t('Email');
    $variables['contact_link'] = 'user/'. $account->uid . '/contact';
    $variables['contact'] = '<div class="contact">' . l($variables['contact_icon'] . ' '  . $variables['contact_text'], $variables['contact_link'], NULL, NULL, NULL, NULL, TRUE) . '</div>';
  }
}
