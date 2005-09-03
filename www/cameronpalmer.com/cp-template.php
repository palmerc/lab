<?php
header("Content-Type: text/html; charset=utf-8");
do_header();
ob_start("do_footer");
ob_start("do_content");

function do_header() {
  global $title, $section;
    echo '
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>';

echo "
    <title>{$title}</title>
";

echo '
    <link rel="stylesheet" type="text/css" href="c/main.css" />
    <link rel="shortcut icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
    <link rel="icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
</head>
<body>
';

}

function do_content($buf) {
  $x=$buf;
  
  return $buf;
}

function do_footer($buf) {
    return $buf.'
<div id="footer">
    Copyright &copy; 2005 Cameron Palmer<br />
    <a href="mailto:cameron@cameronpalmer.com">cameron@cameronpalmer.com</a>
</div>
</body>
</html>
';

}

?>