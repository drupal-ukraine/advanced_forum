<?php
// $Id$

/**
 * @file
 *
 * Theme implementation: Template for forum topic header.
 *
 * - $node: Node object.
 * - $search: Search box to search this topic (Requires Node Comments)
 * - $reply_link: Text link / button to reply to topic.
 * - $total_posts_count: Number of posts in topic.
 * - $new_posts_count: Number of new posts in topic.
 * - $first_new_post_link: Link to first unread post.
 * - $last_post_link: Link to last post.
 */
?>

<div id="forum-topic-header" class="forum-topic-header clear-block">
  <?php print $search; ?>

  <?php print $total_posts_count; ?> / <?php print t('!new new', array('!new' => $new_posts_count)); ?>

  <?php print $reply_link; ?>

  <?php if (!empty($new_posts_count)): ?>
    <?php print $first_new_post_link; ?>
  <?php endif; ?>

  <?php if (!empty($last_post_link)): ?>
     <?php print $last_post_link; ?>
  <?php endif; ?>
  
  <a id="forum-topic-top"></a>
</div>
