<?php
// $Id$

/**
 * @file advf-forum-user.tpl.php
 * Theme implementation to display information about the posting user.
 *
 * Available variables (core modules):
 * - $account: The entire user object.
 * - $picture: Themed user picture.
 * - $name: Themed user name wrapped in a div class "username". 
 * - $name_raw: Themed user name. 
 *
 * - $joined: Date the user joined the site in the format "Joined: DATE"
 * - $joined_raw: Date the user joined the site with no markup
 * - $joined_ago: Time since the user registered in the format "Joined: TIME ago"
 * - $joined_ago_raw: Amount of time since the user joined the site with no markup.
 *
 * - $online_icon: Online/Offline icon.
 * - $online_status: Translated text "Online" or "Offline"
 *
 * - $contact: Marked up and linked icon and text.
 * - $contact_icon: Unlinked icon.
 * - $contact_link: raw link to user contact page. ie: 'user/1/contact'.
 * 
 * - $profile - Profile object from core profile. 
 *     Usage: $profile['category']['field_name']['value']
 *     Example: <?php print $profile['Personal info']['profile_name']['value']; ?>
 *
 * Available variables (contributed modules):
 * - $facebook_status: Status from the facebook status module.
 *
 * - $privatemsg: Marked up and linked icon and text.  
 * - $privatemsg_icon: Unlinked icon. 
 * - $privatemsg_link: Raw link to send private message to user. 
 *
 * - $user_stats_posts: Marked up number of posts from user stats module.
 * - $user_stats_posts_raw: Just the post count.
 * - $user_stats_ip: Marked up IP address from user stats module.
 * - $user_stats_ip_raw: Just the IP address.
 *
 * - $userpoints_points: Marked up number of points from userpoints module.
 * - $userpoints_points_raw: Just the points count.
 *
 * 5.x only at this time:
 *
 * - $buddylist: Marked up and linked icon and text.  
 * - $buddylist_icon: Unlinked icon. 
 * - $buddylist_link: Raw link to send add/remove poster as a buddy. 
 *
 * - $subscribe: Formatted link to subscribe to the author's forum topics.
 * - $subscribe_link: As above but just the relative path.
 *
 * - $user_title: Marked up title from user titles module.
 * - $user_title_raw: Just the title.
 *
 * - $user_badges: Marked up badges from user badges module.
 * - $user_badges_raw: Just the badges.

*/
?>
<div class="user-info-inner">

  <?php print $name; ?>
  <?php if (isset($user_title)) print $user_title; ?>
  <?php if (isset($user_badges)) print $user_badges;  ?>
  <?php print $picture; ?>
  <br />
  <?php print $joined; ?>
  <?php if (isset($user_stats_posts)) print $user_stats_posts; ?>
  <?php if (isset($userpoints_points)) print $userpoints_points; ?>
  <br />
  <?php if (isset($contact)) print $contact; ?>
  <?php if (isset($privatemsg)) print $privatemsg; ?>
  <?php if (isset($buddylist)) print $buddylist; ?>
  <br />
  <?php print $online_status; ?>
  <?php if (isset($users_stats_ip)) print $user_stats_ip; ?>
  
</div>
