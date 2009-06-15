<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to easy memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_forums(&$variables) {
  $variables['template_files'][] = 'advanced_forum-forums';

  if (empty($variables['topics'])) {
    // We don't want the links on the top of the forum overview
    $variables['links_orig'] = $variables['links'];
    $variables['links'] = array();
    $variables['forum_description'] = '';
  }
  else {
    if (module_exists('views')) {
      // When views is installed, we let it take over the topic list page.
      $view = views_get_view('advanced_forum_topic_list');
      $view->set_display('default');
      $view->set_arguments(array($variables['tid']));
      $topic_list_view = $view->preview();

      $variables['topics'] = $topic_list_view;
    }

    // Grab the forum description and make it available to the template file
    $forum = taxonomy_get_term($variables['tid']);
    $variables['forum_description'] = $forum->description;

    // To avoid translation issues, make the button version optional
    if (variable_get('advanced_forum_button_links', 1)) {
      if (isset($variables['links']['forum'])) {
        $title_image = advanced_forum_theme_image(t('new-topic.png'), t('New topic'));
        $variables['links']['forum']['title'] = $title_image;
        $variables['links']['forum']['html'] = TRUE;
      }

      if (isset($variables['links']['poll'])) {
        $title_image = advanced_forum_theme_image(t('new-poll.png'), t('New poll'));
        $variables['links']['poll']['title'] = $title_image;
        $variables['links']['poll']['html'] = TRUE;
      }
    }
  }

  // Add in the mark as read link if user has access
  if (advanced_forum_markasread_access()) {
    $tid = $variables['tid'];
    if ($tid) {
      $title = t('Mark all topics read');
      $link = "forum/markasread/$tid";
    }
    else {
      $title = t('Mark all forums read');
      $link = "forum/markasread";
    }

    // To avoid translation issues, make the button version optional
    if (variable_get('advanced_forum_button_links', 1)) {
      $title_image = advanced_forum_theme_image(t('mark-read.png'), t('Mark read'));
      $variables['links']['markasread'] = array('title' => $title_image, 'href' => $link, 'html' => TRUE);
      $variables['links_orig']['markasread'] = array('title' => $title_image, 'href' => $link, 'html' => TRUE);
    }
    else {
      $variables['links']['markasread'] = array('title' => $title, 'href' => $link);
      $variables['links_orig']['markasread'] = array('title' => $title, 'href' => $link);
    }
  }

  $variables['search'] = theme('advanced_forum_search_forum', $variables['tid']);
}