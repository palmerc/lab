<?php

function doheader($t="",$id="main") {
header("Content-Type: text/html; charset=utf-8");
echo '<'.'?xml version="1.0" encoding="utf-8"?>'."\n";
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title><?=$t?> (Tim Hatch)</title>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <link rel="shortcut icon" type="image/x-icon" href="/favicon.ico" />
  <link rel="alternate" title="RSS 2.0 Full Posts" href="/rss/2.0" />
  <link rel="stylesheet" href="/main.css" />
	<script type="text/javascript" src="/dom/annimg.js"></script>
	<link rel="stylesheet" href="/dom/annimg.css" />
	
</head>
<body id="www-timhatch-com">
<h1 class="hide">Tim Hatch</h1>
<div id="container_container"><div id="container">
<?php

}

function dofooter() {
?><!-- end entries -->

</div>
<div id="sidebar">
<h1><a href="/" title="Return Home">Tim<i> </i><span>Hatch</span></a></h1>
<h2>Navigation</h2>
<ul id="menu">
	<li><a href="/">Blog</a></li>
	<li><a href="/ark">Archives</a></li>
<!--	<li><a href="/photos/">Photos</a></li>
	<li><a href="/panos/">Panos</a></li>
	<li><a href="/hiring/">Hiring</a></li>-->
</ul>

<!--<h2>Depicted</h2>
<ul id="codepictions">
	<li><a href="#">Bird</a></li>
</ul>-->
</div>

</div></body></html>
<?php
}


function dolastmod($files=false) {
	$m=0;

	if($files===false) {
		$files=array();
	} else if(is_string($files)) {
		$files=array($files);
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

function contentlength($s) {
  $size = ob_get_length();
  header("Content-Length: {$size}");
  return $s;
}


function mprint_r($a) {
echo "<pre>";
print_r($a);
echo "</pre>";
}

function mdie($s) {
	header("HTTP/1.0 404 Not Found");
	doheader("Error", "err");
	echo "<div class=\"warn\">\n	<h3>Error</h3>\n	<p>";
	echo htmlentities($s);
	echo "</p>\n</div>\n\n";
	dofooter();
	exit();
}

function unserialize_file($fn) {
	return unserialize(file_get_contents($fn));
}

function serialize_file($fn,$a) {
	if(!$f=@fopen($fn,"wb")) return false;
	fputs($f,serialize($a));
	fclose($f);
	return true;
}

?>
