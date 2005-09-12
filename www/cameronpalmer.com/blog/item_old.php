<?php
require_once("template.php");
require_once("funcs.php");

$archive_root="/ark/";

/* This part is the individual archive */
$y=$_GET['y']-0;
$m=$_GET['m']-0;
$d=$_GET['d']-0;
$t=$_GET['t'];

if(!$p=findpost($y, $m, $d, $t))
	die("Cannot find specified post");

doheader($p['title'],'archive');
/*	$x['y'] = $y; #date("Y",strtotime($dayname));
	$x['m'] = $m; #date("m",strtotime($dayname));
	$x['F'] = date("F",$p['unixcreated']);
	$x['d'] = $d; #date("d",strtotime($dayname));
	$yl=sprintf("<a href=\"{$archive_root}%04d\">%d</a>", $x['y'], $x['y']);
	$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\">%s</a>", $x['y'], $x['m'],$x['F']);
	$dl=sprintf("<a href=\"{$archive_root}%04d/%02d/%02d\">%d</a>", $x['y'], $x['m'], $x['d'], $x['d']);
	$nicedayname="{$ml} {$dl}, {$yl}";
	echo "<div class=\"day\"><h2>$nicedayname</h2>\n";*/

#echo "<p class=\"posted\">Posted on <a href=\"/ark/$y/$m\">{$monthnames[$m]}</a> <a href=\"/ark/$y/$m/$d\">".ordinal($d)."</a>, <a href=\"/ark/$y\">$y</a></p>\n";

format_post($p);
echo "<h3><span>Comments For This Post</span></h3>\n";
dofooter();
exit();


?>
