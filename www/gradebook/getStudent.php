<?php
   require_once('database.php');
   require_once('student.php');
   
   database_connect();
   $student = student_get($_REQUEST['student_key']);
   $first_name = $student[0]['first_name'];
   $last_name = $student[0]['last_name'];
   $empl_id = $student[0]['empl_id'];
   $euid = $student[0]['euid'];
   $email = $student[0]['email'];
   $web_addr = $student[0]['web_addr'] != '' ? $student[0]['web_addr'] : ' ';
   $phone = $student[0]['phone'] != '' ? $student[0]['phone'] : ' ';
   $comments = $student[0]['comments'] != '' ? $student[0]['comments'] : ' ';
   header("Content-type: text/xml");
   header("Cache-Control", "no-cache");
?>
<response>
   <firstName><? echo $first_name ?></firstName>
   <lastName><? echo $last_name ?></lastName>
   <emplId><? echo $empl_id ?></emplId>
   <euid><? echo $euid ?></euid>
   <email><? echo $email ?></email>
   <web><? echo $web_addr ?></web>
   <phone><? echo $phone ?></phone>
   <comments><? echo $comments ?></comments>
   <photo>getStudentImage.php?student_key=<? echo $_REQUEST['student_key'] ?></photo>
</response>
<?php
   database_disconnect();
?>