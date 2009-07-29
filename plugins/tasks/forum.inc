<?php
// $Id$

/**
 * Specialized implementation of hook_page_manager_task_tasks(). See api-task.html for
 * more information.
 */
function advanced_forum_forum_page_manager_tasks() {
  return array(
    // This is a 'page' task and will fall under the page admin UI
    'task type' => 'page',
    'title' => t('Forum page'),
    'admin title' => t('Forum page'),
    'admin description' => t('When enabled, this overrides the default Drupal behavior for displaying forum listings at <em>forum</em> and <em>forum/%forum</em>. If you add variants, you may use selection criteria such as roles or user access to provide different views of the forums. If no variant is selected, the normal advanced forum view will be selected.'),
    'admin path' => 'forum/%forum',

    'hook menu' => 'advanced_forum_forum_menu',
    'hook menu alter' => 'advanced_forum_forum_menu_alter',

    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context', // handler type -- misnamed
    'get arguments' => 'advanced_forum_forum_get_arguments',
    'get context placeholders' => 'advanced_forum_forum_get_contexts',

    // Allow this to be enabled or disabled:
    'disabled' => variable_get('advanced_forum_forum_disabled', TRUE),
    'enable callback' => 'advanced_forum_forum_enable',
  );
}

/**
 * Callback defined by advanced_forum_forum_page_manager_tasks().
 *
 * Alter the user view input so that user view comes to us rather than the
 * normal user view process.
 */
function advanced_forum_forum_menu_alter(&$items, $task) {
  if (variable_get('advanced_forum_forum_disabled', TRUE)) {
    return;
  }

  $items['forum']['page callback'] = 'advanced_forum_forum_page';
  $items['forum']['file path'] = $task['path'];
  $items['forum']['file'] = $task['file'];
}

/**
 * Entry point for our overridden user view.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to Drupal core's
 * user view, which is user_page_view().
 */
function advanced_forum_forum_page($tid = 0) {
  // Load my task plugin:
  $task = page_manager_get_task('forum');

  // Load the account into a context.
  ctools_include('context');
  ctools_include('context-task-handler');
  $contexts = ctools_context_handler_get_task_contexts($task, '', array($tid));

  $output = ctools_context_handler_render($task, '', $contexts, array($tid));
  if ($output === FALSE) {
    // Fall back!
    $output = advanced_forum_page($tid);
  }

  return $output;
}

/**
 * Callback to get arguments provided by this task handler.
 *
 * Since this is the node view and there is no UI on the arguments, we
 * create dummy arguments that contain the needed data.
 */
function advanced_forum_forum_get_arguments($task, $subtask_id) {
  return array(
    array(
      'keyword' => 'forum',
      'identifier' => t('Forum'),
      'id' => 1,
      'name' => 'forum_id',
      'settings' => array('breadcrumb' => TRUE),
    ),
  );
}

/**
 * Callback to get context placeholders provided by this handler.
 */
function advanced_forum_forum_get_contexts($task, $subtask_id) {
  return ctools_context_get_placeholders_from_argument(advanced_forum_forum_get_arguments($task, $subtask_id));
}

/**
 * Callback to enable/disable the page from the UI.
 */
function advanced_forum_forum_enable($cache, $status) {
  variable_set('advanced_forum_forum_disabled', $status);
}
