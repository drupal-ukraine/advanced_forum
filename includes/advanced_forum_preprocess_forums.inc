<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
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
    $variables['search'] = '';
  /*
  // Add extra span tags for image replacement.
  foreach ($variables['links'] AS $key => $link) {
    $variables['links'][$key]['title'] = $variables['links'][$key]['title'] . '<span class="image-replace"></span>';
    $variables['links'][$key]['html'] = TRUE;
  }
  */

    // Format the "post new content" links listing.
    $vid = variable_get('forum_nav_vocabulary', '');
    $vocabulary = taxonomy_vocabulary_load($vid);
    $forum_types = array();

    // Loop through all node types for forum vocabulary.
    foreach ($vocabulary->nodes as $type) {
      // Check if the current user has the 'create' permission for this node type.
      if (node_access('create', $type)) {
        // Fetch the "General" name of the content type.
        $node_type = node_get_types('name', $type);
        
        // Remove the word "Forum" out of "Forum topic" to shorten it.
        // @TODO: this is a little dodgy and may not work right with 
        // translations. Should be replaced if there's a better way.
        $node_type = str_replace('Forum', '', $node_type);
        
        // Push the link with title and url to the array.
        $forum_types[$type] = array(
          'title' => t('New @node_type', array('@node_type' => $node_type)) . '<span class="image-replace"></span>', 
          'href' => 'node/add/'. str_replace('_', '-', $type) .'/'. $variables['tid'], 
          'html' => TRUE);
      }
    }

    if (empty($forum_types)) {
      // The user is logged-in; but denied access to create any new forum content type.
      if ($user->uid) {
        $forum_types['disallowed'] = array('title' => t('You are not allowed to post new content in forum.'));
      }
      // The user is not logged-in; and denied access to create any new forum content type.
      else {
        $forum_types['login'] = array(
          'title' => t('<a href="@login">Login</a> to post new content in forum.', array('@login' => url('user/login', array('query' => drupal_get_destination())))), 
          'html' => TRUE);
      }
    }
    
    $variables['links'] = $forum_types;
    $variables['forum_links'] = theme('links', $variables['links'], array('class' => 'forum-links')); 
  }

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
  $variables['forum_secondary_links'] = theme('links', $variables['forum_secondary_links_array'], array('class' => 'forum-links')); 
}