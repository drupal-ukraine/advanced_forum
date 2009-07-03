<?php
// $Id$

/**
 * @file
 * Holds theme functions and template preprocesses.
 * Other style related functions are in style.inc
 */

// THEME FUNCTIONS **********************************************************/
/**
 * Theme function to format the reply link at the top/bottom of topic.
 */
function theme_advanced_forum_reply_link($node) {
  // Get an object that tells us the CSS class as well as the link/text.
  // Class will be one of: reply-allowed, reply-forbidden, reply-locked.
  // Link will be link to post reply or message.
  $reply_link = advanced_forum_get_reply_link($node);
  
  return '<div class="reply-link ' . $reply_link->class . ' ">' . $reply_link->link . '<span class="image-replace"></span>' . '</div>';
}

/**
 * Theme function to return imagecached version of the user picture.
 */
function theme_advanced_forum_user_picture($account) {
  // Get the imagecache preset to use, if any.
  $preset = variable_get('advanced_forum_user_picture_preset', '');

  // Only proceed if user pictures are enabled, and there is a preset set, and
  // imagecache is enabled. For performace reasons, we return nothing if these
  // critera aren't met because we only call this function when the non
  // imagecached version is already created. If you use this function elsewhere
  // you will need to provide a picture when imagecache is not used.
  if (variable_get('user_pictures', 0) && !empty($preset) && module_exists('imagecache')) {
    if ($account->picture && file_exists($account->picture)) {
      // User has picture, so use that
      $alt = t("@user's picture", array('@user' => $account->name ? $account->name : 'Visitor'));
      $picture = theme('imagecache', $preset, $account->picture);
    }
    else {
      // User doesn't have a picture so use the default, if any
      $default_picture = variable_get('user_picture_default', '');
      if ($default_picture) {
        $picture = theme('imagecache', $preset, $default_picture);
      }
    }

    if (!empty($picture)) {
      // If the user has access to view profiles, link the picture
      if (!empty($account->uid) && user_access('access user profiles')) {
        $picture = l($picture, "user/$account->uid", array('title' => t('View user profile.'), 'html' => TRUE));
      }
    }

    return '<div class="picture">' . $picture . '</div>';
  }
}

/**
 * Theme function to return assembled pager for use on each topic in list.
 *
 * @param $pagecount
 *   Number of pages to include on the pager.
 * @param $topic
 *   Topic object to create a pager for.
 * @return
 *   Assembled pager.
 */
function theme_advanced_forum_topic_pager($pagecount, $topic) {
  // Get the information to assemble the pager. This returns an object with
  // the following properties:
  //  initial_pages: Array of linked numbers for first set of pages
  //  last_page_text: Linked text "Last page" (translatable)
  //  last_page_number: Linked number pointing to the last page.
  $topic_pager = advanced_forum_create_topic_pager($pagecount, $topic);

  if (!empty($topic_pager->initial_pages)) {
    if (!empty($topic_pager->last_page_text)) {
      // If you prefer to end with the number, replace 
      // $topic_pager->last_page_text with $topic_pager->last_page_number
      $last_page = ' &hellip; ' . $topic_pager->last_page_text;
    }
    
    return '<span class="topic-pager">(Page: ' . implode(", ", $topic_pager->initial_pages) . $last_page . ')</span>';
  }
}

/**
 * Theme function to create shadow topic reference.
 *
 * @param $title
 *   Unlinked title of the topic.
 * @param $nid
 *   Node id of the topic.
 * @param $new_forum
 *   Name of the forum that now contains the topic.
 * @return
 *   String to print in place of the title/author info on topic lists.
 */
function theme_advanced_forum_shadow_topic($title, $nid, $new_forum) {
  return $title . '<br />' . 
    t('This topic has been moved to "!forum"', array('!forum' => $new_forum)) . 
    ' (' . l(t('View topic'), "node/$nid") . ')';
}

/**
 * Theme function to a formatted list of subforums.
 *
 * @param $subforum_list
 *   Array of subforums.
 * @return
 *   Formatted list of subforums.
 */
function theme_advanced_forum_subforum_list($subforum_list) {
  $subforums = array();
  foreach($subforum_list AS $tid => $subforum) {
    $subforums[] = l($subforum->name, "forum/$tid");
  }
  
  return implode(', ', $subforums);
}

// TEMPLATE PREPROCESS ******************************************************/

/*** FORUM OVERVIEW & TOPIC LIST PAGES **************************************/

/**
 * Preprocesses template variables for the forum template.
 */
function advanced_forum_preprocess_forums(&$variables) {
   include_once drupal_get_path('module', 'advanced_forum') . '/includes/advanced_forum_preprocess_forums.inc';
   _advanced_forum_preprocess_forums($variables);
}

