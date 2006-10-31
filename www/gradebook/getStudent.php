<?php
   require_once('database.php');
   require_once('student.php');
   
   database_connect();
   $student = student_get($_REQUEST['student_key']);
   $first_name = $student[0]['first_name'];
   $last_name = $student[0]['last_name'];
   $empl_id = $student[0]['empl_id'];
   //$photo = $student[0]['photo'];
   header("Content-type: text/xml");
   header("Cache-Control", "no-cache");
   echo "<response>";
   echo "<firstName>". $first_name ."</firstName>";
   echo "<lastName>". $last_name ."</lastName>";
   echo "<emplId>". $empl_id ."</emplId>";
   //echo "<photo>". $photo ."</photo>";
   echo "</response>";
   database_disconnect();
?>