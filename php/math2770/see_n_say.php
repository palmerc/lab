<?php
	$number_array = array('1');

	echo "1\n";
	for ($i=0; $i < 15; $i++) {
		$number_array = see_n_say($number_array);
		echo implode(",", $number_array) . "\n";
	}

	// And the cow says "mooooooooooo"
	function see_n_say($sequence){
		$digit = 0;
		$next = 0;
		$freq = 1;
		
		//foreach ($sequence as $digit) {
		for ($i=0; $i < count($sequence); $i++){ 
			$digit = $sequence[$i];
			($sequence[$i + 1]) ? ($next = $sequence[$i + 1]) : ($next = 0);
			// echo "Next! {$i} {$next} \n";
			// echo "Processing: {$digit}\n";
			if ($digit == $next) {
				// echo "Last is equal to current\n";
				$freq++;
			} else {
				if ($next != 0 and $freq == 0) $freq++;
				$temp_array[] = $freq;
				$temp_array[] = $digit;
				//echo "{$freq},{$digit}\n";
				$freq = 1;
			}
		} // end foreach

		return $temp_array;
	}
?>
