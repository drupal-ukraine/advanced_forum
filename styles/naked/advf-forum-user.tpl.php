<?php
// $Id$

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
$ip_raw
$ip
$profile - Profile object from core profile. Usage: $profile['category']['field_name']['value']
  Example: Real name: <?php print $profile['Personal info']['profile_name']['value']; ?>
$subscribe - Formatted link to subscribe to the author's forum topics
$subscribe_link - As above but just the relative path

*/
?>
<div class="user-info-inner">

  <?php print $name; ?>
  <?php print $user_title; ?>
  <?php print $picture; ?>
  <?php print $posts; ?>
  <?php print $joined; ?>
  <?php print $points; ?>
  <?php print $user_badges;  ?>
  <?php print $online_status; ?>
  <?php print $contact; ?>
  <?php print $privatemsg; ?>
  <?php print $buddylist; ?>
  <?php print $ip; ?>
  
</div>
