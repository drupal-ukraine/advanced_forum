<?php
// $Id$

/**
 * @file
 *
 * Theme implementation: Template for forum topic header.
 *
 * - $classes: List of classes to apply to the wrapping div.
 * - $reply_link: Text link / button to reply to topic.
 * - $total_posts: Number of posts in topic (not counting first post).
 * - $new_posts: Number of new posts in topic, and link to first new.
 * - $node: Node object.
 * - $search: Search box to search this topic (NC only)

 */
?>

<div id="topic-header" class="<?php print $classes; ?> ">
  <a id="forum-topic-top"></a>

  <?php print $reply_link; ?>

  <?php print $search; ?>

  <div class="reply-count">
    <?php print $total_posts; ?>

    <?php if (!empty($new_posts)): ?>
      [<?php print $new_posts; ?>]
    <?php endif; ?>

    <?php if (!empty($last_post)): ?>
       [<?php print $last_post; ?>]
    <?php endif; ?>
  </div>
</div>
