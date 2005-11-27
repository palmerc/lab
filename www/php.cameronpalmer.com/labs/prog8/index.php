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
		$page_array = @file($urlfile) or die("Unable to fetch page");
		$page_string = implode('', $page_array);
		// I didn't want a bunch of unsightly spaces. Besides a web browser collapses them.
		$page_string = preg_replace('/\s\s+/', ' ', $page_string);

		preg_match_all('#(<\w+[^>]*/{0,1}>)|(</[\w]+[^>]*>)|(\w+[^<]*)#', $page_string, $matches);
		
		// So basically I went to a lot of trouble here for no apparent reason. Well I just wanted to operate on a
		// stack. So there. Nanny nanny boo boo.
		$stack = array();

		foreach ($matches[0] as $token) {
			array_push($stack, $token);
		}
?>
<?php
		while ($item = array_shift($stack)) {
			if (preg_match('/<meta\s.*content=(.*)[^>]>/i', $item, $content)) {
				$content_collector[] = $content[1];
			}
			if (preg_match('/<title>/i', $item)) {
				$title = array_shift($stack);
			}

			if (preg_match('/<table/i', $item)) {
				array_unshift($stack, $item);
				$table_info[] = tabler($stack);
			}
		}
		echo "<p>URL: {$urlfile}<br />\n";
		echo "Title is: {$title}<br />\n";
		foreach ($content_collector as $piece) {
			echo "Content field: {$piece}<br />\n";
		}
		echo "</p>";
		
		echo "<p>Table Information:<br />\n";
		echo "Number of Tables that are not subtables: " . count($table_info) . "<br />\n";
		$k = 1;
		foreach ($table_info as $subtable) {
			echo "<br />\n";
			echo "<strong>Table {$k}:</strong><br />";
			if ($subtable[1]['row'] > 0) {
				echo "Row(s) " . $subtable[1]['row'] . "<br />\n";
			}
			if ($subtable[1]['columns'] > 0) {
				echo "Column(s) " . $subtable[1]['columns'] . "<br />\n";
			} 
			if ($subtable[1]['anchors'] > 0) {
				echo "Anchor(s) " . $subtable[1]['anchors'] . "<br />\n";
			}
			if ($subtable[1]['images'] > 0) {
				echo "Image(s) " . $subtable[1]['images'] . "<br />\n";
			}
			
			$j = 1;
			echo "<blockquote>\n";
			for ($i=2; $i < count($subtable) + 1; $i++) {
				echo "<strong>subTable {$j}:</strong><br />\n";
				if ($subtable[$i]['row'] > 0) {
					echo "Row(s) " . $subtable[$i]['row'] . "<br />\n";
				}
				if ($subtable[$i]['columns'] > 0) {
					echo "Column(s) " . $subtable[$i]['columns'] . "<br />\n";
				} 
				if ($subtable[$i]['anchors'] > 0) {
					echo "Anchor(s) " . $subtable[$i]['anchors'] . "<br />\n";
				}
				if ($subtable[$i]['images'] > 0) {
					echo "Image(s) " . $subtable[$i]['images'] . "<br />\n";
				}
				$j++;
			}
			echo "</blockquote>\n";
			$k++;
		}
			echo "</p>";
//		echo "<pre>";
//		print_r($table_info);
//		echo "</pre>";
	}

function tabler(&$stack) {
	while($item = array_shift($stack)) {
		if (preg_match('/<table/i', $item)) {
			$table_count++;
			$tables_open++;
		} else if (preg_match('/<\/table/i', $item)) {
			$tables_open--;			
			if ($tables_open == 0) {
				return $table_array;
			}
		} else if (preg_match('/<tr/i', $item)) {
			$columns = 0;
			$table_array[$table_count]['row'] += 1;
		} else if (preg_match('/<\/tr/i', $item)) {
			if ($table_array[$table_count]['columns'] < $columns) {
				$table_array[$table_count]['columns'] = $columns;
			}
		} else if (preg_match('/<td/i', $item)) {
			$columns++;
		} else if (preg_match('/<a/i', $item)) {
			$table_array[$table_count]['anchors'] += 1;
		} else if (preg_match('/<img/i', $item)) {
			$table_array[$table_count]['images'] += 1;
		}
	}

}
	
?>