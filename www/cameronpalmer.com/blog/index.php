<?php
require_once("template.php");
require_once("funcs.php");


/* The RSS 2.0 feed */
if($_REQUEST['rss'] == 2) {
	header("Content-Type: application/xml");
	echo '<'.'?xml version="1.0" encoding="utf-8"?'.'>'."\n";
	$fullposts=false;
	if($_REQUEST['fp'] == 1) {
		$fullposts=true;
	}
	$rp=recentposts(10);
	$rss=array();
	foreach($rp as $k=>$v) {
		$rss[$k][1] = $v['title'];
		$p=getpostheader($v['id']);
		$rss[$k][3] = 'http://'.$_SERVER['HTTP_HOST'].$archive_root.date("Y/m/d/",$v['unixcreated']).transform_title($v['title']);
		if($fullposts) {
			foreach($p['item'][0]['body'] as $n) {
				$rss[$k][2] .= $n['value']."\n";
			}
		} else {
			$rss[$k][2] = $p['item'][0]['body'][0]['value'];
			if(isset($p['item'][0]['body'][1])) {
				$ss[$k][2] .= "\n<p>...<a href=\"{$rss[$k][3]}\">continue reading</a></p>";
			}
		}
		$rss[$k][4] = strtotime($p['item'][0]['created'][0]['value']);
		$rss[$k][5] = $p['item'][0]['author'][0]['value'];
			
	}
	$f[false] = "Excerpts";
	$f[true] = "Full posts";
	
	echo "<rss version=\"2.0\"\n\txmlns:content=\"http://purl.org/rss/1.0/modules/content/\"\n\txmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n\t>";
	echo "<channel>\n";
	echo "  <link>http://cameronpalmer.com/blog/</link>\n";
	echo "  <title>Cameron Palmer</title>\n";
	echo "  <description>It's my site, yo. ({$f[$fullposts]})</description>\n";
	echo "  <language>en</language>\n";
	foreach($rss as $r) {
		echo "  <item>\n";
		echo "    <title>".htmlspecialchars($r[1])."</title>\n";
		echo "    <link>".htmlspecialchars($r[3])."</link>\n";
		echo "    <pubDate>".date("r", $r[4])."</pubDate>\n";
		echo "    <dc:creator>".$r[5]."</dc:creator>\n";
		/* Ugly hack for images */
		$r[2]=str_replace("src=\"/","src=\"http://{$_SERVER['HTTP_HOST']}/",$r[2]);
		$r[2]=str_replace("href=\"/","href=\"http://{$_SERVER['HTTP_HOST']}/",$r[2]);
		echo "    <description>".smart_trim2(strip_tags($r[2]), 300)."</description>\n";
		echo "    <content:encoded><![CDATA[".escape_cdata($r[2])."]]></content:encoded>\n";
		echo "  </item>\n";
	}
	echo "</channel>\n";
	echo "</rss>\n";
	exit();
}
function smart_trim2($str, $len) {
	$x = strip_tags($str);
	if(strlen($x) < $len) return htmlspecialchars($x);
	$x = substr($x, 0, $len);
	return htmlspecialchars($x)."...";
}

function escape_cdata($s) {
	$x = array(
		"<![CDATA[" => "< ![CDATA [",
		"]]>" => "] ]>"
	);
	return str_replace(array_keys($x), array_values($x), $s);
}

$y=$_REQUEST['y'];
$m=$_REQUEST['m'];
$d=$_REQUEST['d'];

if($y == "*" && $m=="*" && $d=="*") {
	//huh?
	die("Confused.");
} else if($y == "*" && $m=="*") {
	//monthly
	die("Monthly for $y $m");
}


/* This part is the index page */
doheader("Recent Posts");
$rp=recentposts(5);

$rp=array_sheepandlambs($rp,"/^([\d\-]+)\./");


#mprint_r($rp);
foreach($rp as $dayname=>$day) {
/*
	$arch=$archive_root.date("Y/m/d/",$p['unixcreated']).transform_title($p['title']);
	$nicedayname=date("F d, Y", strtotime($dayname));
*/
/*
	$n['y'] = date("Y",strtotime($dayname));
	$n['m'] = date("m",strtotime($dayname));
	$n['F'] = date("F",strtotime($dayname));
	$n['d'] = date("d",strtotime($dayname));
	$yl=sprintf("<a href=\"{$archive_root}%04d\" title=\"Yearly archive for %d\">%d</a>", $n['y'], $n['y'], $n['y']);
	$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\" title=\"Monthly archive for %s %d\">%s</a>", $n['y'], $n['m'], $n['F'], $n['y'], $n['F']);
	$dl=sprintf("<a href=\"{$archive_root}%04d/%02d/%02d\" title=\"Daily archive for %s %d, %d\">%d</a>", $n['y'], $n['m'], $n['d'], $n['F'], $n['d'], $n['y'], $n['d']);
	$nicedayname="{$ml} {$dl}, {$yl}";
	echo "<div class=\"day\"><h2>$nicedayname</h2>\n";*/
	foreach($day as $p) {
		format_post($p);
	}
/*	echo "</div>";*/
	
}
dofooter();
exit();




?>
