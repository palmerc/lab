<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
   <title>Lone Star Community</title>
   <link rel="stylesheet" type="text/css" href="c/main.css" />
</head>

<body>
	<div id="container">
		<div id="header">
         <div id="topbanner">
            <img src="i/orangetopwtext2.gif" alt="The Lone Star Community" />
         </div>
<?php
//      $mysql_link = mysql_connect("localhost", "mysql_username", "mysql_password");
//      mysql_select_db($db, $mysql_link);
   if (($_POST['username'] == 'user') AND ($_POST['password'] == 'test')) {
        // you should inspect these variables before passing off to mySQL
//         $query = "SELECT user, pass FROM login ";
//         $query .= "WHERE user='$username' AND pass='$password'";
//         $result = mysql_query($query, $mysql_link);
//           if(mysql_num_rows($result)) {
//             // we have at least one result, so update the logged in datetime
//             $query = "UPDATE from login SET logged=SYSDATE()";
//             $query .= "WHERE user='$username' AND pass='$password' ";
//            mysql_query($query,$mysql_link);
//           } else {
//             print("Sorry, this login is invalid.");
//             exit;
//           }
?>
         <div id="topnav">
            <ul id="topbar">
               <li>Go Home</li>
               <li>Rock the Vote</li>
               <li>Create a Report</li>
               <li>View Calendar</li>
               <li>Contact Us</li>
            </ul>
         </div>
      </div>
		<div id="main">
			<div id="leftbar">
				<h1>Calendar</h1>
				<ul>
					<li>Sept. 30, 2006, Council Meeting</li>
					<li>Oct. 6, 2006, Reports Due</li>
					<li>Oct. 12, 2006, Meeting in Fort Worth. Mel Something-or-other discusses new website</li>
					<li>Oct. 16, 2006, Voting on new schedule opens</li>
					<li>Oct. 18, 2006, Voting on new schuedile closes</li>
				</ul>
			</div>
			<div id="rightbar">
            <h1>Lone Star Community Home</h1>
				<p>The Society for Technical Communication (STC) is the world's largest professional organization for people involved in technical communication. The Lone Star community (LSC) is one of the largest communities in the U.S., drawing members from all over the Dallas- Fort Worth Metroplex area.</p>
   			<p> We are writers, editors, graphic artists, web content managers, as well as usability experts, consultants, information managers, educators and students. We work in many industries including telecommunications, software, semiconductor, financial, medical, and transportation. The community provides leadership and direction for more than 350 members and promotes professional growth through meetings, workshops, seminars, conferences, mentoring, and networking.</p> 
			
				<h1>Administrative Council</h1>
				<p>An administrative council manages the LSC. The seven elected officers are the sole voting members with committee managers reporting to the council.</p>
            <p>The Council abides by the LSC and Society Bylaws. The Lone Star community exists under its charter from the Society and operates within the Society's policies. The LSC Bylaws and amendments to them are compatible with the Society's Bylaws and its operating policies.</p> 
            <p>The LSC Administrative Council typically meets at 6:15 p.m. on the first Thursday of each month at the LaMadeleine in Addison, 5290 Belt Line Road, Suite 112, Addison, TX 75240, 972-239-9051.</p>
				<p>Council meetings are open to all LSC members. Contact the officers or committee managers when you have questions or comments.</p>
	
            <h1>Survey Results</h1>
            <p>Lone Star community's survey results from 2005</p>
         </div>
      </div>
   <?php 
   } 
   else
   {
      echo'
      </div>
      <div id="main">
			<div id="leftbar">
			</div>

         <div id="rightbar"><pre>
         ';
         print_r($_POST);
         echo "</pre><form action=\"{$_SERVER['PHP_SELF']}\" method=\"post\">";
         echo'
         <table>
            <tr>
               <td>User:</td><td><input type="text" name="username" /></td>
            </tr>
            <tr>
               <td>Password:</td><td><input type="password" name="password" /><td>
            </tr>
         </table>
         <input type="submit" value="Submit" />
         </form>
         ';
      }
   ?>
         </div>
      </div>
      <div id="footer">
      &nbsp;
      </div>
   </div>   
</body>
</html>
