<?
/* ©2004 Proverbs, LLC. All rights reserved. */ 

require ('../setup.inc.php');
?>
<HTML>
<HEAD>
	<TITLE>Calendar Creation</TITLE>
</HEAD>
<BODY>
	<CENTER>
	Enter the account information that will be used to administer the Calendar below.
	<br>
	Other users can be updated and/or created using this primary account.
	<br>
	<br>
	This script will only create the calendar tables, the database listed in the
	<br>
	config.inc.php file must already exist.
	<br>
	<br>
	Note: If system security is utilized for the Calendar, the username entered must
	<br>
	match a system account name.
	<br>
	<br>
	<FORM METHOD="post" ACTION="processcreate.php">
	<TABLE style="border-style: solid; border-width: 1px; border-color: #000000" cellspacing=0 cellpadding=0>
		<TR>
			<TD style="text-align: center; border-style: solid; border-width: 1px; border-color: #000000" COLSPAN=2>
				Administrator Account
			</TD>
		</TR>
		<TR>
			<TD style="text-align: right; border-style: solid; border-width: 1px; border-color: #000000">
				Username:&nbsp;
			</TD>
			<TD style="text-align: left; border-style: solid; border-width: 1px; border-color: #000000">
				&nbsp;<INPUT type="text" maxlength="30" style="width: 150px" name="name"
<?
	if (isset($_SERVER['PHP_AUTH_USER']) && $_SERVER['PHP_AUTH_USER'] != "")
		echo ' value="'.$_SERVER['PHP_AUTH_USER'].'">&nbsp;
';
	else
		echo '>&nbsp;
';
?>				
			</TD>
		</TR>
<?
	// create the setup object
	$conf = new layout_setup;
	if (!isset($conf->admin_serverside_login) || !$conf->admin_serverside_login)
	{
		echo '		<TR>
			<TD style="text-align: right; border-style: solid; border-width: 1px; border-color: #000000">
				Password:&nbsp;
			</TD>
			<TD style="text-align: left; border-style: solid; border-width: 1px; border-color: #000000">
				&nbsp;<INPUT type="password" maxlength="30" style="width: 150px" name="pass">&nbsp;
			</TD>
		</TR>
';
	}
?>
		<TR>
			<TD style="text-align: center; border-style: solid; border-width: 1px; border-color: #000000" COLSPAN=2>
				Auto Delete Option
			</TD>
		</TR>
		<TR>
			<TD style="text-align: left; border-style: solid; border-width: 1px; border-color: #000000" COLSPAN=2>
				&nbsp;<INPUT type="checkbox" name="autodelete" value="yes">Remove Creation Files
			</TD>
		</TR>
		<TR>
			<TD style="text-align: center; border-style: solid; border-width: 1px; border-color: #000000" COLSPAN=2>
				<INPUT style="width: 80px" type="submit" name="submit" value="Submit">
			</TD>
		</TR>
	</TABLE>
	</FORM>
	</CENTER>
</BODY>
</HTML>
