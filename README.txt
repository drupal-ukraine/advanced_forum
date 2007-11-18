DESCRIPTION:
Advanced forum is a glue/theme module which makes it easier for non programmers to put together a forum similar to popular forum packages using Drupal's core forum. 

advanced_forum.module:
* Determines if the current page is part of the forum
* Uses node-forum.tpl.php for comments as well as posts
* Creates a slew of forum related variables to pass to node-forum.tpl.php
* Creates variables from external helper modules such as user titles, user points, etc.

user_postcounts.module:
* Maintains the user forum post count 

fancy_forum.css:
* Contains the CSS to style the forums

node-forum.tpl.php:
* The template file for both posts and comments in the forum. The variables available are listed at the top. This file can be change to suit your layout.

image directory:
* forum-separater.gif - This is the background for each post/comment. It provides the visual separater between the author info and the post content.
* page.gif decorative page icon by the posted on date.
*** Need button icons for: new post, reply, edit, delete

INSTALLATION:

1) Copy the advanced forum project directory to your normal module directory (ie: sites/all/modules)
2) Copy node-forum.tpl.php to your theme directory
3) Enable the module at http://example.com/?q=admin/build/modules
4) The trickey part: Add the call to advanced forum to template.php:

* If your theme doesn't already have a template.php, just copy the included one to your theme directory.

* If you already have a template.php, open it and find the line "function _phptemplate_variables($hook, $vars) {"
Immediately after that, paste:
  if (module_exists('advanced_forum')) {
    $vars = advanced_forum_addvars($hook, $vars);
  }

* If you are using Garland, it's even trickier because Garland doesn't return the $vars. If you know what you're doing, you can adjust the function. Otherwise, change the name of template-garland.php to template.php and overwrite the one in Garland.

5) Install any helper modules that you like such as user titles, user points, etc, and they will automatically be picked up and added to the forum posts







