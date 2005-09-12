<?php

require_once("_config.php");
require_once("XMLParser.php");
require_once("template.php");


function getcache() {
	global $post_dir;
	$cachename=$post_dir."cache.txt";
	if(file_exists($cachename)) {
		return unserialize_file($cachename);
	} else {
		return rebuildcache();
	}
}

function rebuildcache() {
	global $post_dir,$CACHE;
	if(isset($CACHE)) return $CACHE;
	if(!$p=getpostlist()) die("Cannot rebuild cache");
	$c=array();
	foreach($p as $i) {
		//do something
		if(!$h=getpostheader($i)) {
			echo "Post $i is inaccessible.<br />\n";
			continue;
		}
		$c[$i]['title'] = $h['item'][0]['title'][0]['value'];
		$c[$i]['created'] = $h['item'][0]['created'][0]['value'];
		$c[$i]['unixcreated'] = strtotime($c[$i]['created']);
		$c[$i]['author'] = $h['item'][0]['author'][0]['value'];
		$c[$i]['prettycreated'] = date("Y/m/d h:i a", $c[$i]['unixcreated']);
		$m=$c[$i]['unixcreated'];
		$c[$i]['id']=$i;
		if($h['item'][0]['modified']) {
			foreach($h['item'][0]['modified'] as $nm) {
				if(strtotime($nm['value']) > $m)
					$m=strtotime($nm['value']);
			}
		}
		$c[$i]['unixmodified'] = $m;
	}
	//write to cache
//	serialize_file($post_dir."cache.txt",$c);
	$CACHE=$c;
	//and return
	return $CACHE;
}

function getpostheader($d) {
	global $post_dir;
	$fn="{$post_dir}{$d}/{$d}.xml";
	if(!file_exists($fn)) return false;
	if(!$parser = new XMLParser(realpath($fn), 'file', 0, 0))
		die("Bad XML");
	if(!$tree = $parser->getTree())
		die("Bad XML Tree");
	return $tree;
}

function getpostlist() {
	$p=array();
	global $post_dir;
#	echo "Opening {$post_dir}<br />\n";
	if(!$dh=opendir($post_dir)) return false;
	while (false !== ($file = readdir($dh))) { 
		if(sscanf($file, "%d-%d-%d.%d", $my, $mm, $md, $mi)) {
			$p[]=$file;
		}
	}
	return $p;
}

/*function ($y="*",$m="*",$d="*",$t="*") {
	global $post_dir;
	$dh=opendir("../{$post_dir}/");
	$possibilities=array();
	while (false !== ($file = readdir($dh))) { 
		if(sscanf($file, "%d-%d-%d.%d", $my, $mm, $md, $mi)) {
			if(($y=="*" || $my==$y) && ($m=="*" || $mm==$m) && ($d=="*" || $md==$d)) {
				$possibles[]=$file;
	#  	  echo "$file Matches YMD test<br />\n";
			}
		}
	}

	// okay, the next part is to actually read those files in, and see what they say

	$r=array();
	foreach($possibles as $p) {
		$thefile="../{$post_dir}/{$p}/index-new.xml";
		$dom = domxml_open_file($thefile) or die("Cannot open xml");
		$title_tag=$dom->get_elements_by_tagname("title");
		$title_tag=$title_tag[0]->get_content();

		if(!$created_tag=$dom->get_elements_by_tagname("created"))
			die("No created tag in $thefile");
		$created_tag=$created_tag[0]->get_content();
	
		$mt=transform_title($title_tag);
		if($t=="*" || $mt == $t) {
			$r[$p] = array(
				'filename' => $thefile,
				'title' => $title_tag,
				'title2' => $mt,
				'created' => strtotime($created_tag)
			);
		}
	}
//	$r['count'] = count($r);
	return $r;
}*/

function transform_title($s) {
#	echo "Transforming $s<br />\n";
	$rep = array(
		"/'/"=>"", //apostrophes
		"/&#8212;/"=>"-", //dashes of all sorts
		"/&#8211;/"=>"-",
		"/&mdash;/"=>"-",
		"/&ndash/"=>"-",
		"/&[#a-z0-9]+;/"=>"", //escapes
		"/[^a-zA-z0-9\-_]+/"=>"-",  //non-alphanumeric
		);
	$x=preg_replace(array_keys($rep),array_values($rep),$s);
	$x=preg_replace("/-+/","-",$x);
	$x=trim(strtolower($x),"-");
#	echo "Into $x<br /`>\n";
	return $x;
}


