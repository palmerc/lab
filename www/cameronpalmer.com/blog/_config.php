<?php

/* This file is prepended, so it's a good place */
ob_start("ob_gzhandler");


$database_path="db/";
$post_dir=$database_path."posts/";
$page_dir=$database_path."pages/";

$archive_root="/blog/ark/";

function strips(&$el) {
	if(is_array($el)) {
		foreach($el as $k=>$v) {
			if($k != 'GLOBALS') strips($el[$k]);
		}
	} else {
		$el = stripslashes($el);
	}
}

if (get_magic_quotes_gpc()) strips($GLOBALS);


?>