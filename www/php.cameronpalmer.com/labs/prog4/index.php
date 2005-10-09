<?php
	$title = "CSCE 2410 - PHP Program 5";
	$section = "Assignment: Specialized Functions";
	require("../../php-template.php");

$test_string1 = "This    is     a                                         test!!!";
$test_string2 = "Lorem ipsum and all that jazz.";
$test_array = array("The\n",
												"                        \n",
												"LAZY brown DOG\n",
												"               \n",
												"\n",
												"jumped       OVER  the   MOON\n");
$null_string = "";
$notso_null_string = '                        ';

echo'
<div class="form">
		';

strleading_tester();
strileading_tester();
strtrailing_tester();
stritrailing_tester();
explodeCount_tester();
removeMultipleBlanks_tester();
removeMultipleBlankLines_tester();

																		
echo'
</div>
		';

function display_tests($title, $data_array) {
	echo"
		<table class=\"matrix\">
			<tr>
				<th colspan=\"1\">{$title}</th>
			</tr>
			<tr>
				<th>Param1</th>
				<th>Param2</th>
				<th>Value Returned</th>
			</tr>
			";

	foreach ($data_array as $results_array) {
		list($param1, $param2, $result) = $results_array;
		echo"
			<tr>
				<td><pre>&ldquo;{$param1}&rdquo;</pre></td>
				<td><pre>&ldquo;{$param2}&rdquo;</pre></td>
				<td><pre>&ldquo;{$result}&rdquo;</pre></td>
			</tr>
				";
	}
	echo'
		</table>
			';
}

function strleading($string_in, $amtmatch) {
	if (is_string($amtmatch)) {
		$pattern = '/^'.$amtmatch.'/';
		if (preg_match($pattern, $string_in)) return true;
		else return false;
	} elseif (is_integer($amtmatch) and $amtmatch >= 0) {
		return substr($string_in, 0, $amtmatch);
	}	elseif (is_integer($amtmatch) and $amtmatch < 0) {
		return substr($string_in, $amtmatch, abs($amtmatch));
	} else {
		die ("strleading(): Error invalid parameter\n");
	}
}

function strileading($string_in, $amtmatch) {
	if (is_string($amtmatch)) {
		$pattern = '/^'.$amtmatch.'/i';
		if (preg_match($pattern, $string_in)) return true;
		else return false;
	} elseif (is_integer($amtmatch) and $amtmatch >= 0) {
		return substr($string_in, 0, $amtmatch);
	}	elseif (is_integer($amtmatch) and $amtmatch < 0) {
		return substr($string_in, $amtmatch, abs($amtmatch));
	} else {
		die ("strileading(): Error invalid parameter\n");
	}
}

function strtrailing($string_in, $amtmatch) {
	if (is_string($amtmatch)) {
		$pattern = '/'.$amtmatch.'$/';
		if (preg_match($pattern, $string_in)) return true;
		else return false;
	} elseif (is_integer($amtmatch) and $amtmatch >= 0) {
		return substr($string_in, -($amtmatch), $amtmatch);
	}	elseif (is_integer($amtmatch) and $amtmatch < 0) {
		return substr($string_in, 0, abs($amtmatch));
	} else {
		die ("strtrailing(): Error invalid parameter\n");
	}
}

function stritrailing($string_in, $amtmatch) {
	if (is_string($amtmatch)) {
		$pattern = '/'.$amtmatch.'$/i';
		if (preg_match($pattern, $string_in)) return true;
		else return false;
	} elseif (is_integer($amtmatch) and $amtmatch >= 0) {
		return substr($string_in, -($amtmatch), $amtmatch);
	}	elseif (is_integer($amtmatch) and $amtmatch < 0) {
		return substr($string_in, 0, abs($amtmatch));
	} else {
		die ("stritrailing(): Error invalid parameter\n");
	}
}

function removeMultipleBlanks($string_in) {
	return preg_replace('/\s\s+/', ' ',$string_in);
}

function explodeCount($string_in, $seperator, &$count, $limit=null) {
	if ($limit != null) {
		$data_array = explode($seperator, $string_in, $limit);
	} else {
		$data_array = explode($seperator, $string_in);
	}
	$count = count($data_array);
	return $data_array;
}

