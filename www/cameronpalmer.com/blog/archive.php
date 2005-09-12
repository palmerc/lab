<?php

require_once("template.php");
require_once("funcs.php");
$archive_root="/blog/ark/";


$mycache=getcache();
$a=split_array($mycache,"%d-%d-%d.%d");

if($_REQUEST['t'] == "y") {
	//doing year
	$my=$_REQUEST['y']-0;
	if(isset($a[$my])) 
		$x=array_collapse($a[$my],4);
	doheader("Archive for {$my}");

/*	$ks=array_keys($a);
	ksort($ks);
	$ksf=array_flip($ks);
	if(isset($ks[$ksf[$my]-1])) {
		echo "<a href=\"{$archive_root}{$ks[$ksf[$my]-1]}\" title=\"Prev Year\">&laquo; (".$ks[$ksf[$my]-1].")</a><br />\n";
	}
	echo " ";
	if(isset($ks[$ksf[$my]+1])) {
		echo "<a href=\"{$archive_root}{$ks[$ksf[$my]+1]}\" title=\"Next Year\">(".$ks[$ksf[$my]+1].")&raquo; </a><br />\n";
	}*/
	if(!isset($a[$my])) {
		echo "No posts for this year.";
	} else {
		doarchiveyear($x);
	}
	dofooter();
} else if ($_REQUEST['t'] == "m") {
	//doing month
	$my=$_REQUEST['y']-0;
	$mm=$_REQUEST['m']-0;
	if(!isset($monthnames[$mm])) {
		doheader("Archive for Invalid Month");
		echo "<p>Invalid month.</p>\n";
	}
	if(!isset($a[$my][$mm])) {
		doheader("No posts");
		echo "<p>No posts for this month</p>\n";
	} else if(isset($monthnames[$mm])) {
		$x=array_collapse($a[$my][$mm],3);
		$x=array_flop($x,"unixcreated");
		ksort($x);
		$x=split_array($x,"%d-%d-%d.%d","id");
		#mprint_r($x[$my][$mm]);
		doheader("Archive for {$monthnames[$mm]} {$my}");
		$n['y'] = $my; #date("Y",strtotime($dayname));
		$n['m'] = $mm; #date("m",strtotime($dayname));
		$n['F'] = $monthnames[$mm]; #date("F",strtotime($dayname));
	/*	$yl=sprintf("<a href=\"{$archive_root}%04d\">%d</a>", $n['y'], $n['y']);
		$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\">%s</a>", $n['y'], $n['m'],$n['F']);*/
		$yl=sprintf("<a href=\"{$archive_root}%04d\" title=\"Yearly archive for %d\">%d</a>", $n['y'], $n['y'], $n['y']);
		$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\" title=\"Monthly archive for %s %d\">%s</a>", $n['y'], $n['m'], $n['F'], $n['y'], $n['F']);
		$nicedayname="{$ml}, {$yl}";
	#	echo "<h3><a href=\"{$archive_root}",sprintf("%04d/%02d",$my,$mm)."\">{$monthnames[$mm]}</a>, <a href=\"{$archive_root}{$my}\">{$my}</a></h3>\n";
		echo "<div class=\"day\"><h3>$nicedayname</h3>\n";
		echo "<div class=\"entry\">\n";
		doarchivemonth($my,$mm,$x[$my][$mm]);
		echo "</div></div>\n";
	}
	dofooter();
} else if ($_REQUEST['t'] == "d") {
	//doing day
	$my=$_REQUEST['y']-0;
	$mm=$_REQUEST['m']-0;
	$md=$_REQUEST['d']-0;
	if(isset($a[$my][$mm][$md])) 
		$x=array_collapse($a[$my][$mm][$md],2);
	doheader("Archive for {$monthnames[$mm]} {$md}, {$my}");
	$n['y'] = $my; #date("Y",strtotime($dayname));
	$n['m'] = $mm; #date("m",strtotime($dayname));
	$n['F'] = $monthnames[$mm]; #date("F",strtotime($dayname));
	$n['d'] = $md; #date("d",strtotime($dayname));
/*	$yl=sprintf("<a href=\"{$archive_root}%04d\">%d</a>", $n['y'], $n['y']);
	$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\">%s</a>", $n['y'], $n['m'],$n['F']);
	$dl=sprintf("<a href=\"{$archive_root}%04d/%02d/%02d\">%d</a>", $n['y'], $n['m'], $n['d'], $n['d']);*/

/*	$yl=sprintf("<a href=\"{$archive_root}%04d\" title=\"Yearly archive for %d\">%d</a>", $n['y'], $n['y'], $n['y']);
	$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\" title=\"Monthly archive for %s %d\">%s</a>", $n['y'], $n['m'], $n['F'], $n['y'], $n['F']);
	$dl=sprintf("<a href=\"{$archive_root}%04d/%02d/%02d\" title=\"Daily archive for %s %d, %d\">%d</a>", $n['y'], $n['m'], $n['d'], $n['F'], $n['d'], $n['y'], $n['d']);
	$nicedayname="{$ml} {$dl}, {$yl}";
	echo "<div class=\"day\"><h2>$nicedayname</h2>\n";*/
	if(!isset($a[$my][$mm][$md])) {
		echo "No entries for this day.";
	} else {
		
		foreach($a[$my][$mm][$md] as $id=>$p) {
			$p=array_collapse($p,0);
			format_post($p);
		}
/*		echo "</div>";*/
	}
/*	echo "</div>";*/


			#echo count($x). " posts from {$monthnames[$nm]} {$md}, ($my)<br />\n";
	/*
	echo subarray_count($a[$my][$mm][$md],0) . " posts from your year-month-day (".$monthnames[$mm]." ".ordinal($md).", $my)<br />\n";
	*/
	dofooter();
} else if ($_REQUEST['t'] == "c") {
	doheader("Archive of years");
	$a2=split_array($mycache,"%d-%s");
	foreach($a2 as $year => $data) {
		printf("<div class=\"day\"><a href=\"%s%04d\">Archive for %d</a> (%d posts)</div>\n", $archive_root, $year, $year, count($data));
		//echo $year.count($data)."<br />\n";
	}
	dofooter();	
/* This one never gets called */
} else if ($_REQUEST['t'] == "c") {
	//this month except for the most recent 5
	$my=date("Y")-0;
	$mm=date("m")-0;

	/* This part is the index page */
	doheader("Post that fell off the front page");
	$n=getcache();
	$n=array_flop($n,'unixcreated');
	ksort($n);
	$n=array_reverse($n);
	$n=array_flop($n,"id");
	$n=array_slice($n,5);
	$x=split_array($n,"%d-%d-%d.%d","id");
	$i=0;
	if(!isset($x[$my][$mm])) {
		echo "<p>No entries found for this month besides the contents of the homepage.</p>";
		dofooter();
		exit();
	}
	krsort($x[$my][$mm],true);
	#$x[$my][$mm]=array_reverse($x[$my][$mm]);
	foreach($x[$my][$mm] as $dayname => $day) {
		$day=array_collapse($day,2);
/*		$nicedayname="{$monthnames[$mm]} {$dayname}, {$my}";
		echo "<div class=\"day\"><h2>$nicedayname</h2>\n";*/
		$n['y'] = $my; #date("Y",$day[0]['unixcreated']);
		$n['m'] = $mm; #$dayname; #date("m",$dayname);
		$n['F'] = $monthnames[$mm]; #date("F",$day[0]['unixcreated']);
		$n['d'] = $dayname; #date("d",$day[0]['unixcreated']);
/*		$yl=sprintf("<a href=\"{$archive_root}%04d\">%d</a>", $n['y'], $n['y']);
		$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\">%s</a>", $n['y'], $n['m'],$n['F']);
		$dl=sprintf("<a href=\"{$archive_root}%04d/%02d/%02d\">%d</a>", $n['y'], $n['m'], $n['d'], $n['d']);*/
/*		$yl=sprintf("<a href=\"{$archive_root}%04d\" title=\"Yearly archive for %d\">%d</a>", $n['y'], $n['y'], $n['y']);
		$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\" title=\"Monthly archive for %s %d\">%s</a>", $n['y'], $n['m'], $n['F'], $n['y'], $n['F']);
		$dl=sprintf("<a href=\"{$archive_root}%04d/%02d/%02d\" title=\"Daily archive for %s %d, %d\">%d</a>", $n['y'], $n['m'], $n['d'], $n['F'], $n['d'], $n['y'], $n['d']);
		$nicedayname="{$ml} {$dl}, {$yl}";
		echo "<div class=\"day\"><h2>{$nicedayname}</h2>\n";*/
		foreach($day as $p) {
			format_post($p);
		}
/*		echo "</div>";*/
		
	}
	dofooter();

} else {
	die("Confused.");
}


