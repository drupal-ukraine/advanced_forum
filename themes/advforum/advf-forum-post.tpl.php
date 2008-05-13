<?php
// $Id$

/**
 * forum-post.tpl.php is the template file for both
 * the top post (the node) and the comments/replies
 * Changes here will affect an individual forum post.
 
 * The following standard variables are available to you:
    $top_post    - TRUE if we are formatting the main post (ie, not a comment)
    $title       - Title of this post/comment
    $content     - Content of this post/comment
    $reply_link  - Separated out link to reply to topic
    $jump_first_new - Shows number of new (to user) comments and links to the first one
    $links       - Formatted links (reply, edit, delete, etc)
    $links_array - Unformatted array of links
    $submitted   - Formatted date post/comment submitted

    $accountid   - ID of the poster
    $name        - User name of poster
 */
?>

<?php
if ($top_post) {
  // Class that shows we are on the node to allow for special theming
  $postclass = "top-post";
?> 
  <div class="forum-post-header">
  <?php print $reply_link; ?>
  <?php print $jump_first_new; ?>
  </div>
<?php } ?>

<div class="<?php print $postclass ?> forum-comment forum-comment-<?php print $row_class; print $comment->new ? ' comment-new forum-comment-new' : ''; ?>">

  <div class="post-info">
     <span class="postedon"><?php print t("Posted on: ") . $date ?></span>
    
    <?php if ($comment->new) : ?>
      <a id="new"></a>
      <span class="new"><?php print $new ?></span>
    <?php endif ?>

    <?php
      if (!$top_post) {
        print '<span class="post-num">';
        print $comment_link;
        print ' ' . $page_link;
        print '</span>' ;
      }
    ?>
  </div>
  <div class="clear"></div>

  <div class="forum-post-wrapper">
    <div class="forum-comment-left">
      <div class="innertube">
      <?php print $user_info_pane; ?>   
     </div>
    </div>

    <div class="forum-comment-right">
      <div class="posttitle">
        <?php print $title ?>
      </div>
      
      <div class="content">
        <?php print $content ?>
       </div>  

      <!-- Note: This is only used in 6.x -->
      <?php if ($signature): ?>
        <div class="user-signature clear-block">
          <?php print $signature ?>
        </div>
      <?php endif; ?>
 
    </div>
    <div class="clear"></div>
    <div class="links">
    <?php print $links ?>
  </div>
  
  </div>
  <div class="clear"></div>
  
</div>

<div class="clear"></div>

<?php if ($top_post) { ?>
  <br />
<?php } ?>
