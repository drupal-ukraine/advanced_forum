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

    $web_user = $this->drupalCreateUser([
      'access administration pages',
      'administer forums',
      'administer advanced forum',
    ]);
    $this->drupalLogin($web_user);
  }

  /**
   * Testing Advanced Forum admin pages.
   *
   * @throws \Exception
   *   Exceptions if tests failed.
   */
  function testAdminForm() {
    // Check menu item.
    $this->drupalGet('/admin/config/content');
    $this->assertText('Advanced Forum', 'Advanced Forum menu item is present');

    // Check configuration form.
    $this->drupalGet('/admin/config/content/advanced-forum');
    $this->assertText('Advanced forum style', 'Advanced forum style option is present');
    $this->drupalPostForm('/admin/config/content/advanced-forum', [], t('Save configuration'));
  }
}
