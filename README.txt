$Id$

ADVANCED FORUM
http://drupal.org/project/advanced_forum

DESCRIPTION:
Advanced forum is a glue/theme module which makes it easier for non programmers to put together a forum similar to popular forum packages using Drupal's core forum. It requres the forum module.

INSTALLATION:
See http://drupal.org/node/227108 for the most up to date instructions. A (possibly out of date) copy is provided here for reference.

Make sure the core forum module is enabled and you have forums and (optionally) containers set up.
Copy the entire advanced_forum project directory (not just the contents) to your normal module directory (ie: sites/all/modules)
[Drupal 5.x only] In the themes directory of the module is a directory called "advforum" that contains all the theme files. Copy this entire advforum directory (not just the contents) to your theme's directory. If you use more than one theme with forums, copy it to each theme's directory. "advforum" should become a subdirectory in your theme.
Enable the advanced forum module at ?q=admin/build/modules
[Drupal 5.x only] Add this code to the top of _phptemplate_variables in template.php in your theme:
<?php
  if (module_exists('advanced_forum')) {
    $vars = advanced_forum_addvars($hook, $vars);
  }
?>

Full explanation here: http://drupal.org/node/207841
Adjust all the settings pages (this is for 5.x; 6.x is similar) 

Go to ?q=admin/content/comment/settings
Set Default display mode: Flat list - expanded (Note: You can set it to threaded but will need to adjust your theme to handle it as advforum is designed for flat forums.) 
Default display order: Date - oldest first 
The rest however you want 

Go to ?q=admin/user/settings
Set a default photo if you wish 

Go to ?q=admin/content/forum/settings
Set the topic list to display how you want

CREDITS:
Advanced forum was originally based on flatforum ( http://drupal.org/project/flatforum ). 

The mark all functionality is based on http://wtanaka.com/drupal/markasread

Topic icons by yoroy http://drupal.org/user/41502

Theming help from:
eigentor http://drupal.org/user/96718
jacine http://drupal.org/user/88931 (maintainer of the Sky theme)

Thanks to everyone who has helped by providing testing, patches, design help, etc. If you feel you've been left out and wish to be mentioned here, please let me know. This module pulls together the work of a lot of people and I want to be sure everyone gets the credit they deserve.

MAINTAINER:
Advanced forum is maintained by Michelle Cox ( http://drupal.org/user/23570 ). Please direct all support requests, feature requests, and bug reports to the issue queue: http://drupal.org/project/issues/advanced_forum

DEMO SITE:
http://socnet.shellmultimedia.com 

FUTURE PLANS:
This module is still under active development. The master to do list is here: http://groups.drupal.org/node/8426









