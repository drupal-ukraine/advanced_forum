<?php

/**
 * @file
 * Contains \Drupal\advanced_forum\Form\AdminSettingsForm.
 */

namespace Drupal\advanced_forum\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Symfony\Component\HttpFoundation\Request;
use Drupal\Component\Utility\String;
use Drupal\Core\Datetime\Entity\DateFormat;

/**
 * Defines a form that configures advanced forum settings.
 *
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
      'advanced_forum_general.settings',
      'advanced_forum_lists.settings',
      'advanced_forum_forum_image.settings',
      'advanced_forum_autoload.settings',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state, Request $request = NULL) {

    $advanced_forum_general = $this->config('advanced_forum_general.settings');
    $advanced_forum_lists = $this->config('advanced_forum_lists.settings');
    $advanced_forum_forum_image = $this->config('advanced_forum_forum_image.settings');
    $advanced_forum_autoload = $this->config('advanced_forum_autoload.settings');

    // General settings.
    $form['advanced_forum_general'] = [
      '#type' => 'fieldset',
      '#title' => $this->t('General'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    ];

    // Choose style.
    $options = [];
    // @d7 @todo $available_styles = advanced_forum_get_all_styles();
    $available_styles = [
      'test_style' => [
        'name' => $this->t('Test style name'),
      ],
      'test_style2' => [
        'name' => $this->t('Test style name 2'),
      ],
    ];
    foreach ($available_styles as $style_machine_name => $style) {
      $options[$style_machine_name] = $style['name'];
    }
    asort($options);

    $form['advanced_forum_general']['advanced_forum_style'] = [
      '#type' => 'select',
      '#title' => $this->t('Advanced forum style'),
      '#options' => $options,
      '#description' => $this->t('Choose which style to use for your forums. This will apply independent of site theme.'),
      // '#default_value' => variable_get('advanced_forum_style', 'silver_bells'),
      '#default_value' => $advanced_forum_general->get('advanced_forum_style'),
    ];

    // Choose node types that are styled.
    $node_type_options = [];
    $entity_type_manager = \Drupal::entityManager();
    $bundles = $entity_type_manager->getBundleInfo('node');
    foreach ($bundles as $bundle_id => $bundle) {
      $node_type_options[$bundle_id] = $bundle['label'];
    }

    $form['advanced_forum_general']['advanced_forum_styled_node_types'] = [
      '#type' => 'select',
      '#title' => $this->t('Node types to style'),
      '#options' => $node_type_options,
      '#multiple' => TRUE,
      '#description' => $this->t('Choose which node types will have the forum style applied.'),
      // @d7 '#default_value' => variable_get('advanced_forum_styled_node_types', array('forum')),
      '#default_value' => $advanced_forum_general->get('advanced_forum_styled_node_types'),
    ];

    // Style nodes presented in teaser form.
    $form['advanced_forum_general']['advanced_forum_style_teasers'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Style nodes when being displayed as teasers.'),
      '#default_value' => $advanced_forum_general->get('advanced_forum_style_teasers'),
      '#description' => $this->t('If checked, selected node types will be styled even when they are in a teaser list.'),
    ];

    // Style nodes only if tagged for the forum.
    $form['advanced_forum_general']['advanced_forum_style_only_forum_tagged'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Style nodes only if they have a forum term attached.'),
      '#default_value' => $advanced_forum_general->get('advanced_forum_style_only_forum_tagged'),
      '#description' => $this->t('If checked, selected node types will be only styled if they are associated with a forum term.'),
    ];

    // Style all site comments as forums.
    $form['advanced_forum_general']['advanced_forum_style_all_comments'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Style all comments like forum replies.'),
      '#default_value' => $advanced_forum_general->get('advanced_forum_style_all_comments'),
      '#description' => $this->t('If checked, every comment will be styled as if it were a forum reply.'),
    ];

    $form['advanced_forum_general']['advanced_forum_add_local_task'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Add a tab for forum view page.'),
      '#default_value' => $advanced_forum_general->get('advanced_forum_add_local_task'),
      '#description' => $this->t('If checked, this will add a local task tab for "View forums". Use this in conjunction with making the included views have local tasks. If you don\'t know what this means, leave it unchecked. You must clear the cache before this will take effect.'),
    ];

    $form['advanced_forum_general']['advanced_forum_views_as_tabs'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Add a tab for included views that have their own pages.'),
      '#default_value' => $advanced_forum_general->get('advanced_forum_views_as_tabs'),
      '#description' => $this->t('If checked, this will add a local task tab for "Active topics," "New posts," "My posts," and "Unanswered topics." If you don\'t know what this means, leave it unchecked. You must clear the cache before this will take effect.'),
    ];

    $form['advanced_forum_general']['advanced_forum_keep_classes'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Keep default CSS classes (For experts only).'),
      '#default_value' => $advanced_forum_general->get('advanced_forum_keep_classes'),
      '#description' => $this->t('If checked, all CSS classes generated by Drupal core and other modules/themes will be kept'),
    ];

    $form['advanced_forum_general']['advanced_forum_forum_user_term_fields'] = [
      '#title' => $this->t('Use fields from taxonomy term in forum'),
      '#type' => 'checkbox',
      '#description' => $this->t('Allows to use fields from taxonomy term on the form of creation or editing of the forum'),
      // @d7 '#default_value' => variable_get('advanced_forum_forum_user_term_fields'),
      '#default_value' => $advanced_forum_general->get('advanced_forum_forum_user_term_fields'),
    ];

    // Forum 'topic list settings'.
    $form['advanced_forum_lists'] = [
      '#type' => 'fieldset',
      '#title' => $this->t('Forum and topic lists'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    ];

    // Disable breadcrumbs.
    $form['advanced_forum_lists']['advanced_forum_disable_breadcrumbs'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Disable breadcrumbs'),
      '#default_value' => $advanced_forum_general->get('advanced_forum_disable_breadcrumbs'),
      '#description' => $this->t('Check this to disable breadcrumbs in the forum if you are using another module to customize them. Does not affect node pages and does not work when Page Manager is overriding forum pages.'),
    ];

    $form['advanced_forum_lists']['advanced_forum_collapsible_containers'] = [
      '#type' => 'select',
      '#title' => $this->t('Collapsible forum containers'),
      '#options' => [
        'none' => $this->t("None"),
        'toggle' => $this->t("Toggle"),
        'fade' => $this->t("Fade"),
        'slide' => $this->t("Slide"),
      ],
      '#description' => $this->t('Select whether or not to enable collapsible forum containers and what type of animation to use.'),
      '#default_value' => $advanced_forum_lists->get('advanced_forum_collapsible_containers'),
    ];

    // For default collapsed state configuration.
    // @d7 @todo $collapsed_list_name = variable_get('forum_containers', array());
    $collapsed_list_name = $advanced_forum_general->get('forum_containers') ? $advanced_forum_general->get('forum_containers') : [];
    $collapsed_list_description = [];
    foreach ($collapsed_list_name as $id) {
      $term = taxonomy_term_load($id);
      if (!empty($term)) {
        $collapsed_list_description[$id] = empty($term->name) ? '' : $term->name;
      }
    }
    $form['advanced_forum_lists']['advanced_forum_default_collapsed_list'] = [
      '#type' => 'select',
      '#title' => $this->t('Containers collapsed by default'),
      // @d7 @todo '#default_value' => variable_get('advanced_forum_default_collapsed_list', array()),
      '#default_value' => $advanced_forum_lists->get('advanced_forum_default_collapsed_list'),
      '#options' => $collapsed_list_description,
      '#multiple' => TRUE,
      '#description' => $this->t('Select containers which should be collapsed by default.'),
    ];
    // @todo check this code exists in D8.
    if (\Drupal::moduleHandler()->moduleExists('image')) {

      // Forum image settings.
      $form['advanced_forum_forum_image'] = [
        '#type' => 'fieldset',
        '#title' => $this->t('Forum image settings'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      ];

      // @d7 @todo $forum_vocabulary = taxonomy_vocabulary_load(variable_get('forum_nav_vocabulary', 0));
      $forum_vocabulary = taxonomy_vocabulary_load($advanced_forum_general->get('forum_nav_vocabulary'));
      $image_fields = [];

      if ($forum_vocabulary) {
        $bundle = $forum_vocabulary->id();
        $entity_types = \Drupal::entityManager()->getDefinitions();
        $entity_type = $entity_types['taxonomy_term'];
        $field_info = field_entity_bundle_field_info($entity_type, $bundle);

        foreach ($field_info as $bundle => $field) {
          if ($field->getType() == 'image') {
            $image_fields[$bundle] = $bundle;
          }
        }
      }

      $form['advanced_forum_forum_image']['advanced_forum_forum_image_field'] = [
        '#title' => $this->t('Image field'),
        '#description' => $this->t('The image field to use to display forum images.'),
        '#type' => 'select',
        '#default_value' => $advanced_forum_forum_image->get('advanced_forum_forum_image_field'),
        '#empty_option' => $this->t('None'),
        '#options' => $image_fields,
      ];

      $form['advanced_forum_forum_image']['advanced_forum_forum_image_preset'] = [
        '#title' => $this->t('Forum image style'),
        '#description' => $this->t('The image style to apply to the images.'),
        '#type' => 'select',
        '#default_value' => $advanced_forum_forum_image->get('advanced_forum_forum_image_preset'),
        '#empty_option' => $this->t('None (original image)'),
        '#options' => image_style_options(FALSE),
      ];
    }
    else {
      $advanced_forum_forum_image->set('advanced_forum_forum_image_field', '');
    }
    // Picture preset.
    if (\Drupal::moduleHandler()
        ->moduleExists('image') && \Drupal::moduleHandler()
        ->moduleExists('author_pane')
    ) {
      $options = ['' => ''];
      $styles = image_styles();
      foreach ($styles as $style) {
        $options[$style['name']] = $style['name'];
      }

      $form['advanced_forum_general']['advanced_forum_user_picture_preset'] = [
        '#type' => 'select',
        '#title' => $this->t('User picture preset'),
        '#options' => $options,
        '#description' => $this->t('Image preset to use for forum avatars. Leave blank to not use this feature.'),
        '#default_value' => $advanced_forum_general->get('advanced_forum_user_picture_preset'),
      ];
    }
    else {
      $advanced_forum_general->set('advanced_forum_user_picture_preset', '');
    }

    if (\Drupal::moduleHandler()->moduleExists('author_pane')) {
      $join_date_options = [];
      $date_types = DateFormat::loadMultiple();
      $date_formatter = \Drupal::service('date.formatter');
      foreach ($date_types as $machine_name => $format) {
        $title = $this->t('@name format', ['@name' => $format->get('label')]) . ': ' . $date_formatter->format(REQUEST_TIME, $machine_name);
        $join_date_options[$machine_name] = $title;
      }

      $form['advanced_forum_general']['advanced_forum_author_pane_join_date_type'] = [
        '#type' => 'select',
        '#title' => $this->t('Author Pane - Join date, date type'),
        '#options' => $join_date_options,
        '#description' => $this->t('Select which <a href=":date-type-url">date type</a> to use for displaying the join date in the Author Pane.', [
          '@date-type-url' => Url::fromRoute('entity.date_format.collection')->toString(),
        ]),
        // @D7 @todo '#default_value' => variable_get('advanced_forum_author_pane_join_date_type', 'short'),
        '#default_value' => $advanced_forum_general->get('advanced_forum_author_pane_join_date_type'),
      ];
    }

    // Retrieve new comments on forum listing.
    $form['advanced_forum_lists']['advanced_forum_get_new_comments'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Get the number of new comments per forum on the forum list'),
      '#default_value' => $advanced_forum_lists->get('advanced_forum_get_new_comments'),
      '#description' => $this->t('Core forum shows the number of new topics. If checked, Advanced Forum will get the number of new comments as well and show it under "posts" on the forum overview. Slow query not recommended on large forums.'),
    ];

    // Title length max.
    $form['advanced_forum_lists']['advanced_forum_topic_title_length'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Number of characters to display for the topic title'),
      '#size' => 5,
      '#description' => $this->t('Used on main forum page. Enter 0 to use the full title.'),
      '#default_value' => $advanced_forum_lists->get('advanced_forum_topic_title_length'),
    ];

    // Last Post optimization.
    $form['advanced_forum_lists']['advanced_forum_last_post_query'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Number of last topics which will be used in search for "Last Post"'),
      '#size' => 5,
      '#description' => $this->t('Optimization for large sites. Used on main forum page.'),
      '#default_value' => $advanced_forum_lists->get('advanced_forum_last_post_query'),
    ];

    // Auto loading.
    $form['advanced_forum_autoload'] = [
      '#type' => 'fieldset',
      '#title' => $this->t('Auto-loading'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => $this->t('Changing these settings may require multiple cache clears to take effect.'),
    ];

    // Auto load included page variants.
    $form['advanced_forum_autoload']['advanced_forum_autoload_page_handlers'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Auto load included page handlers'),
      '#description' => $this->t('Uncheck to use Features or manual imports.'),
      '#default_value' => $advanced_forum_autoload->get('advanced_forum_autoload_page_handlers'),
    ];

    // Auto load included views.
    $form['advanced_forum_autoload']['advanced_forum_autoload_views'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Auto load included views'),
      '#description' => $this->t('Uncheck to use Features or manual imports.'),
      '#default_value' => $advanced_forum_autoload->get('advanced_forum_autoload_views'),
    ];

    // Send our form to Drupal to make a settings page.
    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('advanced_forum_forum_image.settings')
      ->set('advanced_forum_forum_image_preset', $form_state->getValue('advanced_forum_forum_image_preset'))
      ->set('advanced_forum_forum_image_field', $form_state->getValue('advanced_forum_forum_image_field'))
      ->save();

    $this->config('advanced_forum_general.settings')
      ->set('advanced_forum_author_pane_join_date_type', $form_state->getValue('advanced_forum_author_pane_join_date_type'))
      ->set('advanced_forum_user_picture_preset', $form_state->getValue('advanced_forum_user_picture_preset'))
      ->set('advanced_forum_forum_user_term_fields', $form_state->getValue('advanced_forum_forum_user_term_fields'))
      ->set('advanced_forum_keep_classes', $form_state->getValue('advanced_forum_keep_classes'))
      ->set('advanced_forum_views_as_tabs', $form_state->getValue('advanced_forum_views_as_tabs'))
      ->set('advanced_forum_add_local_task', $form_state->getValue('advanced_forum_add_local_task'))
      ->set('advanced_forum_style_all_comments', $form_state->getValue('advanced_forum_style_all_comments'))
      ->set('advanced_forum_style_only_forum_tagged', $form_state->getValue('advanced_forum_style_only_forum_tagged'))
      ->set('advanced_forum_style_teasers', $form_state->getValue('advanced_forum_style_teasers'))
      ->set('advanced_forum_styled_node_types', $form_state->getValue('advanced_forum_styled_node_types'))
      ->set('advanced_forum_style', $form_state->getValue('advanced_forum_style'))
      ->save();

    $this->config('advanced_forum_lists.settings')
      ->set('advanced_forum_last_post_query', $form_state->getValue('advanced_forum_last_post_query'))
      ->set('advanced_forum_topic_title_length', $form_state->getValue('advanced_forum_topic_title_length'))
      ->set('advanced_forum_get_new_comments', $form_state->getValue('advanced_forum_get_new_comments'))
      ->set('advanced_forum_default_collapsed_list', $form_state->getValue('advanced_forum_default_collapsed_list'))
      ->set('advanced_forum_collapsible_containers', $form_state->getValue('advanced_forum_collapsible_containers'))
      ->set('advanced_forum_disable_breadcrumbs', $form_state->getValue('advanced_forum_disable_breadcrumbs'))
      ->save();

    $this->config('advanced_forum_autoload.settings')
      ->set('advanced_forum_autoload_views', $form_state->getValue('advanced_forum_autoload_views'))
      ->set('advanced_forum_autoload_page_handlers', $form_state->getValue('advanced_forum_autoload_page_handlers'))
      ->save();

    parent::submitForm($form, $form_state);
  }

}
