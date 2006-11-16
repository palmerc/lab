#!/usr/local/bin/php

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title>Lone Star Community</title>

<style type="text/css">
   html, body {
      height: 100%;
   }
   /* The entire page container */
	#container {
		width: 770px;
		height: 100%;
		font-size: 11px;
		color: #333;
		text-align: left;
		margin: 0 auto;
   }
   html>body #container {
      height: auto;
      min-height: 100%;
   }
	body {
		font-family: Geneva, Arial, Helvetica, sans-serif;
      background: #669  url(graygradient2.jpg) repeat-x top left;
		text-align: center;
	}
   /* Header contains topbanner and topnav */
	#header {
		float:left;
		clear: left;
		margin: 0;
		padding: 0;
      	width: 100%; 
	}
   #topbanner {
      float: left;
	  border-bottom-style: solid;
	  border-bottom-color:#FFCC00;
	  border-bottom-width: thin; 
   }
   #topbar a {
   	  text-decoration: none;
	  color: #333333;
	  }
   #topnav {
      float:left;
      background: #ffff66  url(graybuttoncopy.jpg) repeat-x top left;
      width: 100%;
	  border-top-color: #FFCC00;
	  border-top-style: solid;
	  border-top-width: medium;
   }
   #topnav ul {
      list-style: none;
   }
	#topnav li {
		float: left;
		padding: 0 2em 0.5em 1em;
		color: #332266;
		
   }
	#main {
      float: left;
   }
   /* Leftbar Calendar */
	#leftbar {
	float: left;
      margin: 0;
	  padding: 0;
      width: 168px;
      height: 600px;
	  background-color: #DDD;
	  border-color: #FFCC00;
	  border-style: solid;
	  border-width: thin;
	  border-right: none;

	  
   }
	#leftbar h1 {
      margin: 2em 0 1em .75em;
		font-size: 120%;
		color: #332266;
   }
   #leftbar dl {
   	padding:  0;
	margin: 0;
	color: #CC0000;
	}
	#leftbar dt {
		padding: 0 0 0 1em;
		margin: 0;
		clear: none;
		} 
	#leftbar dd {
	float: left;
	padding: 0;
	margin: 0;
	}
	#leftbar ul li {
		padding: 0 0.5em 0.3em 0;
		margin: 0;
		list-style-image:url(triangle.gif);
		color: #333;
		}
	#leftbar a {
		color: #333;
		}
   /* Rightbar Text */ 
	#rightbar {
		float: right;
		padding: 2em;
      width: 556px;
      height: 1756px;
		background-color:#FFF;
		border-color: #FFCC00;
	  border-style: solid;
	  border-width: thin;
	  border-left: none;

   }
	#rightbar h1 {
		font-size: 120%;
		color: #332266;
   }
   #footer {
      float: left;
      clear: both;
	    background: #003  url(purplebottom.jpg) repeat-x top left;
		border-top-color:#FFCC00;
		border-top-width: medium;
		border-top-style: solid;
	  height: 40px;
	  width: 100%;
	  color: #FFCC00;
   }
   #footer p {
   		padding-left: 2em;
		}
</style>
</head>

