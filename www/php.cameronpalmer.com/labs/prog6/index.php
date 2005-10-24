<?php
	$title = "CSCE 2410 - PHP Program 6";
	$section = "Assignment: Quotation Display System";
	require("../../php-template.php");

  echo"
	  	<form method=\"post\" action=\"{$_SERVER['PHP_SELF']}\">
	  		<div class=\"leftside\">
	  			<p>
	  				Quotation filename: <input type=\"text\" name=\"filename\" size=\"40\" value=\"";
	  				
	if ($_POST['filename']) echo $_POST['filename'];
	echo "\" />
	  			</p>
	  			<p>
		  			Always build index? <input type=\"checkbox\" name=\"buildindex\"";
	if (isset($_POST['buildindex'])) echo " checked=\"checked\"";
	echo' />
		  			Limitation: <select name="limitation">
		  ';
		  $setoptions = array('No Limitations', 'Max Lines', 'Max Characters');
		  foreach($setoptions as $value) {
		  	echo '
		  			<option';
		  	if ($_POST['limitation'] == 'Max Lines' and $value == 'Max Lines') {
		  		echo ' selected="selected">';
		  	} else if ($_POST['limitation'] == 'Max Characters' and $value == 'Max Characters') {
		  		echo ' selected="selected">';
		  	} else {
		  		echo '>';
		  	}
		  	echo "{$value}</option>";
		  }

	echo"
			  		</select>
		  			Max Size: <input type=\"text\" name=\"maxsize\" size=\"4\" value=\"{$_POST['maxsize']}\" />
		  			Preformat output: <input type=\"checkbox\" name=\"preformat\"";
	if (isset($_POST['preformat'])) echo " checked=\"checked\"";
	echo " />
		  		</p>
		  		<p>
			  		<input type=\"submit\" name=\"submit\" value=\"Display 5 random quotes\" />
			  	</p>
	  		</div>
	  	</form>
	  	";

	if ($_SERVER['REQUEST_METHOD'] == "POST") {
		echo"
			<div class=\"leftside\">
			<hr />
				";
		// cleanup the filename and allow for .dat or no extension
		if (isset($_POST['filename'])) {
			//print_r($_POST);
			$passed_value = trim($_POST['filename']);
			if (preg_match('/\.dat$/', $passed_value)) { 
				$datfile = $passed_value;
			} else {
				$datfile = $passed_value . ".dat";
			}
	
			$indexfile = preg_replace('/\.dat$/', '.index', $datfile);
		}
		
		//var_dump($datfile);
		//var_dump($indexfile);
	
		if (file_exists($datfile)) {
			$handle = @fopen($datfile, "rb");
			if (isset($_POST['buildindex']) or !file_exists($indexfile)) {
				createIndexFile($datfile, $indexfile);
			}
		
			$index_handle = fopen($indexfile, "rb") or die("Unable to open {$indexfile}");
	
			if ($_POST['limitation'] == "Max Lines" and $_POST['maxsize'] > 0) {
				$maxsize = $_POST['maxsize'];
				$quote_array = selectQuotes(0, $maxsize);
				displayQuotes($quote_array);
			} else if ($_POST['limitation'] == "Max Characters" and $_POST['maxsize'] > 0) {
				// Added one for the new line character
				$maxsize = $_POST['maxsize'] + 1;
				$quote_array = selectQuotes($maxsize, 0);
				displayQuotes($quote_array);
			} else {
				$quote_array = selectQuotes(0, 0);
				displayQuotes($quote_array);
			}
			fclose($handle);
		} else {
			echo"
				<p style=\"color: red;\">Unable to open {$datfile}</p>
					";
		}
		echo"
			</div>
				";
	}

	echo"
			<div class=\"leftside\">
				<hr />
		  	Original quotation files:
	  		<ul>
		  		<li><a href=\"cats.dat\">cats.dat</a></li>
		  		<li><a href=\"quotes.dat\">quotes.dat</a></li>
		  		<li><a href=\"colors.dat\">colors.dat</a></li>
		  	</ul>
	  	</div>
	  	";


function createIndexFile($datfile, $indexfile) {
	global $handle;
	$whandle = fopen($indexfile, "w+");
	$line_offset = 0;
	while ($line = fgets($handle)) {
		if (preg_match('/^%/', $line)) {
			// remove 2 one for the newline and one for the %
			$ending_offset = ftell($handle) - 2;
			$write_line = "{$line_offset},{$line_count},{$char_count},{$ending_offset}\n";
			fwrite($whandle, $write_line);

			// Offset address
			$line_offset = ftell($handle);
			$char_count = 0;
			$line_count = 0;  
		
			continue;
		} else if (feof($handle)) {
			// remove 1 one for the newline
			$ending_offset = ftell($handle) - 1;
			$write_line = "{$line_offset},{$line_count},{$char_count},{$ending_offset}\n";
			fwrite($whandle, $write_line);

			// Offset address
			$line_offset = ftell($handle);
		
			continue;
		}
		
		$char_count += strlen($line);
		$line_count++;
	}
	fclose($whandle);
}

function selectQuotes($maxchars, $maxlines) {
	global $handle, $index_handle;
	// Read in the index to an array
	if ($maxchars > 0) {
		while ($line = fgets($index_handle)) {
			$quote_info = explode(',', $line);
			if ($quote_info[2] <= $maxchars) {
				$list_of_potentials[] = $line;
				continue;
			}
		}
	} else if ($maxlines > 0) {
		while ($line = fgets($index_handle)) {
			$quote_info = explode(',', $line);
			if ($quote_info[1] <= $maxlines) {
				$list_of_potentials[] = $line;
				continue;
			}
		}
	} else {
		while ($line = fgets($index_handle)) {
			$quote_info = explode(',', $line);
			$list_of_potentials[] = $line;
		}
	}
	$numofquotes = count($list_of_potentials);
	//echo "SIZE " . count($list_of_potentials);
	//echo "<pre>";
	//print_r($list_of_potentials);
	//echo "</pre>";
	// Choose 5 unique values within the range of quotes
	while (count($chosen_array) < 5) {
		$chosen_one = rand(0, $numofquotes - 1);
		$chosen_array[$chosen_one] = $chosen_one; 
	}

	foreach ($chosen_array as $the_one) {
		$line = $list_of_potentials[$the_one];	
		//echo "LINE: {$line}<br />";
		$quote_info = explode(',', $line);
		$starting_offset = $quote_info[0];
		$ending_offset = $quote_info[3] - $starting_offset;
		fseek($handle, $starting_offset);
		$quote_array[] = fread($handle, $ending_offset);
	}

	return $quote_array;
}

function displayQuotes($quote_array) {
	echo"
		<table class=\"quotetable\">
			";

	foreach($quote_array as $quote) {			
		echo"
			<tr>
				<td>
				";
		if (isset($_POST['preformat'])) {
			echo"
					<pre>{$quote}</pre>
					";
		} else {
			echo"
					{$quote}
					";
		}

		echo"
				</td>
			</tr>
				";
	}
	
	echo"
		</table>
			";
}

?>