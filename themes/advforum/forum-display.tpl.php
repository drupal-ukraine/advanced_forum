<?php
  global $user;
  // forum list, topics list, topic browser and 'add new topic' link

  $vocabulary = taxonomy_get_vocabulary(variable_get('forum_nav_vocabulary', ''));
  $title = $vocabulary->name;

  // Breadcrumb navigation:
  $breadcrumb = array();
  if ($tid) {
    $breadcrumb[] = array('path' => 'forum', 'title' => $title);
  }

  if ($parents) {
    $parents = array_reverse($parents);
    foreach ($parents as $p) {
      if ($p->tid == $tid) {
        $title = $p->name;
      }
      else {
        $breadcrumb[] = array('path' => 'forum/'. $p->tid, 'title' => $p->name);
      }
    }
  }

  drupal_set_title(check_plain($title));

  $breadcrumb[] = array('path' => $_GET['q']);
  menu_set_location($breadcrumb);

  if (count($forums) || count($parents)) {
    $output  = '<div id="forum">';

    if ($user->uid && user_access('access content')) {
      
      $markbutton = l($tid ? t('Mark all topics read') : t('Mark all forums read'),"forum/markasread/$tid",array('class'=>'cssbutton'));
      $output .= '<div class ="markasread">' . $markbutton . '</div>';
    }

      $output .= '<span class="clear"></span>';
    $output .= theme('forum_list', $forums, $parents, $tid);

    if ($tid && !in_array($tid, variable_get('forum_containers', array()))) {
      if (user_access('create forum topics')) {
        $output .= '<div id="newtopiclink" class="cssbutton">' . 
                   l(t('Post new forum topic'), "node/add/forum/$tid") . 
                   '</div>';
      }
      else if ($user->uid) {
        $output .= t('You are not allowed to post a new forum topic');
      }
      else {
        $output .= '<div id="newtopiclink" class="cssbutton">';
        $output .= t('<a href="@login">Login to post a new forum topic</a>', array('@login' => url('user/login', drupal_get_destination()))) ;
        $output .= '</div>';
      }
      
      
      $output .= theme('forum_topic_list', $tid, $topics, $sortby, $forum_per_page);
      drupal_add_feed(url('taxonomy/term/'. $tid .'/0/feed'), 'RSS - '. $title);
    }
    $output .= '</div>';
  }
  else {
    drupal_set_title(t('No forums defined'));
    $output = '';
  }
 
  print $output;
  
