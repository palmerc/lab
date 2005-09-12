<?php

require_once("template.php");
require_once("funcs.php");

if(!isset($_REQUEST['p'])) die("No page specified");

$p=strtolower($_GET['p']);

if(($f=return_page($p)) !== false) {
	$f=$f['item'][0];
	doheader($f['title'][0]['value']." (Tim Hatch)",$p);
	echo "<div class=\"day\"><h2>{$f['title'][0]['value']}</h2></div>\n";
	echo "<div class=\"entry\">\n";
	echo $f['body'][0]['value'];
	echo "</div>";
	dofooter();
} else {
	doheader("Error (Tim Hatch)", "error");
	echo "<div class=\"err\">{$err}</div><br /><br /><br /><br /><br /><br />\n";
	dofooter();
}
?>