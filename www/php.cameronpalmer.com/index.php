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
				<th>Picture</th><td><img src="i/ninja_cam.jpg" alt="Ninja Cameron" width="100" /></td>
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
		<h3>Program 2</h3>
			<form action="source.php" method="get">
			<ul>
				<?php $program = "labs/prog2/index.php";
				echo"
				<li><a href=\"". $ptr . $program . "\">Web Page</a></li>
				<li><a href=\"{$ptr}labs/prog2/source.php\" name=\"filename\">PHP Source</a></li>
				";
				?>
			</ul>
			
		<h3>Program 3</h3>
			<form action="source.php" method="get">
			<ul>
				<?php $program = "labs/prog3/index.php";
				echo"
				<li><a href=\"". $ptr . $program . "\">Web Page</a></li>
				<li><a href=\"{$ptr}labs/prog3/source.php\" name=\"filename\">PHP Source</a></li>
				";
				?>
			</ul>
			</form>