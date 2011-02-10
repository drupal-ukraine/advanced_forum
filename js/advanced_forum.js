(function ($) {
  Drupal.advanced_forum = Drupal.advanced_forum || {};

  Drupal.behaviors.advanced_forum = {
    attach: function(context) {
      // Retrieve the collapsed status from a stored cookie.
      // cookie format is: page1=1,2,3/page2=1,4,5/page3=5,6,1...
      var cookie = $.cookie('Drupal.advanced_forum.collapsed');
      var pages = cookie ? cookie.split('/') : new Array();
      // Create associative array where key=page path and value=comma-separated list of collapsed forum ids
      Drupal.advanced_forum.collapsed_page = new Array();
      if (pages) {
        for (x in pages) {
          tmp = pages[x].split('=');
          Drupal.advanced_forum.collapsed_page[tmp[0]] = tmp[1].split(',');
        }
      }

      // Get data for current page
      Drupal.advanced_forum.collapsed_current = Drupal.advanced_forum.collapsed_page[encodeURIComponent(window.location.pathname)];
      if (!Drupal.advanced_forum.collapsed_current)
        Drupal.advanced_forum.collapsed_current = new Array();

      //var handleHeader = $('thead.forum-header th:last-child', context);
      var handleSpan = $('span.forum-collapsible', context);

      // Set initial collapsed state
      handleSpan.once('forum-collapsible', Drupal.advanced_forum.init);
      //handleHeader.once('forum-header', Drupal.advanced_forum.init);
      handleSpan.addClass('clickable').click(function() {
        // Get forum id
        var id = $(this).attr('id').split('-')[2];
        if ( $(this).hasClass('container-collapsed')) {
          Drupal.advanced_forum.expand(id);
          // Reset collapsed status
          Drupal.advanced_forum.collapsed_current.splice(Drupal.advanced_forum.collapsed_current.indexOf(id),1);
        }
        else {
          Drupal.advanced_forum.collapse(id);
          // Set collapsed status
          Drupal.advanced_forum.collapsed_current.push(id);
        }

        // Put status back
        Drupal.advanced_forum.collapsed_page[encodeURIComponent(window.location.pathname)] = Drupal.advanced_forum.collapsed_current;

        // Build cookie string
        cookie = '';
        for(x in Drupal.advanced_forum.collapsed_page) {
          cookie += '/' + x + '=' + Drupal.advanced_forum.collapsed_page[x];
        }
        // Save new cookie
        $.cookie(
          'Drupal.advanced_forum.collapsed',
          cookie.substr(1),
          {
            path: '/',
            // The cookie should "never" expire.
            expires: 36500
          }
        );
      });
    }
  };

  /**
   * Initialize and set collapsible status
   */
  Drupal.advanced_forum.init = function() {
    // get forum id
    var id = $(this).attr('id').split('-')[2];

    // Check if item is collapsed
    if (Drupal.advanced_forum.collapsed_current.indexOf(id) != -1) {
      $(this).addClass('container-collapsed');
      Drupal.advanced_forum.collapse(id);
      return;
    }

    $(this).removeClass('container-collapsed');
    Drupal.advanced_forum.expand(id);
  };

  Drupal.advanced_forum.collapse = function(id) {
    element = $('#forum-collapsible-' + id).addClass('container-collapsed').closest('table').children('tbody');
    if (Drupal.settings.advanced_forum.effect == 'fade') {
      $(element).fadeOut('fast');
    }
    else if (Drupal.settings.advanced_forum.effect == 'slide') {
      $(element).slideUp('fast');
    }
    else {
      $(element).hide();
    }
  };

  Drupal.advanced_forum.expand = function(id) {
    element = $('#forum-collapsible-' + id).removeClass('container-collapsed').closest('table').children('tbody');
    if (Drupal.settings.advanced_forum.effect == 'fade') {
      $(element).fadeIn('fast');
    }
    else if (Drupal.settings.advanced_forum.effect == 'slide') {
      $(element).slideDown('fast');
    }
    else {
      $(element).show();
    }
  };

})(jQuery);
