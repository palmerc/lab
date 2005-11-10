<?php
	$title = "CSCE 2410 - PHP Programming Lab";
	$section = "Welcome";
	require("php-template.php");
?>
		<h2>Personal Details</h2>
		<table>
			<tr>
				<th>Student</th><td><strong>Cameron L Palmer</strong></td>
			</tr>
			<tr>
				<th>Picture</th><td><img src="i/ninja_cam.jpg" alt="Ninja Cameron" width="128" /></td>
			</tr>
			<tr>
				<th>Email</th><td><a href="mailto:cameron.palmer@gmail.com">cameron.palmer@gmail.com</a></td>
			</tr>
		</table>

		<h2>About Me</h2>
		<div id="about">
		<p>I work for the College of Business on the Web Development Team. I was an employee of
		Seagate Technology, San Jose, CA and Bank of America, Dallas, TX where I did Unix system
		administration and firewall security.</p>
		<p>I am taking this class hoping to sharpen my PHP skills and get a better handle on
		MySQL and OO programming with PHP.</p>
		<p>For this assigment I will use Color, Font, Font-Size, and Weight via CSS</p>
		</div>

		<h2>Programs</h2>
		<h3>Program 2 - Generate a Static Multiplication Table</h3>
			<ul>
				<?php $program = "labs/prog2/index.php";
				echo"
				<li><a href=\"". $ptr . $program . "\">Web Page</a></li>
				<li><a href=\"{$ptr}labs/prog2/source.php\" name=\"filename2\">PHP Source</a></li>
				";
				?>
			</ul>
			
		<h3>Program 3 - Generate a Dynamic Multiplication Table</h3>
			<ul>
				<?php $program = "labs/prog3/index.php";
				echo"
				<li><a href=\"". $ptr . $program . "\">Web Page</a></li>
				<li><a href=\"{$ptr}labs/prog3/source.php\" name=\"filename3\">PHP Source</a></li>
				";
				?>
			</ul>

		<h3>Program 4 - Matrices</h3>
			<ul>
				<?php $program = "labs/prog4/index.php";
				echo"
				<li><a href=\"". $ptr . $program . "\">Web Page</a></li>
				<li><a href=\"{$ptr}labs/prog4/source.php\" name=\"filename4\">PHP Source</a></li>
				";
				?>
			</ul>
		<h3>Program 5 - Specialized Functions</h3>
			<ul>
				<?php $program = "labs/prog5/index.php";
				echo"
				<li><a href=\"". $ptr . $program . "\">Web Page</a></li>
				<li><a href=\"{$ptr}labs/prog5/source.php\" name=\"filename5\">PHP Source</a></li>
				";
				?>
			</ul>
		<h3>Program 6 - Quotation Display System</h3>
			<ul>
				<?php $program = "labs/prog6/index.php";
				echo"
				<li><a href=\"". $ptr . $program . "\">Web Page</a></li>
				<li><a href=\"{$ptr}labs/prog6/source.php\" name=\"filename6\">PHP Source</a></li>
				";
				?>
			</ul>
		<h3>Program 7 - Image Uploading</h3>
			<ul>
				<?php $program = "labs/prog7/index.php";
				echo"
				<li><a href=\"". $ptr . $program . "\">Web Page</a></li>
				<li><a href=\"{$ptr}labs/prog7/source.php\" name=\"filename7\">PHP Source</a></li>
				";
				?>
			</ul>
		<h3>Program 8 - Web Page Parsing</h3>
			<ul>
				<?php $program = "labs/prog8/index.php";
				echo"
				<li><a href=\"". $ptr . $program . "\">Web Page</a></li>
				<li><a href=\"{$ptr}labs/prog8/source.php\" name=\"filename8\">PHP Source</a></li>
				";
				?>
			</ul>
		<h3>Final Program - SQL Web Calendar</h3>
			<ul>
				<?php $program = "labs/final/index.php";
				echo"
				<li><a href=\"". $ptr . $program . "\">Web Page</a></li>
				<li><a href=\"{$ptr}labs/final/source.php\" name=\"final\">PHP Source</a></li>
				";
				?>
			</ul>