function daaolastmod($files=false) {
	$m=0;
	
	if($files===false) {
		$files=array();
	} else if (!is_array($files)) {
		$m=$files;
		$files=array();
	}
	$files[]=$_SERVER['SCRIPT_FILENAME'];
	$files[]=realpath("template.php");
	$files[]=realpath("_config.php");
	
	foreach($files as $f) {
		if(filemtime($f) > $m) $m=filemtime($f);
	}
		
	$tsstring = gmdate("D, d M Y H:i:s ", $m) . "GMT";
	
	$not_matching=false;
	if(isset($_SERVER["HTTP_IF_MODIFIED_SINCE"]) && ($_SERVER["HTTP_IF_MODIFIED_SINCE"] != $tsstring)) $not_matching = true;
	if(isset($_SERVER["HTTP_IF_NONE_MATCH"]) && ($_SERVER["HTTP_IF_NONE_MATCH"] != '"'.md5($timestamp).'"')) $not_matching = true;
	
	if((isset($_SERVER["HTTP_IF_MODIFIED_SINCE"]) || isset($_SERVER["HTTP_IF_NONE_MATCH"])) && !$not_matching) {
	  header("HTTP/1.1 304 Not Modified");
	  exit();
	} else {
	  header("Last-Modified: " . $tsstring);
	  header("ETag: \"".md5($timestamp)."\"");
	}

	header("Content-Type: text/html; charset=UTF-8");
	ob_start("contentlength");
}

function format_post($p, $full=true) {
	global $archive_root;
	$arch=$archive_root.date("Y/m/d/",$p['unixcreated']).transform_title($p['title']);
	
	$n=array(); //temp spot for storing dates
	$n['y'] = date("Y",$p['unixcreated']);
	$n['m'] = date("m",$p['unixcreated']);
	$n['F'] = date("F",$p['unixcreated']);
	$n['d'] = date("d",$p['unixcreated']);
	$yl=sprintf("<a href=\"{$archive_root}%04d\" title=\"Yearly archive for %d\">%d</a>", $n['y'], $n['y'], $n['y']);
	$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\" title=\"Monthly archive for %s %d\">%s</a>", $n['y'], $n['m'], $n['F'], $n['y'], $n['F']);
	$dl=sprintf("<a href=\"{$archive_root}%04d/%02d/%02d\" title=\"Daily archive for %s %d, %d\">%d</a>", $n['y'], $n['m'], $n['d'], $n['F'], $n['d'], $n['y'], $n['d']);