function removeMultipleBlankLines(&$string_array) {
	$count = 0;
  $count = count($string_array);
  //echo "<pre>" . print_r($string_array) . "</pre>";
  for ($i=0; $i < $count; $i++) {
		if (preg_match('/^\s*$/', $string_array[$i]) and preg_match('/^\s*$/', $string_array[$i+1])) {
			continue;
		}
		$data_array[] = $string_array[$i];
  }
  $string_array = $data_array;
  //echo "<pre>" . print_r($string_array) . "</pre>";
	$count -= count($data_array);

	return $count;
}

function strleading_tester() {
	global $test_string2;
	
	// strleading() tests
	$results_array = array($test_string2, "Lorem", 0);
	$results_array[2] = (strleading($results_array[0], $results_array[1]) ? 'true' : 'false');
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, "lorem", 0);
	$results_array[2] = (strleading($results_array[0], $results_array[1]) ? 'true' : 'false');
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, 5, 0);
	$results_array[2] = strleading($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, 0, 0);
	$results_array[2] = strleading($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, -5, 0);
	$results_array[2] = strleading($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	display_tests("strleading()", $data_array);
}

function strileading_tester() {
	global $test_string2;
	
	// strileading() tests
	$results_array = array($test_string2, "Lorem", 0);
	$results_array[2] = (strileading($results_array[0], $results_array[1]) ? 'true' : 'false');
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, "lorem", 0);
	$results_array[2] = (strileading($results_array[0], $results_array[1]) ? 'true' : 'false');
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, 5, 0);
	$results_array[2] = strileading($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, 0, 0);
	$results_array[2] = strileading($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, -5, 0);
	$results_array[2] = strileading($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	display_tests("strileading()", $data_array);
}

function strtrailing_tester() {
	global $test_string2;
	
	// strtrailing() tests
	$results_array = array($test_string2, "jazz.", 0);
	$results_array[2] = (strtrailing($results_array[0], $results_array[1]) ? 'true' : 'false');
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, "Jazz.", 0);
	$results_array[2] = (strtrailing($results_array[0], $results_array[1]) ? 'true' : 'false');
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, 5, 0);
	$results_array[2] = strtrailing($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, 0, 0);
	$results_array[2] = strtrailing($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, -5, 0);
	$results_array[2] = strtrailing($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	display_tests("strtrailing()", $data_array);
}

function stritrailing_tester() {
	global $test_string2;
	
	// stritrailing() tests
	$results_array = array($test_string2, "jazz.", 0);
	$results_array[2] = (stritrailing($results_array[0], $results_array[1]) ? 'true' : 'false');
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, "Jazz.", 0);
	$results_array[2] = (stritrailing($results_array[0], $results_array[1]) ? 'true' : 'false');
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, 5, 0);
	$results_array[2] = stritrailing($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, 0, 0);
	$results_array[2] = stritrailing($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	$results_array = array($test_string2, -5, 0);
	$results_array[2] = stritrailing($results_array[0], $results_array[1]);
	$data_array[] = $results_array;
	
	display_tests("stritrailing()", $data_array);
}

function removeMultipleBlanks_tester() {
	global $test_string1, $null_string, $notso_null_string;
	
	// removeMultipleBlanks() tests
	$results_array = array($test_string1, "n/a", "");
	$results_array[2] = removeMultipleBlanks($results_array[0]);
	$data_array[] = $results_array;

	$results_array = array($null_string, "n/a", "");
	$results_array[2] = removeMultipleBlanks($results_array[0]);
	$data_array[] = $results_array;

	$results_array = array($notso_null_string, "n/a", "");
	$results_array[2] = removeMultipleBlanks($results_array[0]);
	$data_array[] = $results_array;
	
	display_tests("removeMultipleBlanks()", $data_array);
}

function explodeCount_tester() {
	global $test_string2;
	
	$results_array = array($test_string2, "", 0);
	$results_array[1] = explodeCount($results_array[0], " ", $count);
	$results_array[2] = $count;
	$data_array[] = $results_array;

	$results_array = array($test_string2, "", 0);
	$results_array[1] = explodeCount($results_array[0], " ", $count, 4);
	$results_array[2] = $count;
	$data_array[] = $results_array;
			
	display_tests("explodeCount()", $data_array);
}

function removeMultipleBlankLines_tester() {
	global $test_array;

	// removeMultipleBlankLines() tests
	$results_array = array($test_array, "n/a", "");
	$results_array[2] = removeMultipleBlankLines($results_array[0]);
	$results_array[0] = implode($test_array);
	$data_array[] = $results_array;

	display_tests("removeMultipleBlankLines()", $data_array);
}

?>