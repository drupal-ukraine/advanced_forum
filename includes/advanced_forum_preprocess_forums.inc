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
    // No forum description on the top level.
    $variables['forum_description'] = '';
    
    // We only handle the search on the top level forum. Per forum searches
    // are handled in the topic list view preprocess.
    $variables['search'] = theme('advanced_forum_search_forum');
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
  }
  
  // Theme the links core provides us. (New topic, etc)
  $variables['forum_links'] = theme('links', $variables['links'], array('class' => 'links forum-links')); 

  $tid = intval($variables['tid']);
  // Add in the mark as read link if user has access
  if (advanced_forum_markasread_access()) {
    if ($tid) {
      $variables['forum_secondary_links_array']['mark-read']['title'] = t('Mark all topics read');
      $variables['forum_secondary_links_array']['mark-read']['href'] = "forum/markasread/$tid";
    }
    else {
      $variables['forum_secondary_links_array']['mark-read']['title'] = t('Mark all forums read');
      $variables['forum_secondary_links_array']['mark-read']['href'] = "forum/markasread";
    }
  }

  // If using Views, add in our special top level views.
  if (module_exists('views')) {
    global $user;
    if ($tid) {
      if ($user->uid) {
        $variables['forum_secondary_links_array']['unread']['title'] = t('My unread');
        $variables['forum_secondary_links_array']['unread']['href'] = "forum/unread";
        $variables['forum_secondary_links_array']['unread']['query'] = array('forum' => $tid);
      }
      $variables['forum_secondary_links_array']['unanswered']['title'] = t('Unanswered topics');
      $variables['forum_secondary_links_array']['unanswered']['href'] = "forum/unanswered";
      $variables['forum_secondary_links_array']['unanswered']['query'] = array('forum' => $tid);
    }
    else {
      if ($user->uid) {
        $variables['forum_secondary_links_array']['unread']['title'] = t('My unread');
        $variables['forum_secondary_links_array']['unread']['href'] = "forum/unread";
      }
      $variables['forum_secondary_links_array']['unanswered']['title'] = t('Unanswered topics');
      $variables['forum_secondary_links_array']['unanswered']['href'] = "forum/unanswered";
      $variables['forum_secondary_links_array']['active']['title'] = t('Active topics');
      $variables['forum_secondary_links_array']['active']['href'] = "forum/active";
    }
  }

  // Render our added links.
  $variables['forum_secondary_links'] = theme('links', $variables['forum_secondary_links_array'], array('class' => 'links forum-secondary-links')); 
}