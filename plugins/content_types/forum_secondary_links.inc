<?php
// $Id$

/**
 * Callback function to supply a list of content types.
 */
function advanced_forum_forum_secondary_links_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Forum secondary links'),
    'icon' => 'icon_forum.png',
    'description' => t('The secondary links for a forum.'),
    'required context' => new ctools_context_required(t('Forum'), 'forum'),
    'category' => t('Forum'),
    'defaults' => array(),
  );
}

/**
 * Render the content.
 */
function advanced_forum_forum_secondary_links_content_type_render($subtype, $conf, $panel_args, $context) {
  if (!empty($context) && empty($context->data)) {
    return;
  }

  if (empty($context)) {
    $tid = 0;
  }
  else {
    $tid = $context->data->tid;
  }

  $block = new stdClass();
  $block->module = 'forum-secondary-links';
  $block->delta  = $tid;

  $block->title = ''; // by default this has no title.
  _advanced_forum_add_files();

  $block->secondary_links = advanced_forum_get_forum_secondary_links($tid);
  $block->content = '<div class="forum-secondary-links">' . theme('links', $block->secondary_links, array('class' => 'forum-links')) . '</div>';

  return $block;
}

/**
 * Returns an edit form for the custom type.
 */
function advanced_forum_forum_secondary_links_content_type_edit_form(&$form, &$form_state) {
}

function advanced_forum_forum_secondary_links_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function advanced_forum_forum_secondary_links_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" secondary links', array('@s' => $context->identifier));
}

