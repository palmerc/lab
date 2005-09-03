<?
/*  ©2004 Proverbs, LLC. All rights reserved.  */

require ('caladmin.inc.php');

$action = "";

if (isset($_POST['submit']) && $_POST['submit'] != "")
	$action = $_POST['submit'];

$page->createpage($action);

$page->displaypage();
?>