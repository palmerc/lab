<?php
   include('includes/header.html');
?>
<body>
	<div id="container">
		<div id="header">
         <div id="topbanner">
            <img src="i/purpletopbar.gif" alt="The Lone Star Community" />
         </div>
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
            <p>Welcome, <a href="edit.php"><?php echo $row[0]; ?></a> <a href="logout.php">[Logout]</a></p>
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
   include('includes/footer.html');
?>
