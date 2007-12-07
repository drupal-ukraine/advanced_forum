ADVANCED FORUM
http://drupal.org/project/advanced_forum

DESCRIPTION:
Advanced forum is a glue/theme module which makes it easier for non programmers to put together a forum similar to popular forum packages using Drupal's core forum. It requres the forum module.

advanced_forum.module:
* Determines if the current page is part of the forum
* Uses node-forum.tpl.php for comments as well as posts
* Creates a slew of forum related variables to pass to node-forum.tpl.php
* Creates variables from external helper modules such as user titles, user points, etc.
* Has the logic to load up the CSS files

advanced_forum-structure.css:
* Contains basic structure such as heights, floats, margins, and padding

advanced_forum.css 
* Contains the CSS to style the forums

node-forum.tpl.php:
* The template file for both posts and comments in the forum. The variables available are listed at the top. This file can be change to suit your layout.

forum-display.tpl.php
* Over all forum display

forum-list.tpl.php
* Table that lists the forums

forum-topics.tpl.php
* Listing of topics in a single forum

user_postcounts.module:
* Maintains the user forum post count 

markasread.module:
* Adds a button to the forums to allow people to mark a single forum or all forum as read

image directory:
* forum-separater.gif - This is the background for each post/comment. It provides the visual separater between the author info and the post content.
* page.gif decorative page icon by the posted on date.
*** Need button icons for: new post, reply, edit, delete

INSTALLATION:
0) Make sure the forum module is enabled
1) Copy the advanced forum project directory to your normal module directory (ie: sites/all/modules)
2) Copy everything in the "fortheme" subdirectory except template.php and template-garland.php to your theme directory
3) Enable the module at http://example.com/?q=admin/build/modules
4) The trickey part: Add the call to advanced forum to template.php:

* If your theme doesn't already have a template.php, just copy the included one to your theme directory.

* If you already have a template.php, open it and find the line "function _phptemplate_variables($hook, $vars) {"
Immediately after that, paste:
  if (module_exists('advanced_forum')) {
    $vars = advanced_forum_addvars($hook, $vars);
  }

* If you are using Garland, it's even trickier because Garland doesn't return the $vars. If you know what you're doing, you can adjust the function. Otherwise, change the name of template-garland.php to template.php and overwrite the one in Garland.


5) Adjust all the settings pages

Go to ?q=admin/content/comment/settings
* Set Default display mode: Flat list - expanded
* Default display order: Date - oldest first
* The rest however you want

Go to ?q=admin/user/settings
* Enable picture support
* Set a default photo if you wish

Go to ?q=admin/build/themes/settings   
* Enable User pictures in posts
* Enable User pictures in comments 

Go to ?q=admin/content/forum/settings
* Set the topic list to display how you want

6) Install any helper modules that you like such as user titles, user points, etc, and they will automatically be picked up and added to the forum posts. If you use a module that advforum doesn't recognize, please file a feature request. (See section "modules made use of" below for a list.)

THEMING AND SKINNING:
Out of the box, the forum is a basic blue. You can change this in several ways.

* To use just the structure CSS and handle the rest in your theme's style.css, copy advanced_forum.css to the root of your theme directory and empty it out.

* To modify the existing CSS, copy advanced_forum.css to the root of your theme directory and make any needed changes. This file will replace the one in the module directory.

* To keep all the existing CSS but add on to it or make minor overrides, create an advanced_forum-skin.css in the root of your theme directory. This will be added on after the other CSS files.


MODULES MADE USE OF:
user_postcount (included)
markasread (included)
user titles ( http://drupal.org/project/user_titles )
user points ( http://drupal.org/project/user_points )

CREDITS:

Advanced forum is based on flatforum ( http://drupal.org/project/flatforum ). Here are the credits listed for flatforum:

"The code was originally written by Károly Négyesi. The CSS is Tibor Liktor's work. These phases were sponsered by http://ak47.dk, and RealROOT BVBA for http://www.zattevrienden.be/forum (not work-safe)."

Additional help for advanced forum from:

The mark all read sub module is from here: http://wtanaka.com/drupal/markasread

Topic icons by yoroy http://drupal.org/user/41502

Thanks to everyone who has helped by providing testing, patches, design help, etc. If you feel you've been left out and wish to be mentioned here, please let me know. This module pulls together the work of a lot of people and I want to be sure everyone gets the credit they deserve.

MAINTAINER:
Advanced forum is maintained by Michelle Cox ( http://drupal.org/user/23570 ). Please direct all support requests, feature requests, and bug reports to the issue queue: http://drupal.org/project/advanced_forum

DEMO SITE:
The development / demo site is at http://advforum.shellmultimedia.com

FUTURE PLANS:
This module is still under active development. Once the Drupal 5 is reasonably finished, I intend to port it to Drupal 6.









