<?php
/**
 * @file
 *   This file provides a preprocess function for the author pane used by  
 *   Advanced Forum and Advanced Profile Kit.
 */


/**
 * Implementation of hook_preprocess_author_pane().
 */
function profile_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;
  if ($account_id != 0) {
    $variables['profile'] = profile_view_profile($variables['account']);
  }
}  