<body>
	
	<div id="container">
		<div id="header"> 
         <div id="topbanner">
			<img src="purpletopwtextslim.gif" alt="The Lone Star Community" width="770" height="90"/>
         </div>
         <div id="topnav">
            <ul id="topbar">
               <li><a href="lonestarwebsitepurplewcalendar.html">Home</a></li>
               <li><a href="votingmockup.html">Vote</a></li>
               <li>Create a Report</li>
               <li><a href="http://bug.cse.unt.edu/mss/eventscalendar.php?year=2006&month=11&day=">View Calendar</a></li>
			   <li>Update Personal Information</li>
               <li>Contact Us</li> 
            </ul>
         </div>
      </div>
	  <div id="main">
		  <div id="leftbar"><h1>alendar</h1>  
						<dt>Nov 7</dt>
						<dd><ul><li><a href="http://www.coba.unt.edu/programs/masters/infosession.php">STC meeting in Atalanta, GA</a></li>
						<li><a href="/news/view.php?/2006/11/02/logistics-student-association-%28logsa%29-meeting">Students for Semicolons meeting</a></li>
						</ul></dd>
						<dt>Nov 9 - Nov 10</dt>
						<dd><ul><li><a href="/news/view.php?/2006/07/27/logistics-management-seminar">Voting Session "The exclamation point: Do we Need it?"</a></li>
						</ul></dd>
						<dt>Nov 9</dt>
						<dd><ul><li><a href="/news/view.php?/2006/11/02/logistics-student-association-%28logsa%29-industry-day">Lone Star meeting in Fort Worth</a></li>
						</ul></dd>
						<dt>Nov 17</dt>
						
						<dd><ul><li><a href="http://www.murphycenter.unt.edu/leadershiplunch.php">2006 STC Leadership Luncheon</a></li>
						<li><a href="/news/view.php?/2006/11/02/1st-annual-unt-logistics-challenge">1st Annual Comma Challenge</a></li>
						</ul></dd>
						</dd>
		  </div>
		  <div id="rightbar">
            <h1>2006-07 Monthly Committee Report</h1>
	    <?php if($_POST['Report'] == 'Yes')
		{
			$to      = 'wng0001@unt.edu';
  			$subject = 'Form';
   			$message = 'Name: '.$_POST['select'].'\n'.'Committee: '.$_POST['select2'].'\n'.'Date of Submission: '.$_POST['textfield'].'\n';
			$q1 = '1. What tasks did your committee accomplish in the past month? \n'.$_POST['Q1'].'\n\n';
			
			$q2 = '2. What tasks do you have planned for the upcoming months? \n'.$_POST['Q2'].'\n\n';
			
			$q3 = '3. Did you hold any meetings? If so, what was the purpose and how many attended? \n'.$_POST['Q3'].'\n\n';
			
			$q4 = '4. Please list the name of the volunteers who helped you this month. Describe in detail what they did for you and how much time they spent performing these duties. Please provide as much information as possible so we can credit these volunteers with the Superstar points they deserve \n'.$_POST['Q4'].'\n\n';
			
			$q5 = '5. Are there any committee or community-related problems that the officers or Council should know about? \n'.$_POST['Q5'].'\n\n';
			
			$q6 = "6. Do you have any topics that need to be discussed during this month's council meeting? If so, please list them. \n".$_POST['Q6'].'\n\n';
			
			$q7 = '7. If you have anything to discuss during the council meeting, please indicate how much time you will need to discuss them. Please stay within your time limit. \n'.$_POST['Q7'].'\n\n';
			
			$q8 = '8. Do you have any materials that are CAA worthy? Please submit them with this form and indicate below what category your materials fulfill. \n'.$_POST['Q8'].'\n\n';
			
			$q9 = '9. Have you updated your page on the website? Please email the webmaster at webmaster@stc-dfw.org with any necessary changes. \n'.$_POST['Q9'].'\n\n';
			
			$q10 = '10. Please enter the name of the newsletter article that you have submitted to the newsletter editor this month. \n'.$_POST['Q10'].'\n\n';
			
			$q11 = '11. Will you need any special arrangements at the meeting (e.g., table, AV equipment)?  Please list necessary items here if applicable. \n'.$_POST['Q11'].'\n\n';
			
			$newmessage = $message.$q1.$q2.$q3.$q4.$q5.$q6.$q7.$q8.$q9.$q10.$q11;
   			
			$headers = 'From: wng0001@unt.edu' . "\r\n" .
      				'Reply-To: wng0001@unt.edu' . "\r\n" .
      				'X-Mailer: PHP/' . phpversion();
			
			$finalmessage = wordwrap ($newmessage, 70, "\n", 1);
			
			//echo $finalmessage;

   			$success = mail($to, $subject, $finalmessage, $headers);
   			if ($success == FALSE)
   			{				
     				echo 'ERROR: Call of mail() has failed.  Reason Unknown.';
   			}
   			else
   			{
     				echo '<p>Thank you for your submission. </p>';
   			}
			
		}
		else
		{
			$to      = 'wng0001@unt.edu';
  			$subject = 'Form';
   			$message = 'Name: '.$_POST['select'].'\n'.'Committee: '.$_POST['select2'].'\n'.'Date of Submission: '.$_POST['textfield'].'\n'.'Nothing To Report \n';
   			$headers = 'From: wng0001@unt.edu' . "\r\n" .
      				'Reply-To: wng0001@unt.edu' . "\r\n" .
      				'X-Mailer: PHP/' . phpversion();
			$finalmessage = wordwrap ($message, 70, "\n", 1);

   			$success = mail($to, $subject, $finalmessage, $headers);
   			if ($success == FALSE)
   			{
     				echo 'ERROR: Call of mail() has failed.  Reason Unknown.';
   			}
   			else
   			{
     				echo '<p>Thank you for your submission. </p>';
   			}
		}
	     ?>
            
            <p><br />
            </p>
            <p>&nbsp;</p>
            <p><br />
            </p>
            <p>
              <label></label>
              <label></label>
            </p>
            <p><br />
            </p>
            <p><br />
            </p>
        </div>
          <p>&nbsp;</p>
	  </div>
      <div id="footer">
	  	<p>      Lone Star Community, lonestar@stc.org</p>
      </div>
   </div>
</body>
</html>
