<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

header('Content-type: text/css');

require ('setup.inc.php');

// create the setup object
$conf = new layout_setup;

echo '
body {color: #'.$conf->html_body_text_color.'; font-family: Arial; font-size: 14px; text-decoration: none; ';
if (trim($conf->html_body_background_image) != '')
	echo 'background-image: url('.$conf->html_body_background_image.'); ';
echo '
	background-color: #'.$conf->html_body_background_color.'; margin: 0px; padding: 0px}
div {height: 50px; overflow: auto; clip: auto}
img {border-style: none; border-width: 0px; margin: 0px; padding: 0px}
input {font-size: 12px; height: 22px}
table {border-style: none; margin: 0px; padding: 0px; border-width: 0px; text-indent: 0px; empty-cells: show}
table.calendar {border-color: #'.$conf->calendar_border_color.'}
table.calhead {border-color: #'.$conf->calendar_border_color.'; width: 100%}
table.login {width: 400px; border-style: solid; border-width: 1px; border-color: #'.$conf->schedule_border_color.'; color: #'.$conf->schedule_text_color.'}
table.schedule {width: 95%; border-style: solid; border-width: 1px; border-color: #'.$conf->schedule_border_color.'; color: #'.$conf->schedule_text_color.'}
tr {border-style: none; border-width: 0px; margin: 0px; padding: 0px}
td {border-style: none; border-width: 0px; margin: 0px; padding: 2px; text-align: left; vertical-align: top}
td.login {text-align: center; vertical-align: middle; font-size: 12px; color: #'.$conf->schedule_time_text_color.'; 
	background-color: #'.$conf->schedule_time_background_color.'; border-style: groove; border-width: 2px; padding: 4px}
td.caladmin {text-align: left; vertical-align: middle; font-size: 12px; color: #'.$conf->calendar_title_text_color.'; padding: 4px;
	background-color: #'.$conf->calendar_title_background_color.'; border-style: groove; border-width: 1px}
td.calmenu {color: #'.$conf->html_body_text_color.'; font-size: 12px; height: 40px; vertical-align: middle; ';
if (trim($conf->html_body_background_image) != '')
	echo 'background-image: url('.$conf->html_body_background_image.'); ';
echo '
	background-color: #'.$conf->html_body_background_color.'}
td.calhead {width: 100%; border-style: solid; border-width: 1px; text-align: center; vertical-align: middle; ';
if (trim($conf->calendar_title_background_image) != '')
	echo 'background-image: url('.$conf->calendar_title_background_image.'); ';
echo '
	column-span: 7; color: #'.$conf->calendar_title_text_color.'; background-color: #'.$conf->calendar_title_background_color.'}
td.caltitle {width: 100%; height: 60px; border-style: none; border-width: 0px; text-align: center; ';
if (trim($conf->calendar_title_background_image) != '')
	echo 'background-image: url('.$conf->calendar_title_background_image.'); ';
echo '
	font-size: 36px; vertical-align: middle; column-span: 3; color: #'.$conf->calendar_title_text_color.'; background-color: #'.$conf->calendar_title_background_color.'}
td.calmonths {height: 24px; width: 200px; border-style: none; border-width: 0px; text-align: center; vertical-align: middle; ';
if (trim($conf->calendar_title_background_image) != '')
	echo 'background-image: url('.$conf->calendar_title_background_image.'); ';
echo '
	font-size: 12px; color: #'.$conf->calendar_title_text_color.'; background-color: #'.$conf->calendar_title_background_color.'}
td.calweekday {width: 100px; height: 24px; border-style: solid; border-width: 1px; font-size: 11px; text-align: center;
	vertical-align: middle; background-color: #'.$conf->calendar_day_background_color.'; color: #'.$conf->calendar_day_text_color.'}
td.calday {width: 100px; height: 85px; border-style: solid; border-width: 1px; font-size: 9px; cursor: pointer; 
	background-color: #'.$conf->calendar_day_background_color.'; color: #'.$conf->calendar_day_text_color.'}
td.schedone {width: 40%; height: 50px; padding: 1px; font-size: 9px; background-color: #'.$conf->schedule_one_background_color.'; 
	color: #'.$conf->schedule_text_color.'}
td.schedtwo {width: 40%; height: 50px; padding: 1px; font-size: 9px; background-color: #'.$conf->schedule_two_background_color.'; 
	color: #'.$conf->schedule_text_color.'}
td.schedtime {width: 10%; height: 50px; text-align: right; vertical-align: middle; font-size: 11px; color: #'.$conf->schedule_time_text_color.'; 
	background-color: #'.$conf->schedule_time_background_color.'; border-style: groove; border-width: 2px; padding: 0px}
td.schedtop {width: 100%; text-align: center; vertical-align: middle; color: #'.$conf->schedule_time_text_color.'; 
	background-color: #'.$conf->schedule_time_background_color.'; padding: 0px; font-size: 14px; height: 40px; 
	border-style: groove; border-width: 2px}
a {border-style: none; border-width: 0px; font-size: 14px; font-style: normal; margin: 0px; padding: 0px; 
	text-align: left; text-decoration: none; text-indent: 0px}
a.calmenu {font-size: 12px; color: #'.$conf->html_body_text_color.'}
a.calmonths {color: #'.$conf->calendar_title_text_color.'}
';
?>