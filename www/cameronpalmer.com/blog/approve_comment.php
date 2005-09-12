<?php
require("funcs.php");

//normally this will be called from an email link, with ?id=2004-05-01.01&n=4, to list, with the option to approve

if($_REQUEST['magic'] == "blah") {
	//approve
	$fn="{$post_dir}{$_REQUEST['id']}/{$_REQUEST['id']}.comments.txt";
	if(!file_exists($fn)) {
		die("There are no comments on this post.");
	}
	$comments = include($fn);

	if($_REQUEST['a'] == "app") {
		$comments[$_REQUEST['n']]['approved'] = 1;
		$c = $comments[$_REQUEST['n']];
/*		$has_email = (strlen($c['email']) && strlen($c['letter']) == 1);
		if($has_email) $email_letter = $_REQUEST['comment-email']."_".$_REQUEST['comment-letter'];

		$emails_and_letters = include("emails.txt");
		$emails_and_letters
		var_save("emails.txt", $emails_and_letters);*/
	} else if ($_REQUEST['a'] == "del") {
		unset($comments[$_REQUEST['n']]);
	} else {
		die("Huh?");
	}
	rebuild_counts($comments,"{$post_dir}{$_REQUEST['id']}/{$_REQUEST['id']}.commentscounts.txt");
	var_save($fn,$comments);
} else {
	die("Set the cookie first.");
}



?>
