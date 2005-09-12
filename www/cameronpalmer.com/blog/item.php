<?php
require_once("template.php");
require_once("funcs.php");

$being_nice = true;

$archive_root="/blog/ark/";

/* This part is the individual archive */
$y=$_GET['y']-0;
$m=$_GET['m']-0;
$d=$_GET['d']-0;
$t=$_GET['t'];

if(!$p=findpost($y, $m, $d, $t))
	die("Cannot find specified post");

if(isset($_REQUEST['comment-submit']) && comments_enabled($p['id'])) {
	require_once("safehtml.class.php");
	$checker = new SafeHtmlChecker;
	$checker->check('<all>'.$_REQUEST['comment-data'].'</all>');
	if ($checker->isOK()) {
		//echo 'Everything is fine';
	} else {
		echo '<ul class=\"errors\">';
		foreach ($checker->getErrors() as $error) {
			echo '<li>'.$error.'</li>';
		}
		echo '</ul>';
		echo "<p>Please hit back, fix those errors, and resubmit.  If the Javascript
		validator didn't catch them, please report it to me via email.  my first name @ my first name my last name dot com.</p>";
		die();
	}
	//verify validity, mark as moderated no matter what if it's not valid
	if(!strlen($_REQUEST['comment-name']))
		die("You must enter your name.");
	$c=array(
		"name" => $_REQUEST['comment-name'],
		"comment" => $_REQUEST['comment-data'],
		"email" => $_REQUEST['comment-email'],
		"letter" => $_REQUEST['comment-letter'],
		"url" => $_REQUEST['comment-url'],
		"timestamp" => time(),
		"ip" => $_SERVER['REMOTE_ADDR'],
		"approved" => 0,
	);
	$has_email = (strlen($_REQUEST['comment-email']) && strlen($_REQUEST['comment-letter']) == 1);
	if($has_email) $email_letter = $_REQUEST['comment-email']."_".$_REQUEST['comment-letter'];
	
	$emails_and_letters = array();
	if(file_exists("emails.txt")) {
		$emails_and_letters = include("emails.txt");
		if($checker->isOK() && $has_email && isset($emails_and_letters[$email_letter]) && $emails_and_letters[$email_letter]['status'] == 1) {
			//no need to moderate
			$c['approved'] = 1;
			$emails_and_letters[$email_letter]['posts'][$p['id']]++;
		}
	}
//	global $being_nice;
	if($being_nice) {
		$c['approved'] = 1;
	}
	
	if(!isset($emails_and_letters[$email_letter])) {
		$emails_and_letters[$email_letter] = array(
			"posts" => array($p['id'] => 1),
			"approved" => 0,
		);
	}
	var_save("emails.txt", $emails_and_letters);
	
	$fn="{$post_dir}{$p['id']}/{$p['id']}.comments.txt";
	$comments = array();
	if(file_exists($fn)) {
		$comments = include($fn);
	}
	$last = array_push2($comments, $c);
	rebuild_counts($comments,"{$post_dir}{$p['id']}/{$p['id']}.commentscounts.txt");
	var_save($fn,$comments);
	mail("cameron@cameronpalmer.com",
		"[comment] {$p['ip']} to {$p['title']}",
		"There has been a comment posted to {$p['title']} from by {$p['name']} from ip {$c['ip']} at ".date("Y-m-d H:i:s").".
Comment itself is {$c['comment']}
	
- Approve http://cameronpalmer.com/approve_comment.php?magic=blah&a=app&id=${p['id']}&n=$last
- Delete http://cameronpalmer.com/approve_comment.php?magic=blah&a=del&id=${p['id']}&n=$last
".($c['approved'] ? "Approved." : "DENIED"));

	header("HTTP/1.0 302 Follow Redirect");
	header("Location: http://{$_SERVER['HTTP_HOST']}{$_SERVER['REQUEST_URI']}#comment_{$last}");
	exit();
	
}

doheader($p['title'],'post');
/*	$x['y'] = $y; #date("Y",strtotime($dayname));
	$x['m'] = $m; #date("m",strtotime($dayname));
	$x['F'] = date("F",$p['unixcreated']);
	$x['d'] = $d; #date("d",strtotime($dayname));
	$yl=sprintf("<a href=\"{$archive_root}%04d\">%d</a>", $x['y'], $x['y']);
	$ml=sprintf("<a href=\"{$archive_root}%04d/%02d\">%s</a>", $x['y'], $x['m'],$x['F']);
	$dl=sprintf("<a href=\"{$archive_root}%04d/%02d/%02d\">%d</a>", $x['y'], $x['m'], $x['d'], $x['d']);
	$nicedayname="{$ml} {$dl}, {$yl}";
	echo "<div class=\"day\"><h2>$nicedayname</h2>\n";*/

#echo "<p class=\"posted\">Posted on <a href=\"/ark/$y/$m\">{$monthnames[$m]}</a> <a href=\"/ark/$y/$m/$d\">".ordinal($d)."</a>, <a href=\"/ark/$y\">$y</a></p>\n";

format_post($p);
echo "<h3><span>Comments For This Post</span></h3>\n";
if(comments_enabled($p['id'])) {
	echo "<div id=\"comments\">\n";
	$fn="{$post_dir}{$p['id']}/{$p['id']}.comments.txt";
	if(file_exists($fn)) {
		$comments = include($fn);
		foreach($comments as $id=>$c) {
			if($c['approved']) {
				printf("<div class=\"comment\" id=\"comment_%d\"><img src=\"%s\" width=\"32\" height=\"32\" class=\"gravatar\" />
	<h4 class=\"commentheader\"><a href=\"%s\">%s</a> said... (<a href=\"#comment_%d\">#%d</a>)</h4>
	<div class=\"commentdata\">
		%s
	</div>
</div>", $id, "http://www.gravatar.com/avatar.php?size=32&amp;gravatar_id=".md5($c['email']), $c['url'], $c['name'], $id, $id, $c['comment']);
			} else {
				printf("<div id=\"comment_%d\">Unapproved comment #%d (awaiting moderation)</div>\n", $id, $id);
			}
		}
	}
	echo "<form id=\"commentform\" action=\"{$_SERVER['REQUEST_URI']}\" method=\"post\"><div class=\"stupid\">
<label for=\"comment-data\">Comment</label> (required, shown publicly)<br />
<textarea id=\"comment-data\" name=\"comment-data\" rows=\"4\" cols=\"40\">&lt;p&gt;Comment here&lt;/p&gt;</textarea><br />
<p>Allowed tags: <code>p</code>, <code>a href</code>, <code>code</code>, <code>em</code>, <code>strong</code>, <code>del</code>, <code>ins</code>, <code>dl</code>, <code>dt</code>, <code>dd</code> (autovalidated and autopreviewed below)</p>

<div id=\"preview\">Preview goes here (Javascript disabled?)</div>
<div id=\"result\">Result goes here (Javascript disabled?)</div>
<div id=\"tree\">Parse tree goes here (Javascript disabled?)</div>

<label for=\"comment-name\">Name</label> (required, shown publicly)<br />
<input type=\"text\" id=\"comment-name\" name=\"comment-name\" /><br />

<label for=\"comment-name\">URL</label> (recommended, shown publicly)<br />
<input type=\"text\" id=\"comment-url\" name=\"comment-url\" /><br />

<label for=\"comment-email\">Email</label> (recommended, see <a href=\"#comment-note\">note below</a>)<br />
<input type=\"text\" id=\"comment-email\" name=\"comment-email\" /><br />

<label for=\"comment-letter\">Favorite Letter</label> (recommended, see <a href=\"#comment-note\">note below</a>)<br />
<input type=\"text\" id=\"comment-letter\" name=\"comment-letter\" size=\"1\" /><br /><br />

<input type=\"submit\" id=\"comment-submit\" name=\"comment-submit\" value=\"Add Comment\" /> (preview above first)

<p id=\"comment-note\">The only information which is <em>required</em> is the
comment itself and your name.  All first-time comments by a given email address
(and those who have chosen to not provide an email address) will be moderated,
due to the possibility for abuse.  Your favorite letter is merely a key to give
a reasonable probability of people not pretending to be you (because they will
not likely pick the same letter).</p>

<p>Hopefully these antispam measures are benign enough for you, as I welcome
comments and despise registrations myself.  Your email address will <em>never</em>
be shown publicly, but will be used to fetch your <a href=\"http://gravatar.com/\">gravatar</a>
and for any response I wish to give directly.</p>

</div></form>
";
	echo "</div>\n";
}
dofooter();
exit();


function comments_enabled($postid) {
	$data = getpostheader($postid);
	if(isset($data['comments'][0]['value'])) {
		return ($data['comments'][0]['value']!="closed");
	}
	return true;
}

?>
