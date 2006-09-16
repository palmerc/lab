<?php
	$title = "CSE 4410 - Software Development 1";
	$section = "Welcome";
	require("dev-template.php");
?>
	<div id="frontpage">
		<div class="leftside">
			<h2>Personal Details</h2>
			<table style="float: left; clear: left;">
				<tr>
					<th>Student</th><td><strong>Cameron L Palmer</strong></td>
				</tr>
				<tr>
					<th>Picture</th><td><img src="i/ninja_cam.jpg" alt="Ninja Cameron" width="128" /></td>
				</tr>
				<tr>
					<th>Email</th><td>cameron DOT palmer AT gmail DOT com</td>
				</tr>
			</table>
		</div>

		<div class="leftside">
			<h2>About Me</h2>
			<p>I work for Dr. Sweany doing Compiler research. I am Senior Computer 
         Science student and have already taken the PHP programming lab.</p>
			<p>For this assigment I will use Color, Font, Font-Size, and Weight via CSS</p>
		</div>
      		
      <div id="programlist">
         <h2>Assignments</h2>

         <h3>Familiarity Assignment 2</h3>
         <ul>
            <?php $program = "labs/assignment2/index.php";
            echo"
            <li><a href=\"". $ptr . $program . "\">Web Page</a></li>
            <li><a href=\"{$ptr}labs/assignment2/source.php\" name=\"filename2\">PHP Source</a></li>
               ";
            ?>
         </ul>
      </div>
  </div>