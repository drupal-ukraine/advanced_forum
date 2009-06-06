<?php
// $Id$

/**
 * @file
 * Views integration for advanced_forum.
 */

/**
 * Loads the included views
 *
 * This function is used instead of views ability to autodiscover a views
 * export .inc because this allows us to put each view in its own file.
 * Thanks to Moshe and OG for the code.
 */
function advanced_forum_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'advanced_forum'). '/includes/views', '.view');
  foreach ($files as $absolute => $file) {
    require_once $absolute;
    if (isset($view)) {
      $views[$file->name] = $view;
    }
  }
  return $views;
}
