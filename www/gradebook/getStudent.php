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
   $web_addr = $student[0]['web_addr'];
   $phone = $student[0]['phone'];
   $comments = $student[0]['comments'];
   header("Cache-Control", "no-cache");
?>
<table>
   <tr>
      <td>Name:</td>
      <td><? echo $first_name . ", " . $last_name ?></td>
   </tr>
   <tr>
      <td>EmplID:</td>
      <td><? echo $empl_id ?></td>
   </tr>
   <tr>
      <td>EUID:</td>
      <td><? echo $euid ?></td>
   </tr>
   <tr>
      <td>Email:</td>
      <td><? echo $email ?></td>
   </tr>
   <tr>
      <td>Website:</td>
      <td><? echo $web_addr ?></td>
   </tr>
   <tr>
      <td>Phone:</td>
      <td><? echo $phone ?></td>
   </td>
   <tr>
      <td>Comments:</td>
      <td><? echo $comments ?></td>
   </tr>
   <tr>
      <td colspan="2"><img src="getStudentImage.php?student_key=<? echo $_REQUEST['student_key']; ?>" /></td>
   </tr>
</table>
<?php
   database_disconnect();
?>