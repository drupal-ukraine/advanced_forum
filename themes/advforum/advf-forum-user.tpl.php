<?php
/*
$account - The entire user object for the profile user
$picture - returned from theme_user_picture
$joined_raw - Date the user joined the site with no markup
$joined - Date the user joined the site in the format "Joined: DATE" marked up
$member_since_raw - Amount of time since the user joined the site with no markup
$member_since - Amount of time since the user joined the site in the format "Member since: DATE" marked up
$online_class 
$online_icon 
$online_text 
$online_status 
$points_raw 
$points 
$posts_raw 
$posts 
$user_title_raw 
$user_title 
$user_badges_raw 
$user_badges 
$contact_class 
$contact_icon 
$contact_text 
$contact_link 
$contact 
$privatemsg_icon 
$privatemsg_text 
$privatemsg_link 
$privatemsg  
$buddylist_class 
$buddylist_icon 
$buddylist_text 
$buddylist_link 
$buddylist 
*/
?>
<div class="user-info">

  <?php print $name; ?>
  <?php print $picture; ?>

  <?php print $user_title; ?>
  <?php print $posts; ?>
  <?php print $joined; ?>
  <?php print $points; ?>
  <?php print $user_badges;  ?>
  <?php print $online_status; ?>
  <?php print $contact; ?>
  <?php print $privatemsg; ?>
  <?php print $buddylist; ?>
</div>
