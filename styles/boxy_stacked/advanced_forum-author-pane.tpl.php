<?php
// $Id$

/**
 * @file
 * Theme implementation to display information about the post/profile author.
 *
 * See author-pane.tpl.php in Author Pane module for a full list of variables.
 */
?>

<div class="author-pane">
  <div class="author-pane-inner">
    <div class="author-pane-top">
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

        <div class="author-pane-line author-pane-online">
          <span class="author-pane-online-icon"><?php print $online_icon; ?></span>
          <span class="author-pane-online-status"><?php print $online_status; ?></span>
        </div>

        <?php if (!empty($user_title)): ?>
          <div class="author-pane-line author-title"> <?php print $user_title; ?> </div>
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
          <div class="author-pane-line  author-fasttoggle-block"><?php print $fasttoggle_block_author; ?></div>
        <?php endif; ?>

        <?php if (!empty($troll_ban_author)): ?>
          <div class="author-pane-line author-troll-ban"><?php print $troll_ban_author; ?></div>
        <?php endif; ?>        
      </div>

      <div class="author-pane-stats author-pane-contact author-pane-section clear-block">
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
        
        <?php if (!empty($flag_friend)): ?>
          <div class="author-pane-icon"><?php print $flag_friend; ?></div>
        <?php endif; ?>
      </div>
    </div>

    <div class="author-pane-groups author-pane-section">
      <?php if (isset($og_groups)): ?>
         <div class="author-pane-line author-groups">
           <span class="author-pane-label"><?php print t('Groups'); ?>:</span> <?php print $og_groups; ?>
         </div>
       <?php endif; ?>      
    </div>
  </div>
</div>

