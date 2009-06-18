<?php
// $Id$
/**
 * @file
 * Contains the topic list style plugin.
 */

/**
 * Style plugin to render each item as a row in a table.
 *
 * @ingroup views_style_plugins
 */
class advanced_forum_plugin_style_forum_topic_list extends views_plugin_style_table {
  /**
   * Add a couple of fields to the query that we can later use. We are going to
   * specificly alias them because this style is not meant to be used on relationships.
   */
  function query() {
    $this->view->query->add_field('node', 'sticky', 'topic_is_sticky');
    $this->view->query->add_field('forum', 'tid', 'topic_actual_forum');
  }
}
