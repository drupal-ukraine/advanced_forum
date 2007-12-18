<?php
/**
 * node-forum.tpl.php is the template file for both
 * the top post (the node) and the comments/replies
 * Changes here will affect an individual forum thread.
 
 * The following standard variables are available to you:
    $is_forum  - TRUE if we are in the forums. Useful to format the
               - node differently if promoted to the front page, for example.
    $top_post  - TRUE if we are formatting the main post (ie, not a comment)
    $row_class - ODD or EVEN post/comment
    $title     - Title of this post/comment
    $content   - Content of this post/comment
    $links     - Formatted links (reply, edit, delete, etc)
    $submitted - Formatted date post/comment submitted

    $userid    - ID of the poster
    $name      - User name of poster
    $joined    - Formatted date of when the poster joined the site
    $posts     - Number of forum posts/comments by the poster

 */
?> 

<?php
// Show nodes normally when they appear outside of the forum
if (!$is_forum) {
  include('node.tpl.php');
  return;
}

// If this is the top post (that is, the node) give it an extra wrapper to allow for special theming
if ($top_post) {
  $postclass = "top-post";
} 
?>

<div class="<?php print $postclass ?> forum-comment forum-comment-<?php print $row_class; print $comment->new ? ' comment-new forum-comment-new' : ''; ?>">

  <div class="post-info">
     <span class="postedon"><?php print "Posted on: " . $date ?></span>
    
    <?php if ($comment->new) : ?>
      <a id="new"></a>
      <span class="new"><?php print $new ?></span>
    <?php endif ?>

    <?php 
      if (!$top_post) {
        print '<span class="post-num">';
        print $comment_link;
        print '</span>' ;
      }
    ?>
  </div>
  <span class="clear"></span>

  <div class="forum-post-wrapper">
    <div class="forum-comment-left">
      <div class="innertube">

        <?php // BEGIN AUTHOR NAME DISPLAY ?>
        <div class="author-name">
          <?php print $name ?>
        </div>
        <?php // END AUTHOR NAME DISPLAY ?>
          
        <?php // BEGIN AUTHOR TITLE DISPLAY ?>
        <?php if (isset($user_title)) { ?> 
        <div class="author-title">
          <?php print $user_title; ?>
        </div>
        <?php } ?>
        <?php // END AUTHOR TITLE DISPLAY ?>

        <?php // BEGIN AUTHOR AVATAR DISPLAY ?>
        <?php print $picture ?>
        <?php // END AUTHOR AVATAR DISPLAY ?>
        
        <?php // BEGIN AUTHOR POST COUNT DISPLAY (user postcount module in advanced forum?>
        <?php if (isset($posts)) { ?> 
        <div class="author-posts">
          <?php print t('Posts:') . ' ' . $posts; ?>
        </div>
        <?php } ?>
        <?php // END AUTHOR POST COUNT DISPLAY ?>
      
        <?php // BEGIN AUTHOR JOIN DATE DISPLAY ?>
        <?php if (isset($joined)) { ?> 
        <div class="author-regdate">
          <?php print t('Joined:') . ' ' . $joined; ?>
        </div>
        <?php } ?>
        <?php // END AUTHOR JOIN DATE DISPLAY ?>
      
        <?php // BEGIN AUTHOR POINTS DISPLAY (userpoints module)?>
        <?php if (isset($points)) { ?> 
        <div class="author-points">
          <?php print t('Points:') . ' ' . $points?>
        </div>
        <?php } ?>
        <?php // END AUTHOR POINTS DISPLAY ?>
        
        <?php 
        print $online_icon; 
        ?>
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
  
  </div>
  <span class="clear"></span>
  
  <div class="links">
    <?php print $links ?>
  </div>
  
</div>

<span class="clear"></span>
<br />

