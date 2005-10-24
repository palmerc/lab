<?php
	$title = "CSCE 2410 - PHP Program 6";
	$section = "Assignment: Quotation Display System";
	require("../../php-template.php");

if ($_SERVER['REQUEST_METHOD'] == "POST") {

	// cleanup the filename and allow for .dat or no extension
	if (isset($_POST['filename'])) {
		print_r($_POST);
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

	if (isset($_POST['buildindex']) or !file_exists($indexfile)) {
		//echo "<p class=\"leftside\">Opening handle</p>";
		$handle = fopen($datfile, "rb");

		createIndexFile($datfile, $indexfile, $handle);
	}

	$index_handle = fopen($indexfile, "rb");

	echo"
		<table class=\"progtable\">
			";
	while ($line = fgets($index_handle)) {
		$quote_info = explode(',', $line);
		$starting_offset = $quote_info[0];
		$ending_offset = $quote_info[3] - $starting_offset;
		fseek($handle, $starting_offset);
		$quote = fread($handle, $ending_offset);
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
} else {
  echo"
	  	<form method=\"post\" action=\"{$_SERVER['PHP_SELF']}\">
	  		<div class=\"leftside\">
	  			<p>
	  				Quotation filename: <input type=\"text\" name=\"filename\" size=\"40\" value=\"\" />
	  			</p>
	  			<p>
		  			Always build index? <input type=\"checkbox\" name=\"buildindex\" />
		  			Limitation: <select name=\"limitation\">
		  										<option>No Limitation</option>
		  										<option>Max Lines</option>
			  									<option>Max Characters</option>
			  								</select>
		  			Max Size: <input type=\"text\" size=\"4\" value=\"\" />
		  			Preformat output: <input type=\"checkbox\" name=\"preformat\" />
		  		</p>
		  		<p>
			  		<input type=\"submit\" name=\"submit\" value=\"Display 5 random quotes\" />
			  	</p>
	  		</div>
	  	</form>
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
}

function createIndexFile($datfile, $indexfile, $handle) {
	$whandle = fopen($indexfile, "w+");
	$line_offset = 0;
	while ($line = fgets($handle)) {
		$char_count += strlen($line);
		$line_count++;
		if (preg_match('/^%/', $line)) {
			$ending_offset = ftell($handle);
			$write_line = "{$line_offset},{$line_count},{$char_count},{$ending_offset}\n";
			fwrite($whandle, $write_line);

			// Offset address
			$line_offset = ftell($handle);
			$char_count = 0;
			$line_count = 0;  
		
			continue;
		}
	}
	fclose($whandle);
}



?>