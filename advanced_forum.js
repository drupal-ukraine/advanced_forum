(function ($) {
  Drupal.behaviors.advanced_forum = {
    attach: function(context) {
      var expand = Drupal.settings.basePath + Drupal.settings.advanced_forum.modulePath + '/styles/naked/images/container_expand.png';
      var collapse = Drupal.settings.basePath + Drupal.settings.advanced_forum.modulePath + '/styles/naked/images/container_collapse.png';
      var handleHeader = $('thead th:first-child', context);

      handleHeader.prepend('<img style="margin-right: 10px;" src="' + collapse + '"alt="Toggle" />');
      $('img', handleHeader).addClass('clickable').click(function() {
        var imageSource = $(this).attr('src');
        if ( imageSource == collapse ) {
          if (Drupal.settings.advanced_forum.effect == 'toggle') {
            $(this).attr('src', expand).closest('table').children('tbody').hide();
          } else {
            $(this).attr('src', expand).closest('table').children('tbody').fadeOut();
          }
        } else {
          if (Drupal.settings.advanced_forum.effect == 'toggle') {
            $(this).attr('src', collapse).closest('table').children('tbody').show();
          } else {
            $(this).attr('src', collapse).closest('table').children('tbody').fadeIn();
          }
        };
      });
    }
  };
})(jQuery);