<?php
function _template_preprocess_advanced_forum_topic_list_view(&$vars) {
  $vars['template_files'][] = 'advanced_forum-topic-list-view';

  $view     = $vars['view'];
  // We assume that the forum this is for will *always* be the first argument.
  // This style will totally break if that's not true. We don't really care that much,
  // though the object could probably run a validate().
  $forum = $view->args[0];

  // We need the raw data for this grouping, which is passed in as $vars['rows'].
  // However, the template also needs to use for the rendered fields.  We
  // therefore swap the raw data out to a new variable and reset $vars['rows']
  // so that it can get rebuilt.
  $result   = $vars['rows'];
  $vars['rows'] = array();

  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;

  $fields   = &$view->field;
  $columns  = $handler->sanitize_columns($options['columns'], $fields);

  $active   = !empty($handler->active) ? $handler->active : '';
  $order    = !empty($handler->order) ? $handler->order : 'asc';

  $query    = tablesort_get_querystring();
  if ($query) {
    $query = '&' . $query;
  }

  // Fields must be rendered in order as of Views 2.3, so we will pre-render
  // everything.
  $renders = array();
  $keys = array_keys($view->field);
  $stickies_done = FALSE;
  $sticky_count = 0;

  foreach ($result as $count => $row) {
    foreach ($keys as $id) {
      $renders[$count][$id] = $view->field[$id]->theme($row);
    }
    // Determine stickiness.
    if (!$stickies_done) {
      $sticky[$count] = !empty($row->topic_is_sticky);
      if ($sticky[$count]) {
        $sticky_count++;
        $vars['row_classes'][$count][] = 'sticky';
      }
      else {
        $stickies_done = TRUE;
        if ($sticky_count) {
          // This ends up appearing only if we have both sticky and non sticky posts.
          // Otherwise it will never show up.
          $vars['row_classes'][$count - 1][] = 'forum-last-sticky';
        }
      }
    }

    $vars['shadow'][$count] = $forum != $row->topic_actual_forum;
  }

  foreach ($columns as $field => $column) {
    // render the header labels
    if ($field == $column && empty($fields[$field]->options['exclude'])) {
      $label = check_plain(!empty($fields[$field]) ? $fields[$field]->label() : '');
      if (empty($options['info'][$field]['sortable']) || !$fields[$field]->click_sortable()) {
        $vars['header'][$field] = $label;
      }
      else {
        // @todo -- make this a setting
        $initial = 'asc';

        if ($active == $field && $order == 'asc') {
          $initial = 'desc';
        }

        $title = t('sort by @s', array('@s' => $label));
        if ($active == $field) {
          $label .= theme('tablesort_indicator', $initial);
        }
        $link_options = array(
          'html' => true,
          'attributes' => array('title' => $title),
          'query' => 'order=' . urlencode($field) . '&sort=' . $initial . $query,
        );
        $vars['header'][$field] = l($label, $_GET['q'], $link_options);
      }
    }

    // Create a second variable so we can easily find what fields we have and what the
    // CSS classes should be.
    $vars['fields'][$field] = views_css_safe($field);
    if ($active == $field) {
      $vars['fields'][$field] .= ' active';
    }

    // Render each field into its appropriate column.
    foreach ($result as $num => $row) {
      if (!empty($fields[$field]) && empty($fields[$field]->options['exclude'])) {
        $field_output = $renders[$num][$field];

        // Don't bother with separators and stuff if the field does not show up.
        if (empty($field_output) && !empty($vars['rows'][$num][$column])) {
          continue;
        }

        // Place the field into the column, along with an optional separator.
        if (!empty($vars['rows'][$num][$column])) {
          if (!empty($options['info'][$column]['separator'])) {
            $vars['rows'][$num][$column] .= filter_xss_admin($options['info'][$column]['separator']);
          }
        }
        else {
          $vars['rows'][$num][$column] = '';
        }

        $vars['rows'][$num][$column] .= $field_output;
      }
    }
  }

  foreach ($vars['rows'] as $num => $row) {
    $vars['row_classes'][$num][] = ($num % 2 == 0) ? 'odd' : 'even';
  }

  $vars['class'] = 'views-table';
  if (!empty($options['sticky'])) {
    drupal_add_js('misc/tableheader.js');
    $vars['class'] .= " sticky-enabled";
  }
}
