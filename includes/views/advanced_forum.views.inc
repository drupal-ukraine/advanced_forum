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

/**
 * Use views_data_alter to add items to the node table that are
 * relevant to topic icons.
 */
function advanced_forum_views_data_alter(&$data) {
  // Topic icon
  $data['node']['topic_icon'] = array(
    'title' => t('Topic Icon'),
    'help' => t('Icon that shows new posts, hot, sticky, locked, etc.'),
    'field' => array(
      'handler' => 'advanced_forum_handler_field_node_topic_icon',
    ),
  );
  
  $data['node']['topic_pager'] = array(
    'title' => t('Topic Pager'),
    'help' => t('Small pager for individual topics.'),
    'field' => array(
      'handler' => 'advanced_forum_handler_field_node_topic_pager',
    ),
  );
}

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function advanced_forum_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'advanced_forum') . '/includes/views',
    ),
    'handlers' => array(
      'advanced_forum_handler_field_node_topic_icon' => array(
        'parent' => 'views_handler_field',
      ),
      'advanced_forum_handler_field_node_topic_pager' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

