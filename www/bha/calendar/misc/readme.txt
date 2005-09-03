**********************************************************
*                                                        *
*               Proverbs PHP Web Calendar                *
*                                                        *
*                     Version 2.0                        *
*                                                        *
**********************************************************

This is a customizable web calendar developed using PHP and powered by MySQL. The calendar is viewed in month format with seperate pages detailing the events of each day as they are clicked on. 

For the most recent version of this application goto http://www.proverbs.biz

CALENDAR FILE LIST

base.inc.php
baselang.inc.php
calaccess.inc.php
calendar.alt.php
calendar.inc.php
calendar.php
caljavascript.js.php
config.inc.php
db_mysql.inc.php
dutch.lng.php
english.lng.php
french.lng.php
german.lng.php
iestylesheet.css.php
italian.lng.php
language.inc.php
navstylesheet.css.php
schedjavascript.js
schedule.alt.php
schedule.inc.php
schedule.nav.php
setup.inc.php
spanish.lng.php

ADMINISTRATION FILE LIST

caladmin\abase.inc.php
caladmin\adminjavascript.js.php
caladmin\caladmin.alt.php
caladmin\caladmin.inc.php
caladmin\caladmin.php

CREATION FILE LIST

caladmin\create.php
caladmin\createtables.inc.php
caladmin\processcreate.php

MISC FILE LIST

misc\readme.txt (this file)
misc\caltables.sql
misc\EULA.txt

INSTALLATION

1) Read the terms and conditions set forth in the End User License Agreement (EULA) contained in the file "misc\EULA.txt".  If you do not agree with the terms and conditions of the EULA, do not install this software.

2) Edit the "setup.inc.php" file to include your website information, color choices and optional title image.  Descriptions are provided for appropriate fields.  All color values must be in hexadecimal format without the # symbol.

3) Edit the "config.inc.php" file to include the information used to attach to your MySQL database.  If the MySQL database that will be used to hold the calendar tables does not exist you must create it manually.  See the MySQL website for further help with creating databases or contact your service provider.

4) Copy all files in the calendar, administration and creation file lists to a directory on your website.  Be certain to place the administration and creation files in a subdirectory off of the calendar directory.

5) Open your web browser to point to "create.php" on your website.  Enter the username and password (if applicable) for the administrative user.  This file should create all the tables needed in your MySQL database for the calendar application.  This administrative user can be removed once a new user is created.

6) Delete the calendar creation files from your website.  These file is only needed for the initial table creation.


NOTE

Version 2.0 of the Proverbs Web Calendar is not compatible with older calendar database tables.

REVISION HISTORY

2.0.7 - 05/06/05 - 1) Fixed multiple All Day event display in Internet Explorer version of Schedule.  
                   2) Add variable $schedule_scroll_time to "setup.inc.php" file to allow control the 
                   time the IE Schedule will scroll to.

2.0.6 - 04/22/05 - 1) Fixed division by zero bug.  
                   2) Configured IE Schedule to scroll to 8:00 am at load.  
                   3) Added javascript empty field check to Calendar Administration pages.  
                   4) Added browser check for Opera and Firefox browsers.

2.0.5 - 03/21/05 - Fixed intermittent stylesheet problems with some browsers.

2.0.4 - 02/22/05 - Fixed variable initialization bug.

2.0.3 - 01/07/05 - Fixed array declaration bug in schedule.inc.php.

2.0.2 - 12/31/04 - Fixed event running to midnight (12:00 AM) bug.

2.0.1 - 12/28/04 - Fixed 31st of month bug.

2.0.0 - 12/20/04 - Initial release of version 2.0.

1.2.2 - 12/06/02 - Fixed am to pm bug.

1.2.1 - 09/17/02 - Fixed problem with time being recorded incorrectly in some versions of MySQL.

1.2.0 - 05/29/02 - 1) Added variable $time_format in the "layout.inc.php" file.  This variable has
                   two settings "24" and "12".  Setting the variable to "12" will force display of time
                   on the schedule in a 12 hour AM/PM format.  Setting the variable to "24" or any other
                   setting will display the schedule in the standard 24 hour format.
                   2) Added variable $start_day in the "layoung.inc.php" file.  This variable is used
                   to set the starting day of the week on the calendar display.  The variable settings
                   range from 0 to 6 and correspond to Sunday to Saturday respectively.  Settings outside
                   the scope will set the calendar to begin with Sunday.  Alternate beginning day of the 
                   week for calendar view was created by Marion Heider of clixworx.net.
                   3) Updated the "setupdb.php" file to check for successful table creation into the
                   database and display results accordingly.

1.1.0 - 03/04/02 - 1) Added variable $time_zone in the "layout.inc.php" file.  Setting this variable
                   to "auto" will force display of the servers time zone on the schedule page.  All
                   other settings will display the variable's text; i.e. $time_zone = 'EST' will
                   display 'EST' after each hour block regardless of the servers time zone.
                   2) Added javascript onClick event to the radio buttons when selecting an entry
                   to edit in the "CalAdmin.php" Edit Existing page that automatically fills in the 
                   current values for that entry into the appropriate text boxes.  All single and 
                   double quotes are stripped from the original text due to the combination of PHP and 
                   Javascript.

1.0.1 - 02/04/02 - 1) Changed addition of recurring events with settings of(Event Day): 
                   "All(Weekly)DAY's of the Month of..." to always be a weekly event.
                   2) Added a strip_tag statement to remove unwanted HTML or PHP code from
                   any event being entered; allows <B>, <U>, <I> and <FONT> tags still.

1.0.0 - 12/31/01 - Initial release.  Written in PHP and powered by MySQL.


COPYRIGHT NOTICE

©2004 Proverbs, LLC. All rights reserved. This program is free software; you can redistribute it and/or modify it with the following stipulations: Changes or modifications must retain all Copyright statements, including, but not limited to the displayed Copyright statement and Proverbs, LLC homepage hyperlink provided at the bottom of the calendar and schedule pages.