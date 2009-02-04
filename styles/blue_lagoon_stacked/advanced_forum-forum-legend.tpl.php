<?php
// $Id$

/**
 * @file
 * Theme implementation to show forum legend.
 *
 */
?>
<div class="forum-folder-legend forum-smalltext">
  <dl><?php print advanced_forum_theme_image('forum-folder-new-posts.png', t('Forum Contains New Posts')); ?></dl>
  <dd><?php print t('Forum Contains New Posts'); ?></dd>
  <dl><?php print advanced_forum_theme_image('/forum-folder.png', t('Forum Contains No New Posts')); ?></dl>
  <dd><?php print t('Forum Contains No New Posts'); ?></dd>
  <dl><?php print advanced_forum_theme_image('forum-folder-locked.png', t('Forum is Locked')); ?></dl>
  <dd><?php print t('Forum is Locked'); ?></dd>
</div>
<br style="clear: both;">
