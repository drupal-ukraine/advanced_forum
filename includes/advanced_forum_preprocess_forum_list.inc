<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_forum_list(&$variables) {
  $collapsible = variable_get('advanced_forum_collapsible_containers', 'toggle');
  if ($collapsible != 'none') {
    drupal_add_js(drupal_get_path('module', 'advanced_forum') . '/advanced_forum.js');
    $settings['advanced_forum'] = array('modulePath' => drupal_get_path('module', 'advanced_forum'), 'effect' => $collapsible);
        drupal_add_js($settings, array('type' => 'setting'));
  }
  advanced_forum_add_template_suggestions("forum_list", $variables);
  // Determine if we want to use taxonomy image here so we don't need to keep
  // checking while in the main loop.
  $variables['use_taxonomy_image'] = (function_exists('taxonomy_image_display') && variable_get('advanced_forum_use_taxonomy_image', TRUE));

  // This will be the ID of the container or forum we are in. If empty,
  // we are on the root forum page.
  $parent_id = $variables['tid'];

  if (empty($parent_id)) {
    $main_forum_page = TRUE;
    $root_forums = FALSE;
    $fake_container = FALSE;
    $containers = FALSE;
    $tids = array();

    foreach ($variables['forums'] as $id => $forum) {
      // If uncontained forums come after an existing container, they'll
      // be put into that container due to the ordering in the template.
      // So don't bother with a fake container if there won't be anything
      // in it.
      if (!empty($forum->container) && !$fake_container) {
        $containers = TRUE;
      }

      if (empty($forum->container) && $forum->parents == array(0 => 0)) {
        $root_forums = TRUE;
        if (!$containers) {
          $fake_container = TRUE;
        }
        $variables['forums'][$id]->depth++;
        $tids[$forum->tid] = $forum->tid;
      }

      if ($root_forums && !empty($tids[$forum->parents[0]])) {
        $variables['forums'][$id]->depth++;
        $tids[$forum->tid] = $forum->tid;
      }
    }

    if ($fake_container) {
      $container = taxonomy_vocabulary_load(variable_get('forum_nav_vocabulary', ''));
      $container->tid = 0;
      $container->container = TRUE;
      $container->num_topics = 0;
      $variables['forums'] = array(0 => $container) + $variables['forums'];
    }
  }
  else {
    $main_forum_page = FALSE;
    // We want to add in the container or use the parent forum as a container.
    $container = taxonomy_term_load($parent_id);
    $container->container = TRUE;
    $container->num_topics = 0;

    // Add in the "container" at the top and bump up the depth of everything
    // underneath. Note that we can't use array_unshift to add the container
    // to the top as that changes all the keys.
    $orginal_forums = $variables['forums'];
    $variables['forums'] = array();
    $variables['forums'][$parent_id] = $container;
    $variables['forums'][$parent_id]->depth = 0;
    foreach ($orginal_forums as $id => $forum) {
      $variables['forums'][$id] = $forum;
      $variables['forums'][$id]->depth++;
    }
  }


  global $user;
  $row = 0;
  // Sanitize each forum so that the template can safely print the data but skip the name of subforums.
  foreach ($variables['forums'] as $id => $forum) {
    $variables['forums'][$id]->description = !empty($forum->description) ? filter_xss_admin($forum->description) : '';
    $variables['forums'][$id]->link = url("forum/$forum->tid");
    $variables['forums'][$id]->name = empty($forum->parents[0]) ? check_plain($forum->name) : $forum->name;
    $variables['forums'][$id]->is_container = !empty($forum->container);
    $variables['forums'][$id]->zebra = $row % 2 == 0 ? 'odd' : 'even';
    $row++;

    $variables['forums'][$id]->new_text = '';
    $variables['forums'][$id]->new_url = '';
    $variables['forums'][$id]->new_topics = 0;
    $variables['forums'][$id]->old_topics = $forum->num_topics;
    if ($user->uid) {
      $variables['forums'][$id]->new_topics = _forum_topics_unread($forum->tid, $user->uid);
      if ($variables['forums'][$id]->new_topics) {
        $variables['forums'][$id]->new_text = format_plural($variables['forums'][$id]->new_topics, '1 new', '@count new');
        $variables['forums'][$id]->new_url = url("forum/$forum->tid", array('fragment' => 'new'));
      }
      $variables['forums'][$id]->old_topics = $forum->num_topics - $variables['forums'][$id]->new_topics;
    }

    $variables['forums'][$id]->last_reply = theme('forum_submitted', array('topic' => isset($forum->last_post) ? $forum->last_post : NULL));

    $variables['forums'][$id]->new_posts = 0;
    $variables['forums'][$id]->new_text_posts = '';
    $variables['forums'][$id]->new_url_posts = '';
    $variables['forums'][$id]->old_posts = (empty($forum->num_posts)) ? '' : $forum->num_posts;

    if ($user->uid) {
      // Show number of new posts as well as topics
      if (variable_get('advanced_forum_get_new_comments', 0)) {
        // This can cause performance issues, so allow it to be turned off
        $variables['forums'][$id]->new_posts = advanced_forum_unread_replies_in_forum($forum->tid, $user->uid) + $variables['forums'][$id]->new_topics;

        if ($variables['forums'][$id]->new_posts) {
          $variables['forums'][$id]->new_text_posts = format_plural($variables['forums'][$id]->new_posts, '1 new', '@count new');
          $variables['forums'][$id]->new_url_posts = url("forum/$forum->tid", array('fragment' => 'new'));
        }

        if (isset($forum->num_posts)) {
          $variables['forums'][$id]->old_posts = $forum->num_posts - $variables['forums'][$id]->new_posts;
        }
      }
    }

    // If there are new topics/posts, change the icon
    if ($forum->new_topics || $forum->new_posts) {
      $variables['forums'][$id]->icon_classes = "forum-icon forum-folder-new-posts";
      $variables['forums'][$id]->icon_text = t("New posts");
    }
    else {
      $variables['forums'][$id]->icon_classes = "forum-icon forum-folder";
      $variables['forums'][$id]->icon_text = t("No new");
    }

    // Add in the taxonomy image, if any
    if ($variables['use_taxonomy_image']) {
      $variables['forums'][$id]->forum_image = taxonomy_image_display($forum->tid);
    }

    if (isset($variables['forums'][$id]->depth) && (intval($variables['forums'][$id]->depth) > 1)) {
      // This is a subforum. Add it to the list for the compact listing.
      $parent_id = $id;
      for ($count = $variables['forums'][$id]->depth; $count > 1; $count--) {
        $parent_id = $variables['forums'][$parent_id]->parents['0'];
      }
      $variables['forums'][$parent_id]->subforum_list[$id] = $variables['forums'][$id];
    }
  }

  foreach ($variables['forums'] as $id => $forum) {
    if (!empty($forum->subforum_list)) {
      $variables['forums'][$id]->subforums = theme('advanced_forum_subforum_list', array('subforum_list' => $forum->subforum_list));
      unset($variables['forums'][$id]->subforum_list);
    }

    if (!empty($forum->parents[0])) {
    // Sanitize the subforums.
      $variables['forums'][$id]->name  = check_plain($forum->name);
    }
  }

  // Give meaning to $tid for themers. $tid actually stands for term id.
  $variables['forum_id'] = isset($variables['tid']) ? $variables['tid'] : 0;
}
