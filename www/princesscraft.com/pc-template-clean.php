<?php

header("Content-Type: text/html; charset=utf-8");
require("common_funcs.php");
$ptr="/";
$root_dir = dirname(__FILE__);

if(file_exists("{$root_dir}/pc_root.php")) require("{$root_dir}/pc_root.php");

ob_start("do_footer");
ob_start("do_content");
do_header();
if(!isset($full_width)) $full_width=false;

function do_header() {
    $id = "princesscraft-com";
    $class = "pc-home";
    global $full_width, $ptr;
    if($full_width) $class.=" wide";
    
    global $title, $sec_title;
    echo '
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>';

echo "
<title>{$title}</title>
";

echo '
    <link rel="stylesheet" type="text/css" href="'.$ptr.'c/main-new.css" />
    <link rel="stylesheet" type="text/css" href="'.$ptr.'c/quote.css" />
    <link rel="shortcut icon" href="'.$ptr.'favicon.ico" type="image/vnd.microsoft.icon" />
    <link rel="icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
    <link rel="alternate" type="application/rss+xml" title="Princess Craft Inventory" href="'.$ptr.'rss/" />
</head>
<body id="'.$id.'" class="'.$class.'">
<div id="wrap">
<div id="header">
    <img src="'.$ptr.'i/crown.gif" width="124" height="108" alt="" />
    <h1>Princess Craft Campers &amp; Trailers</h1>
    <ul id="nav">
        <li><a href="#">Camping Trailers</a></li>
        <li><a href="#">Truck Campers</a></li>
        <li><a href="#">Truck Caps</a></li>
        <li><a href="#">Accessories</a></li>
    </ul>
    <div class="cf"></div>
</div>
';

echo "
<div id=\"content\"><div id=\"content_inner\"><div id=\"content_inner_header\"><h2>{$sec_title}</h2></div>
";

}

function do_footer($buf) {
    global $ptr;
    return $buf.'
<div id="footer">
    <address>
        Copyright &copy; 2005 Princess Craft Manufacturing, Inc.<br />
        102 N 1st St, Pflugerville, TX 78660-2754<br />
        Phone: (512) 251-4536, Toll Free: (800) 338-7123, Fax: (512) 251-3134<br />
        <a href="mailto:sales@princesscraft.com">sales@princesscraft.com</a>
    </address>
    <ul>
    	<li><a href="http://validator.w3.org/check?uri=referer">XHTML 1.0</a>/</li>
    	<li><a href="http://jigsaw.w3.org/css-validator/check/referer">CSS 2.0</a>/</li>
    	<li><a title="RSS 2.0" href="'.$ptr.'rss/" id="rssbutton">RSS 2.0</a></li>
    </ul>
</div> <!-- end div -->
</div> <!-- end wrap -->
</body>
</html>
';

}

function do_content($buf) {
    $x=$buf.'</div></div>
';
    global $full_width, $ptr;
    if(!$full_width) {
    $x.='<div id="sidebar">
  <div id="rvtrader"><a href="http://www.rvtrader.com/rv_ur_list.php?uid=29388">Click here to See Our Current Inventory!</a></div>

<div id="img">
<img src="'.$ptr.'i/aliner_icon.png" height="87" width="110" alt="Aliner Pop-Up Camping Trailer" />

<img src="'.$ptr.'i/campers.png" height="87" width="110" alt="Truck Campers" />

<img src="'.$ptr.'i/caps.png" height="87" width="110" alt="Truck Caps" />

<img src="'.$ptr.'i/access.png" height="87" width="110" alt="Truck Accessories" />
</div>

</div>';
    }
    $x.='
<div class="fixit">&nbsp;</div><!-- get past the two columns -->
';
    return $x;

}

?>
