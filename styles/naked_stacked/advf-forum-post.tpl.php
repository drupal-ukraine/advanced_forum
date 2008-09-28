<?php
// $Id$

/**
 * advf-forum-post.tpl.php is the template file for both
 * the top post (the node) and the comments/replies
 * Changes here will affect an individual forum post.
 
 * The following standard variables are available to you:
 * - $top_post: TRUE if we are formatting the main post (ie, not a comment)
 * - $title: Title of this post/comment
 * - $content: Content of this post/comment
 * - $reply_link: Separated out link to reply to topic
 * - $jump_first_new: Shows number of new (to user) comments and links to the first one
 * - $links: Formatted links (reply, edit, delete, etc)
 * - $links_array: Unformatted array of links
 * - $submitted: Formatted date post/comment submitted

 * - $account: User object of the poster
 * - $name: User name of poster
 * - $user_info_pane: Entire contents of advf-user-info.tpl.php
    
 */
?>

<?php if ($top_post): ?>
  <?php $postclass = "top-post"; ?> 
  
  <div class="forum-post-header">
    <?php print $reply_link; ?>
    <?php print $jump_first_new; ?>
  </div>
  
<?php endif; ?>

<div class="<?php print (isset($postclass)) ? $postclass . ' ' : ''; ?>forum-comment<?php print (isset($row_class)) ? ' forum-comment-' . $row_class : ''; print (!empty($comment->new)) ? ' comment-new forum-comment-new' : ''; ?> clearfix">

  <div class="post-info clearfix">
    <div class="posted-on"><?php print $date ?>
      
    <?php if (!$top_post): ?>
      <?php if (!empty($comment->new)) : ?>
         <a id="new"></a>
        <span class="new">(<?php print $new ?>)</span>
      <?php endif ?>
    <?php endif; ?>

    </div> 
    
    <?php if (!$top_post): ?>
      <span class="post-num"><?php print $comment_link . ' ' . $page_link; ?></span>
    <?php endif; ?>
  </div>

  <div class="forum-post-wrapper">
    
    <div class="forum-comment-left">
      <div class="author-info">
        <?php print $user_info_pane; ?>   
     </div>
    </div>

    <div class="forum-comment-right clearfix">
      <?php if ($title && !$top_post): ?>
        <div class="post-title">
          <?php print $title ?>
        </div>
      <?php endif; ?>  
              
      <div class="content">
        <?php print $content ?>
      </div>  

    </div>
    
    <?php if ($signature): ?>
      <div class="user-signature clear-block">
        <?php print $signature ?>
      </div>
    <?php endif; ?> 
    
    <?php if ($links): ?>
      <div class="links">
        <?php print $links ?>
      </div>
    <?php endif; ?>
 
  </div>
</div>