printf("<h3><span><a href=\"%s\" title=\"Permanent URL for this entry\" rel=\"bookmark\">%s</a></span></h3>
<p class=\"posted\">Posted on %s %s, %s at %s local</p>
<div class=\"postbody\">
", $arch, $p['title'], $dl, $ml, $yl, date("g:ia", $p['unixcreated']));


//	echo "<div class=\"entry\">\n<h3><a href=\"{$arch}\">{$p['title']}</a></h3>\n";
	$b=getpostheader($p['id']);
	foreach($b['item'][0]['body'] as $x) {
		echo $x['value'];
		if(!$full) break;
	}
	global $post_dir;
	$fn="{$post_dir}{$p['id']}/{$p['id']}.commentscounts.txt";
	$counts = 0;
	if(file_exists($fn)) {
		$c = include($fn);
		$counts = $c['approved']-0;
	}
	if($counts) {
		$counts .= " so far";
	} else {
		$counts = "None yet";
	}

printf("<p class=\"extra\">Posted by %s.  <a href=\"%s#comments\">Comments</a> (%s)</p>
</div>
", $b['item'][0]['author'][0]['value'], $arch, $counts);
}

function return_page($p) {
	global $err;
#	echo "Loading page $p<br />\n";
	global $page_dir;
	$p=strtolower($p);
	$fn=realpath($page_dir.$p.".xml");
	if(strpos($fn,$p) !== false) {
#		echo "Showing $fn<br />\n";
		//show the thing
		if(!file_exists($fn)) {
			$err="Specified file does not exist";
			return false;
		}
#		echo "Passed existance check<br />\n";
		if(!$parser = new XMLParser($fn, 'file', 0, 0)) {
			$err="Could not parse XML file (1)";
			return false;
		}
		if(!$tree = $parser->getTree()) {
			$err="Could not parse XML file (2)";
			return false;
		}
		return $tree;
	} else {
		$err="Invalid path.<br />\n";
		return false;
	}
}


function findpost($y,$m,$d,$t) {
	$n=getcache();
	$pat=sprintf("%04d-%02d-%02d", $y, $m, $d);
	$n=array_select($n, "/^{$pat}/");
	foreach($n as $v) {
		if($t == transform_title($v['title'])) {
			return $v;
		}
	}
	return false;
}

function recentposts($num,$skip=0) {
	$n=getcache();
	$n=array_flop($n,'unixcreated');
	ksort($n);
	$n=array_reverse($n);
	$n=array_slice($n,$skip,$num);
	return array_flop($n,'id');
}

function array_select($a, $preg, $from="key") {
	$r=array();
	foreach($a as $k=>$v) {
		if($from=="key") {
			$f=$k;
		} else if($v[$from]) {
			$f=$v[$from];
		} else {
			//um
			die("Bad key in $k");
			continue; //or return false
		}
		if(preg_match($preg,$f)) {
			$r[$k] = $v;
		}
	}
	return $r;
}

/*function titletourl($s) {
	$s=preg_replace("/[^a-z0-9]+/","-",strtolower($s));
	$s=trim($s,"-");
	return $s;
}*/


function array_lump($a,$n) {
	$r=array();
	$i=0;
	foreach($a as $k=>$v) {
		$i++;
		$r[$i % $n][$k]=$v;
	}
	return $r;
}

function array_sheepandlambs($a,$preg,$from="key") {
	$r=array();
	$mk="";
	foreach($a as $k=>$v) {
		if($from=="key") {
			$f=$k;
		} else if($v[$from]) {
			$f=$v[$from];
		} else {
			//um
			die("Bad key in $k");
			continue; //or return false
		}
		if(!preg_match($preg,$f,$m)) {
			//um
			die("Cannot match on $f");
			continue; //or return false
		}
		$mk=$m[1];
		$r[$mk][$k]=$v;
	}
	return $r;
}

function split_array($a,$p="%s",$from="key") {
	$r=array();
	foreach($a as $k=>$v) {
		if($from != "key") $k=$v[$from];
		if($s=sscanf($k, $p)) {
#			print_r($s);
			$b='$r';
			foreach($s as $i) {
				$b.="['{$i}']";
			}
			$b.='[$k]=$v;';
			eval($b);
		} else {
			$r['orphans'][$k] = $v;
		}
	}
	return $r;
}

/** subarray_count()
		parameters: a, level
		A is an array you wish to count
		Levels is the number of levels to _recurse_
	*/
function subarray_count($a,$levels=1) {
	$c=0;
	foreach($a as $v) {
		if(is_array($v) && $levels > 0) {
			$c+=subarray_count($v,$levels-1);
		} else {
			$c++;
		}
	}
	return $c;
}

function array_flop($a,$newkey) {
	$r=array();
	foreach($a as $v) {
		if(!isset($v[$newkey])) continue;
		$r[$v[$newkey]]=$v;
	}
	return $r;
}


function array_collapse($a,$numlevels=1,$ra=false,$prefix=false) {
	if($ra===false) $ra=array();

	foreach($a as $k=>$v) {
		if($numlevels==0) { $ra=$v; return $ra; }
#		echo "Got a key $k<br />\n";
		if($numlevels==1) {
			$ra[] = $v;
		} else {
			array_collapse($v,$numlevels-1,&$ra,$prefix);
		}
	}
	return $ra;
}

function sjoin($a,$b,$glue) {
	if($a != "" && $b != "") return $a.$glue.$b;
	return $a.$b;
}

function ordinal($n) {
	$c=abs($n)%10;
	$suffix=((abs($n) % 100 < 21 && abs($n) % 100 > 4) ? 'th' :
		(($c < 4) ? ($c < 3) ? ($c < 2) ? ($c < 1) ?
		'th':'st':'nd':'rd':'th'));
	return $n.$suffix;
}
$monthnames=array(1=>"January","February","March","April","May","June","July","August","September","October","November","December");


function array_set_current(&$array, $key) {
	reset($array);
	while(current($array)) {
		if(key($array) == $key) {
			break;
		}
		next($array);
	}
}

function array_has_next($a) {
	$k=key($a);
	next($a);
	$x= (key($a) !== false);
	return $x;
	if(is_array($a)) {
		if(next($a) === false) return false;
		return true;
	} else {
		return false;
	}
}

function array_has_prev($a) {
	prev($a);
	return (key($a) !== false);

		if(is_array($a)) {
		if(prev($a) === false) return false;
		return true;
	} else {
		return false;
	}
}

function var_save($fn,$a) {
	if(!$f=@fopen($fn,"wb")) return false;
	fputs($f,"<"."?"."php return ".var_export($a, true)."; ?".">");
	fclose($f);
	return true;
}

function rebuild_counts($a, $fn) {
	$c = array('approved' => 0, 'moderated' => 0);
	foreach($a as $v) {
		if($v['approved']) {
			$c['approved']++;
		} else {
			$c['moderated']++;
		}
	}
	var_save($fn, $c);
}


function array_push2(&$arr, $val) {
	$arr[] = $val;
	list(,$last_key) = each(array_reverse(array_keys($arr)));
	return $last_key;
}


?>
