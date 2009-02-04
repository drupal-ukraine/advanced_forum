<?php
// $Id$

/**
 * @file
 * Theme implementation to display information about the post author.
 *
 * Available variables (core modules):
 * - $account: The entire user object for the author.
 * - $picture: Themed user picture for the author.
 * - $account_name: Themed user name for the author.
 * - $account_id: User ID number for the author.
 *
 * - $joined: Date the post author joined the site.
 * - $joined_ago: Time since the author registered in the format "TIME ago"
 *
 * - $online_icon: Icon that changes depending on whether the author is online.
 * - $online_status: Translated text "Online" or "Offline"
 * - $last_active: Time since author was last active. eg: "5 days 3 hours"
 *
 * - $contact: Linked icon.
 * - $contact_link: Linked translated text "Contact user".
 *
 * - $profile - Profile object from core profile.
 *     D5 Usage: $profile['category']['field_name']['value']
 *     D5 Example: <?php print $profile['Personal info']['profile_name']['value']; ?>
 *     D6 Usage: $profile['category']['field_name']['#value']
 *     D6 Example: <?php print $profile['Personal info']['profile_name']['#value']; ?>
 *
 * Available variables (contributed modules):
 * - $buddylist: Linked icon.
 * - $buddylist_link: Linked translated text "Add to buddylist" or "Remove from buddylist".

 * - $user_relationship_api: Linked icon.
 * - $user_relationship_api_link: Linked text "Add to <relationship>" or "Remove from <relationship>".

 * - $facebook_status: Status, including username, from the facebook status module.
 * - $facebook_status: Status from the facebook status module.
 *
 * - $privatemsg: Linked icon.
 * - $privatemsg_link: Linked translated text "Send PM".
 *
 * - $user_badges: Badges from user badges module.
 *
 * - $userpoints_points: Number of points from default category of the userpoints module.
 * - $userpoints_categories: Array holding each category and the points for that category.
 *
 * - $user_stats_posts: Number of posts from user stats module.
 * - $user_stats_ip: IP address from user stats module.
 *
 * - $user_title: Title from user titles module.

 * - $og_groups: Linked list of OG groups author is a member of.

 * - $location: User location as reported by the location module.
 */
?>

<div class="author-pane">
 <div class="author-pane-inner">
    <div class="author-pane-picture author-pane-section">
      <?php if (!empty($picture)): ?>
        <?php print $picture; ?>
      <?php endif; ?>
    </div>

    <div class="author-pane-name-status author-pane-section">

      <div class="author-pane-line author-name"> <?php print $account_name; ?> </div>

      <?php if (!empty($facebook_status_status)): ?>
        <div class="author-pane-line author-facebook-status"><?php print $facebook_status_status;  ?></div>
      <?php endif; ?>

      <?php if (!empty($user_title)): ?>
        <div class="author-pane-line author-title">
          <span class="author-pane-label"><?php print t('Title'); ?>:</span> <?php print $user_title; ?>
        </div>
      <?php endif; ?>

      <?php if (!empty($user_badges)): ?>
        <div class="author-pane-line author-badges"> <?php print $user_badges;  ?> </div>
      <?php endif; ?>

      <?php if (!empty($location)): ?>
        <div class="author-pane-line author-location"> <?php print $location;  ?> </div>
      <?php endif; ?>
    </div>

    <div class="author-pane-admin author-pane-section">

      <?php if (!empty($user_stats_ip)): ?>
        <div class="author-pane-line author-ip">
          <span class="author-pane-label"><?php print t('IP'); ?>:</span> <?php print $user_stats_ip; ?>
        </div>
      <?php endif; ?>

      <?php if (!empty($fasttoggle_block_author)): ?>
        <div class="author-fasttoggle-block"><?php print $fasttoggle_block_author; ?></div>
      <?php endif; ?>

    </div>

    <div class="author-pane-stats author-pane-contact author-pane-section">

      <?php if (!empty($joined)): ?>
        <div class="author-pane-line author-joined">
          <span class="author-pane-label"><?php print t('Joined'); ?>:</span> <?php print $joined; ?>
        </div>
      <?php endif; ?>

      <?php if (isset($user_stats_posts)): ?>
        <div class="author-pane-line author-posts">
          <span class="author-pane-label"><?php print t('Posts'); ?>:</span> <?php print $user_stats_posts; ?>
        </div>
      <?php endif; ?>

      <?php if (isset($userpoints_points)): ?>
        <div class="author-pane-line author-points">
          <span class="author-pane-label"><?php print t('!Points', userpoints_translation()); ?></span>: <?php print $userpoints_points; ?>
        </div>
      <?php endif; ?>

      <?php if (isset($og_groups)): ?>
        <div class="author-pane-line author-groups">
          <span class="author-pane-label"><?php print t('Groups'); ?>:</span> <?php print $og_groups; ?>
        </div>
      <?php endif; ?>

      <div class="author-pane-icon"><?php print $online_icon; ?></div>

      <?php if (!empty($contact)): ?>
        <div class="author-pane-icon"><?php print $contact; ?></div>
      <?php endif; ?>

      <?php if (!empty($privatemsg)): ?>
        <div class="author-pane-icon"><?php print $privatemsg; ?></div>
      <?php endif; ?>

      <?php if (!empty($buddylist)): ?>
        <div class="author-pane-icon"><?php print $buddylist; ?></div>
      <?php endif; ?>

      <?php if (!empty($user_relationships_api)): ?>
        <div class="author-pane-icon"><?php print $user_relationships_api; ?></div>
      <?php endif; ?>
    </div>

  </div>
</div>
