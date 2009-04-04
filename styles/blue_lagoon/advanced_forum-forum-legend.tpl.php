<?php
// $Id$

/**
 * @file
 * Theme implementation to show forum legend.
 *
 */
?>
<div class="forum-folder-legend forum-smalltext">
  <dl>
    <dt><?php print $folder_new_posts; ?></dt>
    <dd><?php print t('Forum Contains New Posts'); ?></dd>
    <dt><?php print $folder_default; ?></dt>
    <dd><?php print t('Forum Contains No New Posts'); ?></dd>
    <dt><?php print $folder_locked ?></dt>
    <dd><?php print t('Forum is Locked'); ?></dd>
  </dl>
</div>
<br style="clear: both;" />
