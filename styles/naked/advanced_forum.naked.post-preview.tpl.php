<?php
// $Id$

/**
 * @file
 *
 * Theme implementation: Template the preview version of a post.
 *
 * All variables available in node.tpl.php and comment.tpl.php for your theme
 * are available here. In addition, Advanced Forum makes available the following
 * variables:
 *
 * - $top_post: TRUE if we are formatting the main post (ie, not a comment)
 * - $reply_link: Text link / button to reply to topic.
 * - $total_posts: Number of posts in topic (not counting first post).
 * - $new_posts: Number of new posts in topic, and link to first new.
 * - $links_array: Unformatted array of links.
 * - $account: User object of the post author.
 * - $name: User name of post author.
 * - $author_pane: Entire contents of advanced_forum-author-pane.tpl.php.

 */
?>

<?php if ($top_post): ?>
  <a id="forum-topic-top"></a>
<?php else: ?>
  <a id="forum-reply-preview"></a>
<?php endif; ?>

<?php
// Gather other possible class list variables into ours. This must be done here
// rather than in the preprocess because themes run after the AF preprocess.
  $all_classes = "";
  if (!empty($advanced_forum_classes)) {
    $all_classes = $advanced_forum_classes;
  }
  
  if (!empty($classes)) {
    $all_classes .= ' ' . $classes;
  }

  if (!empty($node_classes)) {
    $all_classes .= ' ' . $node_classes;
  }

  if (!empty($comment_classes)) {
    $all_classes .= ' ' . $comment_classes;
  }
?>

<div id="<?php print $post_id; ?>" class="<?php print $all_classes; ?>">
  <div class="forum-post-info clear-block">
    <div class="forum-posted-on">
      <?php print $date ?>

      <?php if (!$top_post): ?>
        <?php if (!empty($first_new)): ?>
          <?php print $first_new; ?>
        <?php endif; ?>
        <?php print $new_output; ?>
      <?php endif; ?>
    </div>

    <?php if (!$top_post): ?>
      <span class="forum-post-number"><?php print $comment_link . ' ' . $page_link; ?></span>
    <?php endif; ?>
  </div>

  <div class="forum-post-wrapper">
    <div class="forum-post-panel-sub">
      <?php print $author_pane; ?>
    </div>

    <div class="forum-post-panel-main clear-block">
      <?php if ($title): ?>
        <div class="post-title">
          <?php print $title ?>
        </div>
      <?php endif; ?>

      <div class="forum-post-content">
        <?php print $content ?>
      </div>

      <?php if ($signature): ?>
        <div class="author-signature">
          <?php print $signature ?>
        </div>
      <?php endif; ?>
    </div>
  </div>

  <div class="forum-post-footer clear-block">
    <div class="forum-jump-links">
      <a href="#forum-topic-top" title="Jump to top of page"><?php print t("Top"); ?></a>
    </div>
  </div>
</div>