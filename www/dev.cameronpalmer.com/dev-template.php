<?php
header("Content-Type: text/html; charset=utf-8");

$ptr="/";
$root_dir = dirname(__FILE__);
if (file_exists("{$root_dir}/dev_root.php")) require("{$root_dir}/dev_root.php");


do_header();
ob_start("do_footer");
ob_start("do_content");

function do_header() {
  global $title, $section, $alternate_css, $ptr;
    echo '
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>';

echo "
    <title>{$title}</title>
";

echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"".$ptr."c/main.css\" />\n";
if ($alternate_css) {
    echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"{$ptr}{$alternate_css}\" />\n";
}

echo'
    <link rel="shortcut icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
    <link rel="icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
</head>
<body>
    ';
    echo"
   <div id=\"page\">
    <div id=\"header\">
			<h1>{$title}</h1>
		</div>
        ";

    echo'<div id="dateline">This file was last updated: ' . date ('F d Y H:i:s.', getlastmod());
	echo'</div>';
    if ($section != "Welcome") {
        echo"
        		<div id=\"assignment\">
 	 	        	<h2>{$section}</h2>
  	        	<a href=\"{$ptr}\">Return Home</a>
  	        </div>
        ";
    } else {
        echo"
        		<div id=\"assignment\">
        			<h2>{$section}</h2>
        		</div>
        		";
    }
}

function do_content($buf) {
  $x=$buf;

  return $buf;
}

function do_footer($buf) {
    return $buf.'
		<div id="footer">
		   Copyright &copy; 2006 Cameron Palmer<br />
		   cameron DOT palmer AT gmail DOT com
		   <br style="clear: left;" />
		</div>
        <br style="clear: left;" />
	</div>
</body>
</html>
';

}

?>