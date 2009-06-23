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
    // We don't want the links on the top of the forum overview.
    $variables['links_orig'] = $variables['links'];
    $variables['links'] = array();
    
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
  
  // Add in the mark as read link if user has access
  if (advanced_forum_markasread_access()) {
    $tid = $variables['tid'];
    if ($tid) {
      $variables['links']['mark-read']['title'] = t('Mark all topics read');
      $variables['links']['mark-read']['href'] = "forum/markasread/$tid";
    }
    else {
      $variables['links']['mark-read']['title'] = t('Mark all forums read');
      $variables['links']['mark-read']['href'] = "forum/markasread";
   }
  }

  // Render links.
  $variables['links_rendered'] = theme('links', $variables['links'], array('class' => 'links forum-links')); 
}