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

  function testRouteGeneration() {
    // @TODO remove after https://www.drupal.org/node/2431263 is solved.
    $this->container->get('module_installer')->install(array('kint'));

    // Test Devel load and render routes for entities with both route
    // definitions.
    $this->drupalGet('admin/config/content/advanced-forum');
    $this->assertText('Devel', 'Devel tab is present');

  }

}
