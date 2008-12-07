<?php
// $Id$

/**
 * @file advf-forum-repeat-post.tpl.php
 * Used for the repeated node on the top of each page of a paginated forum
 * thread. By default, it contains only the "header" information for the thread
 * and the rest is empty.
 *
 * If you leave it empty, subsequent pages will start with the next comment
 * like you typically find in forum software. You could also put a specially
 * formatted teaser to remind people what post they are reading. If you like
 * having the entire node repeated, simply copy the entire contents of
 * advf-forum-post.tpl.php into this file. All the same variables are available.
 */
?>

<div class="forum-post-header clear-block">
  <?php print $reply_link; ?>
  
  <div class="reply-count">
    <?php print $total_posts; ?>
    
    <?php if (!empty($new_posts)): ?>
      (<?php print $new_posts; ?>)
    <?php endif; ?>
  </div>
</div>