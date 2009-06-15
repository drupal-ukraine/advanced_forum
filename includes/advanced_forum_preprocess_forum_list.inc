<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to easy memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_forum_list(&$variables) {

  $variables['template_files'][] = 'advanced_forum-forum-list';

  global $user;

  $number_of_forums = count($variables['forums']);
  $forum_counter = 0;
  foreach ($variables['forums'] as $id => $forum) {
    // Counter to label the rows by position
    $forum_counter++;
    switch ($forum_counter) {
      case "1":
        $row_classes = 'first-row';
        break;
      case $number_of_forums:
        $row_classes = 'last-row';
        break;
      default:
        $row_classes = 'middle-row';
    }

    if ($forum->is_container) {
      $row_classes .= ' container';
    }

    $variables['forums'][$id]->row_classes = $row_classes;

    $variables['forums'][$id]->new_posts = 0;
    $variables['forums'][$id]->new_text_posts = '';
    $variables['forums'][$id]->new_url_posts = '';
    $variables['forums'][$id]->old_posts = $forum->num_posts;

    $current_container = $current_container_depth = 0;
    if ($forum->is_container) {
      $current_container = $forum->tid;
      $current_container_depth = $forum->depth;
      $variables['forums'][$id]->container_id = $current_container;
    }
    else {
      if ($forum->depth > $current_container_depth ) {
        $variables['forums'][$id]->container_id = $current_container;
      }
      else {
        $variables['forums'][$id]->container_id = 0;
      }
    }

    if ($user->uid) {
      // Show number of new posts as well as topics
      if (variable_get('advanced_forum_get_new_comments', 0)) {
        // This can cause performance issues, so allow it to be turned off
        $variables['forums'][$id]->new_posts = advanced_forum_unread_replies_in_forum($forum->tid, $user->uid) + $variables['forums'][$id]->new_topics;

        if ($variables['forums'][$id]->new_posts) {
          $variables['forums'][$id]->new_text_posts = format_plural($variables['forums'][$id]->new_posts, '1 new', '@count new');
          $variables['forums'][$id]->new_url_posts = url("forum/$forum->tid", array('fragment' => 'new'));
        }

        $variables['forums'][$id]->old_posts = $forum->num_posts - $variables['forums'][$id]->new_posts;
      }
    }

    // If there are new topics/posts, change the icon
    if ($forum->new_topics || $forum->new_posts) {
      $variables['forums'][$id]->icon = advanced_forum_theme_image(t('forum-folder-new-posts.png'), t('new post folder'));
    }
    else {
      $variables['forums'][$id]->icon = advanced_forum_theme_image(t('forum-folder.png'), t('folder'));
    }

    // Make container description more accessable
    if (!empty($variables['parents']['0'])) {
      $variables['container_description'] = $variables['parents']['0']->description;
    }
  }

  if (user_access('view forum statistics')) {
    $variables['forum_statistics'] = theme('advanced_forum_statistics');
  }

  $variables['forum_legend'] = theme('advanced_forum_forum_legend');
}