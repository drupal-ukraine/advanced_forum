<?php
// $Id$

/**
 * @file advf-forum-post.tpl.php
 *
 * Theme implementation: Template for each forum post whether node or comment.
 *
 * All variables available in node.tpl.php and comment.tpl.php for your theme
 * are available here. In addition, Advanced Forum makes available the following
 * variables: 
 * 
 * - $top_post: TRUE if we are formatting the main post (ie, not a comment)
 * - $reply_link: Text link / button to reply to topic. 
 * - %total_posts: Number of posts in topic (not counting first post).
 * - $new_posts: Number of new posts in topic, and link to first new.
 * - $links_array: Unformatted array of links.
 * - $account: User object of the post author.
 * - $name: User name of post author.
 * - $author_pane: Entire contents of advf-author-pane.tpl.php.

 */
?>

<?php if ($top_post): ?>
  <div class="forum-post-header clear-block">
    <?php print $reply_link; ?>
    
    <div class="reply-count">
      <?php print $total_posts; ?>
      
      <?php if (!empty($new_posts)): ?>
        (<?php print $new_posts; ?>)
      <?php endif; ?>

      <?php if (!empty($last_post)): ?>
        <?php print $last_post; ?>
      <?php endif; ?>
     </div>   
  </div>

  <?php $classes .= $node_classes; ?>
  <div id="node-<?php print $node->nid; ?>" class="top-post forum-post <?php print $classes; ?> clear-block">
  <a id="top"></a>
  
<?php else: ?>
  <?php $classes .= $comment_classes; ?>
  <div id="comment-<?php print $comment->cid; ?>" class="forum-post <?php print $classes; ?> clear-block">
<?php endif; ?>

  <div class="post-info clear-block">
    <div class="posted-on">
      <?php print $date ?>

      <?php if (!$top_post && !empty($comment->new)): ?>
        <a id="new"><span class="new">(<?php print $new ?>)</span></a>      
      <?php endif; ?>
    </div>

    <?php if (!$top_post): ?>
      <span class="post-num"><?php print $comment_link . ' ' . $page_link; ?></span>
    <?php endif; ?>
  </div>

  <div class="forum-post-wrapper">

    <div class="forum-post-panel-sub">
      <div class="author-pane">
        <?php print $author_pane; ?>
     </div>
    </div>

    <div class="forum-post-panel-main clear-block">
      <?php if ($title && !$top_post): ?>
        <div class="post-title">
          <?php print $title ?>
        </div>
      <?php endif; ?>

      <div class="content">
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
      <a href="#top">Top</a>
    </div>
    
    <?php if (!empty($links)): ?>
      <div class="forum-post-links">
        <?php print $links ?>
      </div>
    <?php endif; ?>
  </div>
</div>