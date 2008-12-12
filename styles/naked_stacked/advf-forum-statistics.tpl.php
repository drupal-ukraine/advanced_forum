<div id="forum-statistics">
  <div id="forum-statistics-header"><?php print t('What\'s Going On?'); ?></div>

  <div id="forum-statistics-active-header" class="forum-statistics-sub-header">
    <?php print t('Currently active users: !current_total (!current_users users and !current_guests guests)', array('!current_total' => $current_total, '!current_users' => $current_users, '!current_guests' => $current_guests)); ?>
  </div>

  <div id="forum-statistics-active-body" class="forum-statistics-sub-body">
    <?php print $online_users; ?>
  </div>

  <div id="forum-statistics-statistics-header" class="forum-statistics-sub-header">
    <?php print t('Statistics'); ?>
  </div>

  <div id="forum-statistics-statistics-body" class="forum-statistics-sub-body">
    <?php print t('Topics: !topics, Posts: !posts, Users: !users', array('!topics' => $topics, '!posts' => $posts, '!users' => $users)); ?>
    <br /><?php print t('Welcome to our latest member, !user', array('!user' => l($latest_user_name, 'user/' . $latest_user_id))); ?>
  </div>
</div>
