<?php
header("Content-Type: text/html; charset=utf-8");

$ptr="/";
$root_dir = dirname(__FILE__);
if (file_exists("{$root_dir}/php_root.php")) require("{$root_dir}/php_root.php");


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
if ($alternate_css and file_exists($alternate_css)) {
    echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"{$alternate_css}\" />\n";
}

echo'
    <link rel="shortcut icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
    <link rel="icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
</head>
<body>
    ';
    echo"
		<h1>{$title}</h1>
        ";

    echo'<p>This file was last updated: ' . date ('F d Y H:i:s.', getlastmod());
	echo'</p>';
    if ($section != "Welcome") {
        echo"
            <h2 class=\"leftside\">{$section}</h2>
            <a href=\"{$ptr}\" class=\"rightside\">Return Home</a>
        ";
    } else {
        echo "<h2>{$section}</h2>";
    }
}

function do_content($buf) {
  $x=$buf;

  return $buf;
}

function do_footer($buf) {
    return $buf.'
<div id="footer">
    Copyright &copy; 2005 Cameron Palmer<br />
    <a href="mailto:cameron.palmer@gmail.com">cameron.palmer@gmail.com</a>
</div>
</body>
</html>
';

}

?>