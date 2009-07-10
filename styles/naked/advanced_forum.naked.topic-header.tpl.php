<div class="<?php print $classes; ?>">
  <a id="top"></a>

  <?php print $reply_link; ?>

  <?php print $search; ?>

  <div class="reply-count">
    <?php print $total_posts; ?>

    <?php if (!empty($new_posts)): ?>
      [<?php print $new_posts; ?>]
    <?php endif; ?>

    <?php if (!empty($last_post)): ?>
       [<?php print $last_post; ?>]
    <?php endif; ?>
  </div>
</div>
