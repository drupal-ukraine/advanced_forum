<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the user badges module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function user_badges_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;
  
  if ($account_id != 0) {
    $variables['user_badges'] = user_badges_for_uid($account_id);
  } 
}