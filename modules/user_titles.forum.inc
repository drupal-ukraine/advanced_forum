<?php
/**
 * @file
 *   This file provides advanced forum process functions for the user_titles module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function user_titles_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;
  if ($account_id != 0) {
    $variables['user_title'] = user_titles_get_user_title($account_id);
  }  
}
