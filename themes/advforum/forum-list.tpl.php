<?php
  global $user;
  
  

  if ($forums) {
     
     $header = array(t('Forum'), t('Topics'), t('Posts'), t('Last post')); 
     /*{    $rows[] = array(
            array('data' => t('Subject'), 'class' => 'f-subject'),
            array('data' => t('Topics'), 'class' => 'f-topics'),
            array('data' => t('Posts'),'class' => 'f-posts'),
            array('data' => t('Last post'),'class' => 'f-last-reply')
            ); }*/

    foreach ($forums as $forum) {
      if ($forum->container) {
        // Add a class to the div for indenting sub containers
		if($forum->depth > 0) {
		  $description  = '<div class="forum-indented">' . "\n";
		} else {
		  $description  = '<div class="forum-default">' . "\n";
		}

        $description .= ' <div class="name">'. l($forum->name, "forum/$forum->tid") ."</div>\n";

        if ($forum->description) {
          $description .= ' <div class="description">'. filter_xss_admin($forum->description) ."</div>\n";
        }
        
        $description .= "</div>\n";
        $rows[] = array(array('data' => $description, 'class' => 'container', 'colspan' => '4'));
        
		/*if($forum->depth == 0) {
          $rows[] = array(
            array('data' => t('Forum'), 'class' => 'f-subject'),
            array('data' => t('Topics'), 'class' => 'f-topics'),
            array('data' => t('Posts'),'class' => 'f-posts'),
            array('data' => t('Last post'),'class' => 'f-last-reply')
            );
        }*/
      }
      else {
        if ($user->uid) {
          $new_topics = _forum_topics_unread($forum->tid, $user->uid);
        } else {
          $new_topics = 0;
        }
        
        $forum->old_topics = $forum->num_topics - $new_topics;
        

        // Add a class to the div for indenting sub forums
		if($forum->depth > 0) {
		  $description  = '<div class="forum-indented">' . "\n";
		} else {
		  $description  = '<div class="forum-default">' . "\n";
		}
			
        $description .= ' <div class="name">'. l($forum->name, "forum/$forum->tid") ."</div>\n";

        if ($forum->description) {
          $description .= ' <div class="description">'. filter_xss_admin($forum->description) ."</div>\n";
        }
        $description .= "</div>\n";

        // LAST POST
        $fLastPost = $forum->last_post;
        $lastPostTitle = advanced_forum_get_last_topic($fLastPost->uid, $forum->tid);
        if ($fLastPost && $fLastPost->timestamp) {
            $lpURL = $lastPostTitle->nid;
            if($lastPostTitle->resType == "comment") {
             $lpURL .= '#comment-'. $lastPostTitle->cid;
            }
          $last_post =  t('<a href="node/'. $lpURL .'">@title</a><br />@time ago<br />by !author', array('@title' => $lastPostTitle->nodeTitle, '@time' => format_interval(time() - $fLastPost->timestamp), '!author' => theme('username', $fLastPost)));
        } else {
          $last_post =  t('n/a');
        }

        $rows[] = array(
          array('data' => $description, 'class' => 'forum'),
          array('data' => $forum->num_topics . ($new_topics ? '<br />'. l(format_plural($new_topics, '1 new', '@count new'), "forum/$forum->tid", NULL, NULL, 'new') : ''), 'class' => 'topics'),
          array('data' => $forum->num_posts, 'class' => 'posts'),
          array('data' => $last_post, 'class' => 'last-reply'));
      }
    }

    // set table header if page is a container with listing of forums
    if(in_array(arg(1), variable_get('forum_containers', array()))){
      // reverse array
      $nrows = array_reverse($rows,true);
      
      //get the container description
      $container = taxonomy_get_term(arg(1));
            
      //add to output
      $rows = $nrows;
      $rows[] = array(
                array('data' => t('Subject'), 'class' => 'f-subject'),
                array('data' => t('Topics'), 'class' => 'f-topics'),
                array('data' => t('Posts'),'class' => 'f-posts'),
                array('data' => t('Last post'),'class' => 'f-last-reply')
                );
            
      $rows[] = array(array('data' => $container->description, 'class' => 'container', 'colspan' => 4));
            
      //reverse again to output
      $nrows = array_reverse($rows,true);
      $rows = $nrows;
    }
    print theme('table', $header, $rows);

  }