function advanced_forum_preprocess_advanced_forum_search_forum(&$variables) {
  $variables['template_files'][] = 'advanced_forum-search-forum';

  $variables['forum'] = $variables['tid'] ? $variables['tid'] : 'All';
  $variables['path'] = url('forum/search');
}

function advanced_forum_preprocess_views_view_fields__advanced_forum_search(&$variables) {
  _advanced_forum_add_files();
  $variables['template_files'][] = 'advanced_forum-search-result';
}

/**
 * Preprocesses template variables for the submitted by/in template.
 */
function advanced_forum_preprocess_forum_submitted(&$variables) {
  $variables['template_files'][] = 'advanced_forum-submitted';

  // Avoid E_ALL warning
  $variables['topic_link'] = '';

  if (isset($variables['topic']->node_title)) {
    $nid = $variables['topic']->nid;

    // Make a fake node object to avoid the node load
    $node = new stdClass();
    $node->nid = $nid;
    $node->type = $variables['topic']->type;

    // Find the page of the first unread comment, if any
    $num_comments = db_result(db_query('SELECT COUNT(cid) FROM {comments} WHERE nid = %d', $nid));
    $new_replies = advanced_forum_reply_num_new($nid);
    $query = advanced_forum_page_first_new($num_comments, $new_replies, $node);

    // Format the node title with a link
    $title_length = variable_get('advanced_forum_topic_title_length', 15);
    if ($title_length == 0) {
      $short_topic_title = $variables['topic']->node_title;
    }
    else {
      $short_topic_title = truncate_utf8($variables['topic']->node_title, $title_length, TRUE, TRUE);
    }

    $fragment = ($new_replies) ? 'new' : NULL;
    $variables['topic_link'] = l($short_topic_title, "node/$nid", array('query' => $query, 'fragment' => $fragment));
  }

  // For items posted more than $cutoff hours ago, offer an actual date.
  $cutoff = variable_get('advanced_forum_time_ago_cutoff', 48) * 60 * 60;
  if (isset($variables['topic']->timestamp)) {
      $timestamp = $variables['topic']->timestamp;
      if (time() - $timestamp > $cutoff) {
        $variables['date_posted'] = format_date($timestamp, 'small');
      }
  }
}

/*** JUST FORUM OVERVIEW PAGE ***********************************************/

/**
 * Preprocesses template variables for the forum legend template.
 */
function advanced_forum_preprocess_advanced_forum_forum_legend(&$variables) {
  $variables['template_files'][] = 'advanced_forum-forum-legend';
}

/**
 * Preprocesses template variables for the forum statistics template.
 */
function advanced_forum_preprocess_advanced_forum_statistics(&$variables) {
  $variables['template_files'][] = 'advanced_forum-statistics';

  $variables['topics'] = advanced_forum_statistics_topics();
  $variables['posts'] = advanced_forum_statistics_replies() + $variables['topics'];
  $variables['users'] = advanced_forum_statistics_users();

  $authenticated_users = advanced_forum_statistics_online_users();
  $variables['online_users'] = implode(', ', $authenticated_users);
  $variables['current_users'] = count($authenticated_users);
  $variables['current_guests'] = sess_count(time() - variable_get('user_block_seconds_online', 900));
  $variables['current_total'] = $variables['current_users'] + $variables['current_guests'];

  $latest_user = advanced_forum_statistics_latest_user();
  $variables['latest_user'] = theme('username', $latest_user);
}

/**
 * Preprocesses template variables for the forum list template.
 */
function advanced_forum_preprocess_forum_list(&$variables) {
   include_once drupal_get_path('module', 'advanced_forum') . '/includes/advanced_forum_preprocess_forum_list.inc';
   _advanced_forum_preprocess_forum_list($variables);
}


/*** JUST TOPIC LIST PAGES **************************************************/

function advanced_forum_preprocess_views_view_forum_topic_list__advanced_forum_topic_list(&$variables) {
  _advanced_forum_add_files();
  $variables['template_files'][] = 'advanced_forum-topic-list-view';

  // Set a variable for displaying the topic legend.
  $variables['topic_legend'] = theme('advanced_forum_topic_legend');
}

function advanced_forum_preprocess_views_view__advanced_forum_topic_list(&$variables) {
  _advanced_forum_add_files();
  $variables['template_files'][] = 'advanced_forum-topic-list-outer-view';
  
  // Set a variable for displaying the search.
  $view = $variables['view'];
  $forum = $view->args[0];
  $variables['search'] = theme('advanced_forum_search_forum', $forum);
  
  // Set a variable for displaying the topic legend.
  $variables['topic_legend'] = theme('advanced_forum_topic_legend');
}

/**
 * Display a view as a forum topic list style.
 */
