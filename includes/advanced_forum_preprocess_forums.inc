<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_forums(&$variables) {
  $variables['forums_defined'] = count($variables['forums']) || count($variables['parents']);

  advanced_forum_add_template_suggestions("forums", $variables['template_files']);
  $tid = intval($variables['tid']);

  if (!empty($variables['forums'])) {
    $variables['forums'] = theme('forum_list', $variables['forums'], $variables['parents'], $variables['tid']);
  }
  else {
    $variables['forums'] = '';
  }

  $variables['search'] = theme('advanced_forum_search_forum', $variables['tid']);

  if (empty($variables['topics'])) {
    // No forum description on the top level.
    $variables['forum_description'] = '';

  }
  else {
    // Grab the forum description and make it available to the template file
    $forum = taxonomy_get_term($tid);
    $variables['forum_description'] = $forum->description;

    $variables['links'] = advanced_forum_get_forum_post_links($tid);
    $variables['forum_links'] = theme('links', $variables['links'], array('class' => 'forum-links'));

    // Set a variable for displaying the topic legend.
    $variables['topic_legend'] = theme('advanced_forum_topic_legend');
  }

  $variables['forum_secondary_links_array'] = advanced_forum_get_forum_secondary_links($tid);
  // Render our added links.
  $variables['forum_secondary_links'] = theme('links', $variables['forum_secondary_links_array'], array('class' => 'forum-links'));


  // Fetch the forum statistics.
  $variables['forum_statistics'] = '';
  if (user_access('view forum statistics')) {
    $variables['forum_statistics'] = theme('advanced_forum_statistics');
  }

  // Fetch the forum legend.
  $variables['forum_legend'] = theme('advanced_forum_forum_legend');

  // Fetch the current language
  global $language;
  $variables['language_class'] = $language->language;
}
