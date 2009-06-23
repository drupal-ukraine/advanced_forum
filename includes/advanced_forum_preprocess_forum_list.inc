<?php
// $Id$

/**
 * @file
 * Holds the contents of a preprocess function moved into its own file
 * to ease memory requirements and having too much code in one file.
 */

function _advanced_forum_preprocess_forum_list(&$variables) {
  $variables['template_files'][] = 'advanced_forum-forum-list';

  // This will be the ID of the container or forum we are in. If empty,
  // we are on the root forum page.
  $parent_id = arg(1);

  if (!empty($parent_id)) {
    // We want to add in the container or use the parent forum as a container.
    $container = taxonomy_get_term($parent_id);
    $container->container = TRUE;
    $container->num_topics = 0;
 
    // Add in the "container" at the top and bump up the depth of everything 
    // underneath. Note that we can't use array_unshift to add the container
    // to the top as that changes all the keys.
    $orginal_forums = $variables['forums'];
    $variables['forums'] = array();
    $variables['forums'][$parent_id] = $container;
    foreach ($orginal_forums as $id => $forum) {
      $variables['forums'][$id] = $forum;
      $variables['forums'][$id]->depth++;
    }
  }
  
  global $user;
  $row = 0;
  // Sanitize each forum so that the template can safely print the data.
  foreach ($variables['forums'] as $id => $forum) {
    $variables['forums'][$id]->description = !empty($forum->description) ? filter_xss_admin($forum->description) : '';
    $variables['forums'][$id]->link = url("forum/$forum->tid");
    $variables['forums'][$id]->name = check_plain($forum->name);
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
    
    $variables['forums'][$id]->last_reply = theme('forum_submitted', $forum->last_post);
  
    $variables['forums'][$id]->new_posts = 0;
    $variables['forums'][$id]->new_text_posts = '';
    $variables['forums'][$id]->new_url_posts = '';
    $variables['forums'][$id]->old_posts = $forum->num_posts;

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
      $variables['forums'][$id]->icon_classes = "forum-icon forum-folder-new-posts";
      $variables['forums'][$id]->icon_text = t("New posts");
    }
    else {
      $variables['forums'][$id]->icon_classes = "forum-icon forum-folder";
      $variables['forums'][$id]->icon_text = t("No new");
    }
    
    if ($variables['forums'][$id]->depth > 1) {
      // This is a subforum. Add it to the list for the compact listing.
      $parent_id = $variables['forums'][$id]->parents['0'];
      $variables['forums'][$parent_id]->subforum_list[$id] = $variables['forums'][$id];
    }     
  }

  foreach ($variables['forums'] as $id => $forum) {
     if (!empty($forum->subforum_list)) {
       $variables['forums'][$id]->subforums = theme('advanced_forum_subforum_list', $forum->subforum_list);
       unset($variables['forums'][$id]->subforum_list);
     }
   }

  
  // Give meaning to $tid for themers. $tid actually stands for term id.
  $variables['forum_id'] = $variables['tid'];
  unset($variables['tid']);

  $variables['forum_statistics'] = '';
  if (empty($parent_id) && user_access('view forum statistics')) {
    $variables['forum_statistics'] = theme('advanced_forum_statistics');
  }

  $variables['forum_legend'] = theme('advanced_forum_forum_legend');
}