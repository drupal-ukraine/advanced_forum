<?php
// $Id$

/**
 * @file
 * Theme implementation to show forum legend.
 *
 */
?>
<div class="forum-float-left">
  <div class="forum-float-left">
    <dl class="forum-topic-legend forum-smalltext">
      <dd><?php print advanced_forum_theme_image('topic-new.png', t('New Posts')); ?><?php print t('New Posts'); ?></dd>
      <dd><?php print advanced_forum_theme_image('topic-hot-new.png', t('Hot Thread (New)')); ?><?php print t('Hot Thread (New)'); ?></dd>
      <dd><?php print advanced_forum_theme_image('topic-hot.png', t('Hot Thread (No New)')); ?><?php print t('Hot Thread (No New)'); ?></dd>
  </dl>
  </div>
  <div class="forum-float-left">
    <dl class="forum-topic-legend forum-smalltext">
      <dd><?php print advanced_forum_theme_image('topic-default.png', t('No New Posts')); ?><?php print t('No New Posts'); ?></dd>
      <dd><?php print advanced_forum_theme_image('topic-sticky.png', t('Sticky Thread')); ?><?php print t('Sticky Thread'); ?></dd>
      <dd><?php print advanced_forum_theme_image('topic-closed.png', t('Locked Thread')); ?><?php print t('Locked Thread'); ?></dd>
  </dl>
  </div>
  <br style="clear: both;">
</div>
