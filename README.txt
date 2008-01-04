ADVANCED FORUM
http://drupal.org/project/advanced_forum

DESCRIPTION:
Advanced forum is a glue/theme module which makes it easier for non programmers to put together a forum similar to popular forum packages using Drupal's core forum. It requres the forum module.

advanced_forum.module:
* Determines if the current page is part of the forum
* Uses forum-thread.tpl.php for comments as well as posts
* Creates a slew of forum related variables to pass to forum-thread.tpl.php
* Creates variables from external helper modules such as user titles, user points, etc.
* Has the logic to load up the CSS files

advanced_forum-structure.css:
* Contains basic structure such as heights, floats, margins, and padding

advanced_forum.css 
* Contains the CSS to style the forums

forum-display.tpl.php
* Over-all forum display

forum-list.tpl.php
* Table that lists the forums

forum-topics.tpl.php
* Listing of topics in a single forum

forum-thread.tpl.php:
* The template file for both posts and comments in the forum. The variables available are listed at the top. This file can be change to suit your layout.

user_postcounts.module:
* Maintains the user forum post count 

markasread.module:
* Adds a button to the forums to allow people to mark a single forum or all forum as read

image directory:
* forum-separater.gif - This is the background for each post/comment. It provides the visual separater between the author info and the post content. You need to change this when changing the color of your forums.
* page.gif decorative page icon by the posted on date.
*** Need button icons for: new post, reply, edit, delete

INSTALLATION:
1) Make sure the forum module is enabled and you have forums and (optionally) containers set up.
2) Copy the entire advanced_forum project directory (not just the contents) to your normal module directory (ie: sites/all/modules)
2) In the themes directory of the module is a directory called "advforum" that contains all the theme files. Copy this entire advforum directory (not just the contents) to your theme directory. If you use more than one theme with forums, copy it to each.
3) Enable the advanced forum module at http://example.com/?q=admin/build/modules
4) Optionally enable the user postcounts and markasread modules included in the package.
5) Add the call to advanced forum to template.php. This is the hardest part because how it's done is different in every theme. Pick the method that best matches your theme:

* If your theme is listed in the "themes" directory, and you have not modified the template.php that comes with your theme, you can try using the template.php found there. As themes get updated, these files may get out of date. If it doesn't work, try one of the methods below.

* If your theme doesn't already have a template.php, create one at the root of your theme and paste in this:

<?php
function _phptemplate_variables($hook, $vars) {
  if (module_exists('advanced_forum')) {
    $vars = advanced_forum_addvars($hook, $vars);
  }

  return $vars;
}

(Note: you don't want a ?> here)

* If you already have a template.php, see if it contains this line: "function _phptemplate_variables($hook, $vars) {"

If it doesn't, paste in the code above except for the <?php part somewhere outside of a function. Right at the top of the file should be fine.

If it does have that line, you will need to merge the code into this function, which can be tricky for non programmers. This function will vary from theme to theme but the easiest place for most themes is right at the top after the "function..." line. Paste this:

if (module_exists('advanced_forum')) {
  $vars = advanced_forum_addvars($hook, $vars);
}

Just doing that should work in most themes. Some themes, though, like Garland, don't return the $vars variable from the function in all cases. That makes it harder. You will need find the bottom of the function which will be a "}" on a line all by itself right before the next line that starts with "function". Look on the line before that "}". If it says "return array();" you will need to replace the "array();" with "$vars;". If the line before is another "}" then leave that bracket there and put "return $vars;" immediately before the last "}" 

The end result is you want the function to look like this:

function _phptemplate_variables($hook, $vars) {
  if (module_exists('advanced_forum')) {
    $vars = advanced_forum_addvars($hook, $vars);
  }

***There may be other code in this section, probably something like "case ($hook == 'page)"***


  return $vars;
}

function SOMEOTHERFUNCTION() {  (This may not exist if you are at the bottom of the file)

I've done my best to explain this but, if you just can't figure out how to do this step, post an issue with the template.php of the theme you are using and I will try to help.


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

THEMING:
If you want to make major changes to the forum, edit the files in the "advforum" directory you copied into your theme directory. If you just want to make a few changes, make them in the style.css that comes with your theme. That will be called after and override the css that comes with the module. 

Theming of advforum is still being worked on at this point, so it's recommended you record any changes you make if you are an early adopter so you can upgrade them.

MODULES MADE USE OF:
user_postcount (included)
markasread (included)
user titles ( http://drupal.org/project/user_titles )
user points ( http://drupal.org/project/user_points )
user badges ( http://drupal.org/project/user_badges )

CREDITS:

Advanced forum is based on flatforum ( http://drupal.org/project/flatforum ). Here are the credits listed for flatforum:

"The code was originally written by Károly Négyesi. The CSS is Tibor Liktor's work. These phases were sponsered by http://ak47.dk, and RealROOT BVBA for http://www.zattevrienden.be/forum (not work-safe)."

Additional help for advanced forum from:

The mark all read sub module is from here: http://wtanaka.com/drupal/markasread

Topic icons by yoroy http://drupal.org/user/41502

Theming help from jacine, maintainer of the Sky theme.

Thanks to everyone who has helped by providing testing, patches, design help, etc. If you feel you've been left out and wish to be mentioned here, please let me know. This module pulls together the work of a lot of people and I want to be sure everyone gets the credit they deserve.

MAINTAINER:
Advanced forum is maintained by Michelle Cox ( http://drupal.org/user/23570 ). Please direct all support requests, feature requests, and bug reports to the issue queue: http://drupal.org/project/advanced_forum

DEMO SITE:
The development / demo site is at http://advforum.shellmultimedia.com . If the site is down, try again later. The module is being actively developed there and it doesn't always work.

FUTURE PLANS:
This module is still under active development. Once the Drupal 5 is reasonably finished, I intend to port it to Drupal 6.