function template_preprocess_advanced_forum_topic_list_view(&$variables) {
  include_once drupal_get_path('module', 'advanced_forum') . '/includes/template_preprocess_advanced_forum_topic_list_view.inc';
  _template_preprocess_advanced_forum_topic_list_view($variables);
}


/**
 * Preprocesses template variables for the topic list template.
 */
function advanced_forum_preprocess_forum_topic_list(&$variables) {
  // Take control of the template file.
  $variables['template_files'][] = 'advanced_forum-topic-list';

  if (!module_exists('views')) {
    include_once drupal_get_path('module', 'advanced_forum') . '/includes/advanced_forum_preprocess_forum_topic_list.inc';
    _advanced_forum_preprocess_forum_topic_list($variables);
  }
}

/**
 * Preprocesses template variables for the forum icon template.
 */
function advanced_forum_preprocess_forum_icon(&$variables) {
  $variables['template_files'][] = 'advanced_forum-topic-icon';
}

/**
 * Preprocesses template variables for the topic legend template.
 */
function advanced_forum_preprocess_advanced_forum_topic_legend(&$variables) {
  $variables['template_files'][] = 'advanced_forum-topic-legend';
}


/*** TOPIC PAGES ************************************************************/

function advanced_forum_preprocess_advanced_forum_search_topic(&$variables) {
  $variables['template_files'][] = 'advanced_forum-search-topic';

  $variables['path'] = url('node/'. $variables['node']->nid . '/search');
}

function advanced_forum_preprocess_views_view_fields__advanced_forum_search_topic(&$variables) {
  _advanced_forum_add_files();
  $variables['template_files'][] = 'advanced_forum-search-result';
}

/**
 * Preprocesses template variables for the topic header template.
 */
function advanced_forum_preprocess_advanced_forum_topic_header(&$variables) {
  $variables['template_files'][] = 'advanced_forum-topic-header';

  // Build the reply link / button
  $variables['reply_link'] = theme('advanced_forum_reply_link', $variables['node']);

  if (!empty($variables['comment_count'])) {
    // Add count of total posts and link to last reply
    $variables['total_posts'] = format_plural($variables['comment_count'], '1 reply', '@count replies');
    $variables['last_post'] = advanced_forum_last_post_link($variables['node']);
  }
  else {
    $variables['total_posts'] = t('No replies');
  }

  // Add count of new posts (if any) linked to the first one.
  $variables['new_posts'] = advanced_forum_first_new_post_link($variables['node'], $variables['comment_count']);
  
  // Add search form to search within the topic. (only works with nodecomment)
  $variables['search'] = theme('advanced_forum_search_topic', $variables['node']);
}

/**
 * Preprocesses template variables for the author pane.
 */
function advanced_forum_preprocess_author_pane(&$variables) {
  // Author pane is used in various places. The only way to be sure to only
  // hit it when we are using it is to check the template suggestion.
  if (!empty($variables['template_suggestion']) && $variables['template_suggestion'] == 'advanced_forum-author-pane') {
    // Attempt to override the user picture fetched by Author Pane from core.
    $sized_picture = theme('advanced_forum_user_picture', $variables['account']);
    if (!empty($sized_picture)) {
      $variables['picture'] = $sized_picture;
    }
  }
}

/**
 * Preprocesses template variables for the node template.
 */
function advanced_forum_preprocess_node(&$variables) {
 if (advanced_forum_treat_as_forum_post('node', $variables)) {
   include_once drupal_get_path('module', 'advanced_forum') . '/includes/advanced_forum_preprocess_node.inc';
   _advanced_forum_preprocess_node($variables);
 }
}

function advanced_forum_preprocess_comment_wrapper(&$variables) {
  if (advanced_forum_treat_as_forum_post('comment-wrapper', $variables)) {
    $variables['template_files'][] = 'advanced_forum-comment-wrapper';

    $variables['reply_link'] = '';
    if ($variables['node']->comment != COMMENT_NODE_READ_WRITE || variable_get('comment_form_location_'. $variables['node']->type, COMMENT_FORM_SEPARATE_PAGE) == COMMENT_FORM_SEPARATE_PAGE) {
      // If the post is locked or the comment form is on a seperate page, build
      // the reply/locked link / button
      $variables['reply_link'] = theme('advanced_forum_reply_link', $variables['node']);
    }
  }
}

/**
 * Preprocesses template variables for the comment template.
 */
function advanced_forum_preprocess_comment(&$variables) {
 if (advanced_forum_treat_as_forum_post('node', $variables)) {
   include_once drupal_get_path('module', 'advanced_forum') . '/includes/advanced_forum_preprocess_comment.inc';
   _advanced_forum_preprocess_comment($variables);
 }
}

function advanced_forum_preprocess_forum_topic_navigation(&$variables) {
  $variables['prev'] = $variables['next'] = '';
}
