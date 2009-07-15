<?php
// $Id$

/**
 * @file
 * Advanced forum settings / configuration page.
 */

/**
 * Defines the Advanced Forum settings form.
 */
function advanced_forum_settings_page() {
  /* General settings */
  $form['advanced_forum_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Choose style
  $options = array();
  $available_styles = advanced_forum_get_all_styles();
  foreach ($available_styles as $style_machine_name => $style) {
    $options[$style_machine_name] = $style['name'];
  }
  asort($options);

  $form['advanced_forum_general']['advanced_forum_style'] = array(
    '#type' => 'select',
    '#title' => t('Advanced forum style'),
    '#options' => $options,
    '#description' => t('Choose which style to use for your forums. This will apply independent of site theme.'),
    '#default_value' => variable_get('advanced_forum_style', 'blue_lagoon_stacked'),
  );

  // Theme all site comments as forums
  $form['advanced_forum_general']['advanced_forum_theme_all_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Treat all site comments like forum posts'),
    '#default_value' => variable_get('advanced_forum_theme_all_comments', 0),
    '#description' => t('Choosing yes causes advanced forum to consider every comment a forum comment and attempt to theme it that way. Some changes to your theme may be required.'),
  );

  /* Forum / topic list settings */
  $form['advanced_forum_lists'] = array(
    '#type' => 'fieldset',
    '#title' => t('Forum and topic lists'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (!module_exists('views')) {
    // Hide 'created' column
    $form['advanced_forum_lists']['advanced_forum_hide_created'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide the created column on the topic list'),
      '#default_value' => variable_get('advanced_forum_hide_created', 0),
      '#description' => t('This allows you to hide the created column on the topic list. Useful if you want to move the created by information to underneath the topic title.'),
    );

    // Pager max
    $form['advanced_forum_lists']['advanced_forum_topic_pager_max'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum number of pages to show on per title pager'),
      '#size' => 5,
      '#description' => t('Used on the pager under topic titles in topic list. e.g. entering 5 will get you 1,2,3,4 ... 10'),
      '#default_value' => variable_get('advanced_forum_topic_pager_max', 5),
    );

    // Time ago cutoff
    $form['advanced_forum_lists']['advanced_forum_time_ago_cutoff'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of hours before switching to date posted in displays'),
      '#size' => 5,
      '#description' => t('Will show "time ago" until this cutoff and then switch to actual date posted.'),
      '#default_value' => variable_get('advanced_forum_time_ago_cutoff', 48),
    );
  }

  // Retrieve new comments on forum listing
  $form['advanced_forum_lists']['advanced_forum_get_new_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Get the number of new comments per forum on the forum list'),
    '#default_value' => variable_get('advanced_forum_get_new_comments', 0),
    '#description' => t('Core forum shows the number of new topics. If checked, Advanced Forum will get the number of new comments as well and show it under "posts" on the forum overview. Slow query not recommended on large forums.'),
  );

  // Title length max
  $form['advanced_forum_lists']['advanced_forum_topic_title_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of characters to display for the topic title'),
    '#size' => 5,
    '#description' => t('Used on main forum page. Enter 0 to use the full title.'),
    '#default_value' => variable_get('advanced_forum_topic_title_length', 15),
  );

  /* Topic settings */
  $form['advanced_forum_topics'] = array(
    '#type' => 'fieldset',
    '#title' => t('Topics'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Use topic navigation
  $form['advanced_forum_topics']['advanced_forum_use_topic_navigation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use topic navigation'),
    '#default_value' => variable_get('advanced_forum_use_topic_navigation', 1),
    '#description' => t('The query to get the previous / next topics can cause performance problems. Unchecking this will prevent the query from running.'),
  );

  if (module_exists('imagecache')) {
    $options = array('' => '');
    $presets = imagecache_presets();
    foreach ($presets AS $preset) {
      $options[$preset['presetname']] = $preset['presetname'];
    }

    $form['advanced_forum_topics']['advanced_forum_user_picture_preset'] = array(
      '#type' => 'select',
      '#title' => t('User picture preset'),
      '#options' => $options,
      '#description' => t('Imagecache preset to use for forum avatars. Leave blank to not use this feature.'),
      '#default_value' => variable_get('advanced_forum_user_picture_preset', ''),
    );
  }

  // Send our form to Drupal to make a settings page
  return system_settings_form($form);
}
