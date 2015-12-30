<?php
/**
 * @file
 * Admin settings form.
 */

namespace Drupal\advanced_forum\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Symfony\Component\HttpFoundation\Request;

/**
 * Class AdminSettingsForm. Defines a form that configures advanced forum settings.
 *
 * @package Drupal\advanced_forum\Form
 */
class AdminSettingsForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'advanced_forum_admin_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return [
      'advanced_forum_general',
      'advanced_forum_lists',
      'advanced_forum_forum_image',
      'advanced_forum_autoload',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state, Request $request = NULL) {


    // General settings.
    $form['advanced_forum_general'] = array(
      '#type' => 'fieldset',
      '#title' => t('General'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    // Choose style.
    $options = array();
    // @d7 $available_styles = advanced_forum_get_all_styles();
    $available_styles = [
      'test_style' => [
        'name' => t('Test style name')
      ],
      'test_style2' => [
        'name' => t('Test style name 2')
      ]
    ];
    foreach ($available_styles as $style_machine_name => $style) {
      $options[$style_machine_name] = $style['name'];
    }
    asort($options);

    $form['advanced_forum_general']['advanced_forum_style'] = array(
      '#type' => 'select',
      '#title' => t('Advanced forum style'),
      '#options' => $options,
      '#description' => t('Choose which style to use for your forums. This will apply independent of site theme.'),
      // '#default_value' => variable_get('advanced_forum_style', 'silver_bells'),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_style'),
    );

    // Choose node types that are styled.
    // @d7 $node_types = _node_types_build()->types;
    $nt = new \stdClass();
    $nt->name = t('Test node type Name 1');
    $node_types = [
      'node_type_test' => $nt,
    ];
    $options = array();
    foreach ($node_types as $node_machine_name => $node_type) {
      $options[$node_machine_name] = $node_type->name;
    }

    $form['advanced_forum_general']['advanced_forum_styled_node_types'] = array(
      '#type' => 'select',
      '#title' => t('Node types to style'),
      '#options' => $options,
      '#multiple' => TRUE,
      '#description' => t('Choose which node types will have the forum style applied.'),
      // @d7 '#default_value' => variable_get('advanced_forum_styled_node_types', array('forum')),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_styled_node_types'),
    );

    // Style nodes presented in teaser form.
    $form['advanced_forum_general']['advanced_forum_style_teasers'] = array(
      '#type' => 'checkbox',
      '#title' => t('Style nodes when being displayed as teasers.'),
      // @d7 '#default_value' => variable_get('advanced_forum_style_teasers', 0),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_style_teasers'),
      '#description' => t('If checked, selected node types will be styled even when they are in a teaser list.'),
    );

    // Style nodes only if tagged for the forum.
    $form['advanced_forum_general']['advanced_forum_style_only_forum_tagged'] = array(
      '#type' => 'checkbox',
      '#title' => t('Style nodes only if they have a forum term attached.'),
      // @d7 '#default_value' => variable_get('advanced_forum_style_only_forum_tagged', 1),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_style_only_forum_tagged'),
      '#description' => t('If checked, selected node types will be only styled if they are associated with a forum term.'),
    );

    // Style all site comments as forums.
    $form['advanced_forum_general']['advanced_forum_style_all_comments'] = array(
      '#type' => 'checkbox',
      '#title' => t('Style all comments like forum replies.'),
      // @d7 '#default_value' => variable_get('advanced_forum_style_all_comments', 0),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_style_all_comments'),
      '#description' => t('If checked, every comment will be styled as if it were a forum reply.'),
    );

    $form['advanced_forum_general']['advanced_forum_add_local_task'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add a tab for forum view page.'),
      // @d7 '#default_value' => variable_get('advanced_forum_add_local_task', TRUE),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_add_local_task'),
      '#description' => t('If checked, this will add a local task tab for "View forums". Use this in conjunction with making the included views have local tasks. If you don\'t know what this means, leave it unchecked. You must clear the cache before this will take effect.'),
    );

    $form['advanced_forum_general']['advanced_forum_views_as_tabs'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add a tab for included views that have their own pages.'),
      // @d7 '#default_value' => variable_get('advanced_forum_views_as_tabs', TRUE),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_views_as_tabs'),
      '#description' => t('If checked, this will add a local task tab for "Active topics," "New posts," "My posts," and "Unanswered topics." If you don\'t know what this means, leave it unchecked. You must clear the cache before this will take effect.'),
    );

    $form['advanced_forum_general']['advanced_forum_keep_classes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Keep default CSS classes (For experts only).'),
      // @d7 '#default_value' => variable_get('advanced_forum_keep_classes', FALSE),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_keep_classes'),
      '#description' => t('If checked, all CSS classes generated by Drupal core and other modules/themes will be kept'),
    );

    $form['advanced_forum_general']['advanced_forum_forum_user_term_fields'] = array(
      '#title' => t('Use fields from taxonomy term in forum'),
      '#type' => 'checkbox',
      '#description' => t('Allows to use fields from taxonomy term on the form of creation or editing of the forum'),
      // @d7 '#default_value' => variable_get('advanced_forum_forum_user_term_fields'),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_forum_user_term_fields'),
    );

    // Forum 'topic list settings'.
    $form['advanced_forum_lists'] = array(
      '#type' => 'fieldset',
      '#title' => t('Forum and topic lists'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    // Disable breadcrumbs.
    $form['advanced_forum_lists']['advanced_forum_disable_breadcrumbs'] = array(
      '#type' => 'checkbox',
      '#title' => t('Disable breadcrumbs'),
      // @d7 '#default_value' => variable_get('advanced_forum_disable_breadcrumbs', 0),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_disable_breadcrumbs'),
      '#description' => t('Check this to disable breadcrumbs in the forum if you are using another module to customize them. Does not affect node pages and does not work when Page Manager is overriding forum pages.'),
    );

    $form['advanced_forum_lists']['advanced_forum_collapsible_containers'] = array(
      '#type' => 'select',
      '#title' => t('Collapsible forum containers'),
      '#options' => array(
        'none' => t("None"),
        'toggle' => t("Toggle"),
        'fade' => t("Fade"),
        'slide' => t("Slide"),
      ),
      '#description' => t('Select whether or not to enable collapsible forum containers and what type of animation to use.'),
      // @d7 '#default_value' => variable_get('advanced_forum_collapsible_containers', 'toggle'),
      '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_collapsible_containers'),
    );

    // For default collapsed state configuration.
    // @d7 $collapsed_list_name = variable_get('forum_containers', array());
    $collapsed_list_name = $this->config('advanced_forum_general')->get('forum_containers') ? $this->config('advanced_forum_general')->get('forum_containers') : array();
    $collapsed_list_description = array();
    foreach ($collapsed_list_name as $id) {
      $term = taxonomy_term_load($id);
      if (!empty($term)) {
        $collapsed_list_description[$id] = empty($term->name) ? '' : $term->name;
      }
    }
    $form['advanced_forum_lists']['advanced_forum_default_collapsed_list'] = array(
      '#type' => 'select',
      '#title' => t('Containers collapsed by default'),
      // @d7 '#default_value' => variable_get('advanced_forum_default_collapsed_list', array()),
      '#default_value' => $this->config('advanced_forum_lists')->get('advanced_forum_default_collapsed_list'),
      '#options' => $collapsed_list_description,
      '#multiple' => TRUE,
      '#description' => t('Select containers which should be collapsed by default.'),
    );

    if (\Drupal::moduleHandler()->moduleExists('image')) {

      // Forum image settings.
      $form['advanced_forum_forum_image'] = array(
        '#type' => 'fieldset',
        '#title' => t('Forum image settings'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      // @d7 $forum_vocabulary = taxonomy_vocabulary_load(variable_get('forum_nav_vocabulary', 0));
      $forum_vocabulary = taxonomy_vocabulary_load($this->config('advanced_forum_general')->get('forum_nav_vocabulary'));

      // @todo find replacement.
      // @d7 $field_info = field_info_instances('taxonomy_term', $forum_vocabulary->machine_name);
      $image_fields = array();
      $field_info = [
        'field_bundle_with_image' => [
          'test_field_with_image' => ['display' => ['default' => ['type' => 'image']]]
        ]
      ];
      foreach ($field_info as $bundle => $field) {
        if (!empty($field['display']['default']['type']) && ($field['display']['default']['type'] == 'image')) {
          $image_fields[$bundle] = $bundle;
        }
      }
      $form['advanced_forum_forum_image']['advanced_forum_forum_image_field'] = array(
        '#title' => t('Image field'),
        '#description' => t('The image field to use to display forum images.'),
        '#type' => 'select',
        // @d7 '#default_value' => variable_get('advanced_forum_forum_image_field', ''),
        '#default_value' => $this->config('advanced_forum_forum_image')->get('advanced_forum_forum_image_field'),
        '#empty_option' => t('None'),
        '#options' => $image_fields,
      );

      $form['advanced_forum_forum_image']['advanced_forum_forum_image_preset'] = array(
        '#title' => t('Forum image style'),
        '#description' => t('The image style to apply to the images.'),
        '#type' => 'select',
        // @d7 '#default_value' => variable_get('advanced_forum_forum_image_preset', ''),
        '#default_value' => $this->config('advanced_forum_forum_image')->get('advanced_forum_forum_image_preset'),
        '#empty_option' => t('None (original image)'),
        '#options' => image_style_options(FALSE),
      );
    }
    else {
      $this->config('advanced_forum_forum_image')->set('advanced_forum_forum_image_field', '');
    }
    // Picture preset.
    if (\Drupal::moduleHandler()->moduleExists('image') && \Drupal::moduleHandler()->moduleExists('author_pane')) {
      $options = array('' => '');
      $styles = image_styles();
      foreach ($styles as $style) {
        $options[$style['name']] = $style['name'];
      }

      $form['advanced_forum_general']['advanced_forum_user_picture_preset'] = array(
        '#type' => 'select',
        '#title' => t('User picture preset'),
        '#options' => $options,
        '#description' => t('Image preset to use for forum avatars. Leave blank to not use this feature.'),
        // @d7 '#default_value' => variable_get('advanced_forum_user_picture_preset', ''),
        '#default_value' => $this->config('advanced_forum_general')->get('advanced_forum_user_picture_preset'),
      );
    }
    else {
      $this->config('advanced_forum_general')->set('advanced_forum_user_picture_preset', '');
    }

    if (\Drupal::moduleHandler()->moduleExists('author_pane')) {
      $join_date_options = array();
      foreach (system_get_date_types() as $date_type) {
        $join_date_options[$date_type['type']] = $date_type['title'];
      }

      $form['advanced_forum_general']['advanced_forum_author_pane_join_date_type'] = array(
        '#type' => 'select',
        '#title' => t('Author Pane - Join date, date type'),
        '#options' => $join_date_options,
        '#description' => t('Select which <a href="@date-type-url">date type</a> to use for displaying the join date in the Author Pane.', array('@date-type-url' => url('admin/config/regional/date-time'))),
        '#default_value' => variable_get('advanced_forum_author_pane_join_date_type', 'short'),
      );
    }

    // Retrieve new comments on forum listing.
    $form['advanced_forum_lists']['advanced_forum_get_new_comments'] = array(
      '#type' => 'checkbox',
      '#title' => t('Get the number of new comments per forum on the forum list'),
      // @d7 '#default_value' => variable_get('advanced_forum_get_new_comments', 0),
      '#default_value' => $this->config('advanced_forum_lists')->get('advanced_forum_get_new_comments'),
      '#description' => t('Core forum shows the number of new topics. If checked, Advanced Forum will get the number of new comments as well and show it under "posts" on the forum overview. Slow query not recommended on large forums.'),
    );

    // Title length max.
    $form['advanced_forum_lists']['advanced_forum_topic_title_length'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of characters to display for the topic title'),
      '#size' => 5,
      '#description' => t('Used on main forum page. Enter 0 to use the full title.'),
      // @d7 '#default_value' => variable_get('advanced_forum_topic_title_length', 20),
      '#default_value' => $this->config('advanced_forum_lists')->get('advanced_forum_topic_title_length'),
    );

    // Last Post optimization.
    $form['advanced_forum_lists']['advanced_forum_last_post_query'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of last topics which will be used in search for "Last Post"'),
      '#size' => 5,
      '#description' => t('Optimization for large sites. Used on main forum page.'),
      // @d7 '#default_value' => variable_get('advanced_forum_last_post_query', 10000),
      '#default_value' => $this->config('advanced_forum_lists')->get('advanced_forum_last_post_query'),
    );

    // Auto loading.
    $form['advanced_forum_autoload'] = array(
      '#type' => 'fieldset',
      '#title' => t('Auto-loading'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('Changing these settings may require multiple cache clears to take effect.'),
    );

    // Auto load included page variants.
    $form['advanced_forum_autoload']['advanced_forum_autoload_page_handlers'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto load included page handlers'),
      '#description' => t('Uncheck to use Features or manual imports.'),
      // @d7 '#default_value' => variable_get('advanced_forum_autoload_page_handlers', TRUE),
      '#default_value' => $this->config('advanced_forum_autoload')->get('advanced_forum_autoload_page_handlers'),
    );

    // Auto load included views.
    $form['advanced_forum_autoload']['advanced_forum_autoload_views'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto load included views'),
      '#description' => t('Uncheck to use Features or manual imports.'),
      // @d7 '#default_value' => variable_get('advanced_forum_autoload_views', TRUE),
      '#default_value' => $this->config('advanced_forum_autoload')->get('advanced_forum_autoload_views'),
    );

    // Send our form to Drupal to make a settings page.
    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    // @todo implement submit handler.
  }

}
