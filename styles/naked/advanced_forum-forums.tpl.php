<?php
// $Id$

/**
 * @file
 * Default theme implementation to display a forum which may contain forum
 * containers as well as forum topics.
 *
 * Variables available:
 * - $forum_links: An array of links that allow a user to post new forum topics.
 *   It may also contain a string telling a user they must log in in order
 *   to post. Empty if there are no topics on the page. (ie: forum overview)
 * - $forum_links_orig: Same as $forum_links but not emptied on forum overview page.
 * - $forums: The forums to display (as processed by forum-list.tpl.php)
 * - $topics: The topics to display (as processed by forum-topic-list.tpl.php)
 * - $forums_defined: A flag to indicate that the forums are configured.
 *
 * @see template_preprocess_forums()
 * @see advanced_forum_preprocess_forums()
 */
?>

<?php if ($forums_defined): ?>
  <div id="forum">
    <?php if ($search): ?>
      <div id="search-all-forums"><?php print $search; ?></div>
    <?php endif; ?>

    <?php if (empty($topics)): ?>
      <?php // We only want these printed on top on the main forum page ?>
      <div class="forum-secondary-links"><?php print $forum_secondary_links ?></div>
    <?php endif; ?>
    
    <?php print $forums; ?>
    
    <?php if (!empty($topics)): ?>
      <?php // Print a set on top of the topics. ?>
      <div class="forum-primary-links"><?php print $forum_links ?></div>
      <div class="forum-secondary-links clear-block"><?php print $forum_secondary_links ?></div>
    <?php endif; ?>
    
    <?php print $topics; ?>
    
    <?php if (!empty($topics)): ?>
      <?php // Print a set under the topics. ?>
      <div class="forum-primary-links"><?php print $forum_links ?></div>
      <div class="forum-secondary-links clear-block"><?php print $forum_secondary_links ?></div>
    <?php endif; ?>

    <?php if (empty($topics) && !empty($forum_legend)): ?>
      <?php // Only print legend on pages with no topics to avoid clutter. ?>
      <?php print $forum_legend; ?>
    <?php endif; ?>

     <?php if (arg(1) == '' && !empty($forum_statistics)): ?>
       <?php // Only print stats on main forum page. ?>
       <?php print $forum_statistics; ?>
     <?php endif; ?>
  </div>
<?php endif; ?>
