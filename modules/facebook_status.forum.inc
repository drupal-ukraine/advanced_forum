<?php
/**
 * @file
 *   This file provides advanced forum preprocess functions for the facebook status module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function facebook_status_preprocess_author_pane(&$variables) {
  // We don't want to do this for anonymous users.
  if ($variables['account']->uid == 0) {
    return;
  }
  
  $variables['facebook_status'] = facebook_status_advf($variables['account']);
}