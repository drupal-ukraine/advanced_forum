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
  <div id="forum" class="<?php print $language_class; ?>">
    <?php if ($search): ?>
      <div id="search-all-forums"><?php print $search; ?></div>
    <?php endif; ?>

    <?php print $forums; ?>

    <?php if (!empty($forum_tools)): ?>
      <div class="forum-tools"><?php print $forum_tools; ?></div>
    <?php endif; ?>

    <?php print $topics; ?>

    <?php if (!empty($topics)): ?>
      <?php print $topic_legend; ?>
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
