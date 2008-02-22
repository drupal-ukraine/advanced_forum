<?php
/**
 * node-forum.tpl.php is the template file for both
 * the top post (the node) and the comments/replies
 * Changes here will affect an individual forum post.
 
 * The following standard variables are available to you:
    $is_forum  - TRUE if we are in the forums. Useful to format the
               - node differently if promoted to the front page, for example.
    $top_post  - TRUE if we are formatting the main post (ie, not a comment)
    $row_class - ODD or EVEN post/comment
    $title     - Title of this post/comment
    $content   - Content of this post/comment
    $links     - Formatted links (reply, edit, delete, etc)
    $submitted - Formatted date post/comment submitted

    $accountid    - ID of the poster
    $name      - User name of poster    
 */
?> 

<?php
// If this is the top post (that is, the node) give it an extra wrapper to allow for special theming
if ($top_post) {
  $postclass = "top-post";
} 
?>

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
      <?php print theme('forum_user',$accountid); ?>     
     </div>
    </div>

    <div class="forum-comment-right">
      <div class="posttitle">
        <?php print $title ?>
      </div>
      
      <div class="content">
        <?php print $content ?>
      </div>  
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
