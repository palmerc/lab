<?php
header("Content-Type: text/html; charset=utf-8");

$ptr="/";
$root_dir = dirname(__FILE__);
if (file_exists("{$root_dir}/stc-root.php")) require("{$root_dir}/stc-root.php");

do_header();
ob_start("do_footer");
ob_start("do_content");

function do_header() {
   global $title, $calendar, $leftbar, $alternate_css, $ptr, $user_id, $admin;
   echo'
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
      ';
   echo"
   <title>{$title}</title>
   <link rel=\"stylesheet\" type=\"text/css\" href=\"".$ptr."c/main.css\" />
      ";
   if ($alternate_css) {
      echo "
      <link rel=\"stylesheet\" type=\"text/css\" href=\"{$ptr}{$alternate_css}\" />\n";
}
?>
      <link rel="shortcut icon" href="<?php echo "{$ptr}i/favicon.ico"; ?>" type="image/vnd.microsoft.icon" />
      <link rel="icon" href="<?php echo "{$ptr}i/favicon.ico"; ?>" type="image/vnd.microsoft.icon" />
   </head>
   <body>
	<div id="container">
		<div id="header"> 
         <div id="topbanner">
			<img src="<?php echo "{$ptr}i/purpletopwtextslim.gif"?>" alt="The Lone Star Community" width="770" height="90"/>
         </div>
         <div id="topnav">
            <ul id="topbar">
               <li><a href="<? echo $ptr; ?>">Home</a></li>
               <li><a href="<? echo $ptr; ?>vote/">Vote</a></li>
               <li><a href="http://students.csci.unt.edu/~wng0001/Website/lonestarwebsiteform1.html">Create a Report</a></li>
               <li><a href="<? echo $ptr; ?>calendar/">Calendar</a></li>
               <li><a href="<? echo $ptr; ?>repository.php">Repository</a></li>
               <li><a href="<? echo $ptr; ?>edit.php?email=<?php echo $user_id; ?>">Update Personal Information</a></li>
               <?if ($admin == 1) echo "<li><a href=\"{$ptr}manage.php\">Manage Users</a></li>"; ?>
               <li><a href="logout.php">Log Out</a></li>
            </ul>
         </div>
      </div>
		<div id="main">
<? if (!$calendar) { ?>
         <div id="leftbar">
<?php if (file_exists("{$leftbar}")) require("{$leftbar}"); ?>
         </div>
         <div id="rightbar">
<?php
   }
   else
   {
      echo '<div id="fullbar">';
   }
}

function do_content($buf) {
   $x=$buf;
   return $buf;
}

function do_footer($buf)
{
   return $buf.'
         </div>
      </div>
      <div id="footer">
	  	<p>Lone Star Community, lonestar@stc.org</p>
      </div>
   </div>
</body>
</html>
               ';

}
?>