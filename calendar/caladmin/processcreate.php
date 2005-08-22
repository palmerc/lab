<?
/* ©2004 Proverbs, LLC. All rights reserved. */ 

require ('createtables.inc.php');

$username = "";
$userpass = false;

if (isset($_POST['name']) && $_POST['name'] != "")
	$username = strtolower($_POST['name']);
else
{
	// redirect to create page
	header("Location: create.php");
	exit;
}

if (isset($_POST['pass']) && $_POST['pass'] != "")
	$userpass = $_POST['pass'];

$creator = new setupdb;

$success = $creator->CreateDatabase($username, $userpass);
?>
<HTML>
<HEAD>
	<TITLE>Calendar Creation</TITLE>
</HEAD>
<BODY>
	<CENTER>
<?
	if ($success)
	{
		echo '		Calendar database tables successfully created.
		<br>
		<br>
		For security purposes, verify the removal of or manually remove the following setup files:
		<br>
		create.php
		<br>
		createtables.inc.php
		<br>
		processcreate.php
';
	}
	else
	{
		echo '		There was an error creating the Calendar tables.
		<br>
		<br>
		Verify the database connection setting are correct in the config.inc.php file
		<br>
		and that the database exists on the SQL server.
';
	}
?>
	</CENTER>
</BODY>
</HTML>
<?
if (isset($_POST['autodelete']) && $_POST['autodelete'] == "yes" && $success)
{
	unlink('createtables.inc.php');
	unlink('create.php');
	unlink('processcreate.php');
}
?>