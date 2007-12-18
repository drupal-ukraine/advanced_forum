<?php
  global $user;

  if ($forums) {

    $header = array(t('Forum'), t('Topics'), t('Posts'), t('Last post'));

    foreach ($forums as $forum) {
      if ($forum->container) {
        $description  = '<div style="margin-left: '. ($forum->depth * 30) ."px;\">\n";
        $description .= ' <div class="name">'. l($forum->name, "forum/$forum->tid") ."</div>\n";

        if ($forum->description) {
          $description .= ' <div class="description">'. filter_xss_admin($forum->description) ."</div>\n";
        }
        $description .= "</div>\n";

        $rows[] = array(array('data' => $description, 'class' => 'container', 'colspan' => '4'));
      }
      else {
        $new_topics = _forum_topics_unread($forum->tid, $user->uid);
        $forum->old_topics = $forum->num_topics - $new_topics;
        if (!$user->uid) {
          $new_topics = 0;
        }

        $description  = '<div style="margin-left: '. ($forum->depth * 30) ."px;\">\n";
        $description .= ' <div class="name">'. l($forum->name, "forum/$forum->tid") ."</div>\n";

        if ($forum->description) {
          $description .= ' <div class="description">'. filter_xss_admin($forum->description) ."</div>\n";
        }
        $description .= "</div>\n";

        $rows[] = array(
          array('data' => $description, 'class' => 'forum'),
          array('data' => $forum->num_topics . ($new_topics ? '<br />'. l(format_plural($new_topics, '1 new', '@count new'), "forum/$forum->tid", NULL, NULL, 'new') : ''), 'class' => 'topics'),
          array('data' => $forum->num_posts, 'class' => 'posts'),
          array('data' => _forum_format($forum->last_post), 'class' => 'last-reply'));
      }
    }

    print theme('table', $header, $rows);

  }
