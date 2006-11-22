<?php
require('database.php');
require('user.php');
session_start();
if (!$_SESSION['email']) 
   header('location:login.php');
$user_id = $_SESSION['email'];
$first = $_SESSION['first_name'];

database_connect();
$result = retrieve_user($user_id);
$admin = $result[0]['admin'];
database_disconnect();

   $title = "Lone Star Community - Main Page";
   $leftbar = "leftbar.php";
   require("stc-template.php");
?>
<p>Hello, <?php echo $first; ?>! </p>

<h1>Lone Star Community Home</h1>
<p>The Society for Technical Communication (STC) is the world's largest 
professional organization for people involved in technical communication. The 
Lone Star community (LSC) is one of the largest communities in the U.S., drawing 
members from all over the Dallas- Fort Worth Metroplex area.</p>
<p>We are writers, editors, graphic artists, web content managers, as well as 
usability experts, consultants, information managers, educators and students. We
work in many industries including telecommunications, software, semiconductor,
financial, medical, and transportation. The community provides leadership and
direction for more than 350 members and promotes professional growth through
meetings, workshops, seminars, conferences, mentoring, and networking.</p> 

<h1>Administrative Council</h1>
<p>An administrative council manages the LSC. The seven elected officers are the
sole voting members with committee managers reporting to the council.</p> 
<p>The Council abides by the LSC and Society Bylaws. The Lone Star community 
exists under its charter from the Society and operates within the Society's 
policies. The LSC Bylaws and amendments to them are compatible with the 
Society's Bylaws and its operating policies.</p> 
<p>The LSC Administrative Council typically meets at 6:15 p.m. on the first 
Thursday of each month at the LaMadeleine in Addison, 5290 Belt Line Road, Suite
112, Addison, TX 75240, 972-239-9051.</p>
<p>Council meetings are open to all LSC members. Contact the officers or 
committee managers when you have questions or comments.</p>

<h1>Survey Results</h1>
<p>Lone Star community's survey results from 2005</p>