function doarchiveyear($a) {
	global $monthnames;
	global $archive_root;
	$x=array_flop($a,"unixcreated");
	ksort($x);
	$x=split_array($x,$p="%d-%d-%d.%d","id");
	foreach($x as $ynum=>$year) {
/*		echo "<h2>Year $ynum</h2>\n";*/
		ksort($year);
		foreach($year as $mnum=>$month) {
#			echo "<h3><a href=\"{$archive_root}",sprintf("%04d/%02d",$ynum,$mnum)."\">{$monthnames[$mnum]}</a></h3>\n";
#			echo "<h3><a href=\"{$archive_root}",sprintf("%04d/%02d",$ynum,$mnum)."\">{$monthnames[$mnum]}</a>, <a href=\"{$archive_root}{$ynum}\">{$ynum}</a></h3>\n";
			$n['y'] = $ynum; #date("Y",strtotime($dayname));
			$n['m'] = $mnum; #date("m",strtotime($dayname));
			$n['F'] = $monthnames[$mnum]; #date("F",strtotime($dayname));
/*			$yl=sprintf("<a href=\"{$archive_root}%04d\">%d</a>", $n['y'], $n['y']);
			$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\">%s</a>", $n['y'], $n['m'],$n['F']);*/
			$yl=sprintf("<a href=\"{$archive_root}%04d\" title=\"Yearly archive for %d\">%d</a>", $n['y'], $n['y'], $n['y']);
			$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\" title=\"Monthly archive for %s %d\">%s</a>", $n['y'], $n['m'], $n['F'], $n['y'], $n['F']);
			$nicedayname="{$ml}, {$yl}";
			echo "<div class=\"day\"><h3>$nicedayname</h3>\n";
			echo "<div class=\"entry\">\n";
			doarchivemonth($ynum,$mnum,$month);
			echo "</div></div>\n";
			
		}
	}
}

