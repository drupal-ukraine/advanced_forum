<?php
// $Id$

/**
 * @file
 * Default theme implementation to display a forum which may contain forum
 * containers as well as forum topics.
 *
 * Variables available:
 * - $links: An array of links that allow a user to post new forum topics.
 *   It may also contain a string telling a user they must log in in order
 *   to post. Empty if there are no topics on the page. (ie: forum overview)
 * - $links_orig: Same as $links but not emptied on forum overview page.
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
  <?php if (empty($topics)): ?>
    <div class="forum-links-no-topics"><?php print $links_rendered ?></div>
  <?php endif; ?>
  
   <?php if ($search): ?>
    <div id="search-all-forums"><?php print $search; ?></div>
  <?php endif; ?>

  <?php print $forums; ?>
  
  <?php if (!empty($topics)): ?>
    <div class="forum-links-top clear-block"><?php print $links_rendered ?></div>
  <?php endif; ?>
  
  <?php print $topics; ?>
  
  <?php if (!empty($topics)): ?>
    <div class="forum-links-bottom clear-block"><?php print $links_rendered ?></div>
  <?php endif; ?>
</div>
<?php endif; ?>
