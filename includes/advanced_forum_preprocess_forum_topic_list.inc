<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_forum_topic_list(&$variables) {
    // Redo the table header so we can make our adjustments. This relies on this
    // header being unaltered from the forum module or it will be messed up. I
    // decided to do it this way rather than making it better able to adapt for
    // perfomance reasons.
    global $forum_topic_list_header;

    $new_header = array();
    $forum_topic_list_header[0]['class'] = 'topic-icon';
    $new_header[] = $forum_topic_list_header[0];

    $forum_topic_list_header[1]['class'] = 'topic-topic';
    $new_header[] = $forum_topic_list_header[1];

    $forum_topic_list_header[2]['class'] = 'topic-replies';
    $new_header[] = $forum_topic_list_header[2];

    // Topic views require the statistics module so don't show if it's not enabled
    if (module_exists('statistics')) {
      $new_header[] = array('data' => t('Views'), 'class' => 'topic-views');
    }

    // Allow admins to turn off the created column
    if (!variable_get('advanced_forum_hide_created', 0)) {
      $forum_topic_list_header[3]['class'] = 'topic-created';
      $new_header[] = $forum_topic_list_header[3];
    }

    $forum_topic_list_header[4]['class'] = 'topic-lreply';
    $new_header[] = $forum_topic_list_header[4];

    $forum_topic_list_header =  $new_header;

    // Create the tablesorting header.
    $ts = tablesort_init($forum_topic_list_header);
    $header = '';
    foreach ($forum_topic_list_header as $cell) {
      $cell = tablesort_header($cell, $forum_topic_list_header, $ts);
      $header .= _theme_table_cell($cell, TRUE);
    }
    $variables['header'] = $header;

    // Grab the forum description and make it available to the template file
    $forum = taxonomy_get_term($variables['topic_id']);
    $variables['forum_description'] = $forum->description;

    // Do our own topic processing.
    if (!empty($variables['topics'])) {
      $row = 0;
      $was_sticky = FALSE;

      // Find out how many pages to show on the topic pager. We do this outside
      // the loop because it will be the same for all topics.
      $max_pages_to_display = variable_get('advanced_forum_topic_pager_max', 5);

      foreach ($variables['topics'] as $id => $topic) {
        // Get a pager to add to the topic, if there is one
        $variables['topics'][$id]->pager = theme('advanced_forum_topic_pager', $max_pages_to_display, $topic);

        // Make shadow copy point to actual thread and tell you new the forum name
        if ($variables['topics'][$id]->moved == TRUE) {
          $term = taxonomy_get_term($topic->forum_tid);
          $variables['topics'][$id]->message = l(t('This topic has been moved to !forum', array('!forum' => $term->name)), "node/$topic->nid");
        }

        // Send the NID into the icon theme code so it can be linked to the topic
        $variables['topics'][$id]->icon = theme('forum_icon', $topic->new, $topic->num_comments, $topic->comment_mode, $topic->sticky, $topic->nid);

        // Add in the number of views
        if (module_exists('statistics')) {
          $variables['topics'][$id]->views =  _advanced_forum_get_topic_views_count($topic->nid);
        }

        // Set classes based on stickiness. This allows themers to seperate out
        // all the sticky posts into their own section.
        if ($topic->sticky) {
          $sticky_class = 'sticky-topic';
          $was_sticky = TRUE;
        }
        elseif ($was_sticky) {
          $sticky_class = 'first-not-sticky not-sticky';
          $was_sticky = FALSE;
        }
        else {
          $sticky_class  = 'not-sticky';
        }

        $topic->sticky_class = $sticky_class;

      }
  }

  // Set a variable for displaying the topic legend.
  $variables['topic_legend'] = theme('advanced_forum_topic_legend');

}