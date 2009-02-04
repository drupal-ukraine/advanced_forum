<?php
// $Id$

/**
 * @file
 * Theme implementation to show forum legend.
 *
 */
?>
<div class="forum-topic-legend forum-smalltext clear-block">
  <div class="legend-group">
    <dl>
      <dt><?php print $topic_default; ?></dt>
      <dd><?php print t('No New Posts'); ?></dd>
    </dl>
    <dl>
      <dt><?php print $topic_new; ?></dt>
      <dd><?php print t('New Posts'); ?></dd>
    </dl>
  </div>
  <div class="legend-group">
    <dl>
      <dt><?php print $topic_hot; ?></dt>
      <dd><?php print t('Hot Thread (No New)'); ?></dd>
    </dl>
    <dl>
      <dt><?php print $topic_hot_new; ?></dt>
      <dd><?php print t('Hot Thread (New)'); ?></dd>
    </dl>
  </div>
  <div class="legend-group">
    <dl>
      <dt><?php print $topic_sticky; ?></dt>
      <dd><?php print t('Sticky Thread'); ?></dd>
    </dl>
    <dl>
      <dt><?php print $topic_closed; ?></dt>
      <dd><?php print t('Locked Thread'); ?></dd>
  </dl>
  </div>
</div>

 