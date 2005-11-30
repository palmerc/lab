<?php
	class Rational {
		// Really this should be protected. But that would be a problem in PHP 4.
		var $numerator, $denominator;
		
		function Rational()
		{
			// The lack of constructor overloading in PHP is retarded
			// I think I have found the Achilles heel of PHP, its' OOP capabilities suck.
			if (func_num_args() == 2) {
				// WEAK.
				$this->assign(array(func_get_arg(0),func_get_arg(1)));
			} else {
				$this->assign(func_get_arg(0));
			}
		}

		function assign($thing)
		{				
			if (is_object($thing)) {;
				$this->numerator = $thing->numerator;
				$this->denominator = $thing->denominator;	
			} else {
				$this->numerator = $thing[0];
				$this->denominator = $thing[1];
			}
            $gcd = $this->gcd($this->numerator, $this->denominator);
            $this->numerator = $this->numerator / $gcd;
            $this->denominator = $this->denominator / $gcd;
            $this->negative();
		}

		function display()
		{
			return $this->numerator . "/" . $this->denominator;
		}

		function add($obj)
		{
			$passen = $obj->numerator;
			$passed = $obj->denominator;

			$passen = $passen * $this->denominator;
			$passed = $passed * $this->denominator;

			$orign = $this->numerator * $obj->denominator;
			$origd = $this->denominator * $obj->denominator;

			$finaln = $orign + $passen;
			$finald = $passed;
			$gcd = $this->gcd($finaln, $finald);
			$finaln = $finaln / $gcd;
			$finald = $finald / $gcd;

			return new Rational($finaln, $finald);
		}

		function subtract($obj)
		{
			$passen = $obj->numerator;
			$passed = $obj->denominator;

			$passen = $passen * $this->denominator;
			$passed = $passed * $this->denominator;

			$orign = $this->numerator * $obj->denominator;
			$origd = $this->denominator * $obj->denominator;

			$finaln = $orign - $passen;
			$finald = $passed;
			$gcd = $this->gcd($finaln, $finald);
			$finaln = $finaln / $gcd;
			$finald = $finald / $gcd;

			return new Rational($finaln, $finald);
			}

		function multiply($obj)
		{
			$passen = $obj->numerator;
			$passed = $obj->denominator;

			$finaln = $passen * $this->numerator;
			$finald = $passed * $this->denominator;

			$gcd = $this->gcd($finaln, $finald);
			$finaln = $finaln / $gcd;
			$finald = $finald / $gcd;

			return new Rational($finaln, $finald);			
		}

		function divide($obj)
		{
			$passen = $obj->numerator;
			$passed = $obj->denominator;

			$finaln = $passed * $this->numerator;
			$finald = $passen * $this->denominator;

			$gcd = $this->gcd($finaln, $finald);
			$finaln = $finaln / $gcd;
			$finald = $finald / $gcd;

			return new Rational($finaln, $finald);
		}

        function negative()
        {
            if ($this->denominator < 0){
                $this->numerator = $this->numerator * -1;
                $this->denominator = $this->denominator * -1;
            }
        }

		function gcd($a, $b)
		{
			// Euclid's Algorithm - Copyright Euclid c. 300 BCE
			// According to Lamé it should find an answer within 5 times the smaller
			// number steps, where n >= 1.
			$x = abs($a);
			$y = abs($b);

			while ($y != 0) {
				$r = $x % $y;
				$x = $y;
				$y = $r;
			}
			return $x;
		}
	}

  $title = "CSCE 2410 - PHP Program 9";
  $section = "Assignment: Class-based Rational Numbers";
  require("../../php-template.php");

  echo'
		<div class="leftside">
			';
	echo"
		<form action=\"{$_SERVER['PHP_SELF']}\" method=\"post\">
			";
?>
			<div id="rational">
			<div class="inputbox">
				Rational Number 1:<input type="text" name="num1" size="5" value="<?php echo $_POST['num1']?>" />/<input type="text" name="den1" size="5" value="<?php echo $_POST['den1']?>" />
			</div>
			<select name="operation">
				<option<?php echo ($_POST['operation'] == "Add") ? ' selected' : ''; ?>>Add</option>
				<option<?php echo ($_POST['operation'] == "Subtract") ? ' selected' : ''; ?>>Subtract</option>
				<option<?php echo ($_POST['operation'] == "Multiply") ? ' selected' : ''; ?>>Multiply</option>
				<option<?php echo ($_POST['operation'] == "Divide") ? ' selected' : ''; ?>>Divide</option>
			</select>
			<div class="inputbox">
				Rational Number 2:<input type="text" name="num2" size="5" value="<?php echo $_POST['num2']?>" />/<input type="text" name="den2" size="5" value="<?php echo $_POST['den2']?>" />
			</div>
			<input type="submit" value="Calculate" />			
			</div>
		</form>
	</div>
	
<?php
	if ($_SERVER['REQUEST_METHOD'] == "POST") {
		$num1 = $_POST['num1'];
		$den1 = $_POST['den1'];
		$num2 = $_POST['num2'];
		$den2 = $_POST['den2'];
		$operation = $_POST['operation'];
		
		if ($num1 and $num2 and $den1 and $den2) {
			$rat1 = new Rational($num1, $den1);
			$rat2 = new Rational($num2, $den2);
			if ($operation == "Add") {
				$result = $rat1->add($rat2);
			}
			if ($operation == "Subtract") {
				$result = $rat1->subtract($rat2);
			}
			if ($operation == "Multiply") {
				$result = $rat1->multiply($rat2);
			}
			if ($operation == "Divide") {
				$result = $rat1->divide($rat2);
			}
			$display = $result->display();
		}
		echo "<div class=\"result\">The result is: {$display}.</div>";
	}
?>