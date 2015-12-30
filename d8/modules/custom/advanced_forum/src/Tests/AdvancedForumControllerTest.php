<?php

/**
 * @file
 * Contains \Drupal\advanced_forum\Tests\AdvancedForumControllerTest.
 */

namespace Drupal\advanced_forum\Tests;

use Drupal\simpletest\WebTestBase;

/**
 * Class AdvancedForumControllerTest.
 *
 * @package Drupal\advanced_forum\Tests
 *
 * @group advanced_forum
 */
class AdvancedForumControllerTest extends WebTestBase {

  /**
   * Modules to enable.
   *
   * @var array
   */
  public static $modules = array('advanced_forum', 'forum', 'user', 'node');

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();

    $web_user = $this->drupalCreateUser(array(
      'administer forums',
      'administer advanced forum',
    ));
    $this->drupalLogin($web_user);
  }

  function testAdminForm() {

    // Test Devel load and render routes for entities with both route
    // definitions.
    $this->drupalGet('/admin/config/advanced-forum');
    $this->assertText('Advanced forum style', 'Advanced forum style option is present');
    $this->drupalPostForm('/admin/config/advanced-forum', [], t('Save configuration'));
  }

}
