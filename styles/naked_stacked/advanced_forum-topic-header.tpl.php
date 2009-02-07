<div class="forum-topic-header clear-block">
  <a id="top"></a>

  <?php print $reply_link; ?>

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
