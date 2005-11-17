<?php
  $title = "CSCE 2410 - PHP Program 8";
  $section = "Assignment: Web Page Parsing";
  require("../../php-template.php");

	echo'
	<div class="leftside">
			';
	echo"
		<form action=\"{$_SERVER['PHP_SELF']}\" method=\"post\">
			";
	echo'
			<p>
			Enter URL: <input type="text" name="urlfile" />
			</p>
			<p>
			<input type="submit" value="Submit" />
		</form>
	</div>
			';
	echo'
		<div class="leftside">
		<hr />
			';


	if ($_SERVER['REQUEST_METHOD'] == "POST") {
		$urlfile = $_POST['urlfile'];
		$page_array = file($urlfile);
		$page_string = implode('', $page_array);
		// I didn't want a bunch of unsightly spaces. Besides a web browser collapses them.
		$page_string = preg_replace('/\s\s+/', ' ', $page_string);
//		$page_string = preg_replace('/\n/', '', $page_string);
	
//		echo '<pre>';
//		echo strlen($page_string) . ": ";
//		$page_string = nl2br($page_string);
//		echo htmlspecialchars($page_string) . "\n";				
//		echo '</pre>';
//		echo '<hr />';

		preg_match_all('#(<\w+[^>]*/*>)|(</[\w]+[^>]*>)|(\w+[^<]*)#', $page_string, $matches);
		
//		echo '<pre>';
//		print_r($matches);

		// So basically I went to a lot of trouble here for no apparent reason. Well I just wanted to operate on a
		// stack. So there. Nanny nanny boo boo.
		$stack = array();
		foreach ($matches[0] as $token) {
			array_push($stack, $token);
//			echo htmlspecialchars($token) . "\n";
		}
//		echo '</pre>';

		while ($item = array_pop($stack)) {
			if (preg_match('/<meta .*content="?(.+)"?/', $item, $content)) {
				echo "Content field: {$content[1]}<br />";
			}
			if ($item == "</title>") {
					$title = array_pop($stack);
					// Pop off the opening title tag
					array_pop($stack);
			}

			if ($item == "</table>") {
			}
		}
		echo "Title is: {$title}.<br />\n";
	}
?>