function doarchivemonth($ynum,$mnum,$a) {
	global $monthnames;
	global $archive_root;
	if(!$a) {
		echo "<p>No entries found.</p>";
		return;
	}
	echo "<dl class=\"arch1 cf\">\n";
	ksort($a);
	foreach($a as $dnum=>$day) {
				
		echo "  <dt class=\"arch1\"><a href=\"{$archive_root}",sprintf("%04d/%02d/%02d", $ynum, $mnum, $dnum)."\">{$dnum}</a></dt>\n";
		echo "  <dd class=\"arch1\">\n";
		echo "    <ul class=\"arch2 cf\">\n";
		foreach($day as $id=>$entry) {
			$entry=array_collapse($entry,0);
			echo "      <li class=\"arch2\"><a href=\"{$archive_root}",sprintf("%04d/%02d/%02d/", $ynum, $mnum, $dnum).transform_title($entry['title'])."\">{$entry['title']}</a></li>\n";
		}
		echo "    </ul>\n";
#		echo "    <br style=\"clear: left;\" />\n";
		echo "  </dd>\n";
	}
	echo "</dl>\n";
#	echo "<br style=\"clear: left;\" />\n";
}

exit();
mprint_r($x);
exit();
$my=2004;$mm=5;$md=1;
echo "I'm sorry, but your post cannot be found.  There are:<br />\n";
echo subarray_count($a[$my],2) . " posts from your year ($my)<br />\n";
echo subarray_count($a[$my][$mm],1) . " posts from your year-month(".$monthnames[$mm].", $my)<br />\n";
echo subarray_count($a[$my][$mm][$md],0) . " posts from your year-month-day (".$monthnames[$mm]." ".ordinal($md).", $my)<br />\n";
/*echo "<pre>";
echo subarray_count($a, 1)."<br />";
print_r($a);
echo "</pre>";*/


?>
