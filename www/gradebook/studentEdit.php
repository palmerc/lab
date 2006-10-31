<?php
   require('database.php');
   require('student.php');
   
   database_connect();
   if ($_SERVER['REQUEST_METHOD'] == "POST")
   {
      // Validate submission
      $student_key = $_REQUEST['student_key'];
      $empl_id = $_REQUEST['empl_id'];
      $first_name = $_REQUEST['first_name'];
      $last_name = $_REQUEST['last_name'];
      $email = $_REQUEST['email'];
      $phone = $_REQUEST['phone'];
      $web_addr = $_REQUEST['web_addr'];
      $euid = $_REQUEST['euid'];
      $comments = $_REQUEST['comments'];
      //echo '<pre>';
      //print_r($_FILES['photo']);
      //echo '</pre>';
      student_edit($student_key,$empl_id,$first_name,$last_name,$email,$phone,$euid,$web_addr,$comments,$is_active=1);
      if ($_FILES['photo']) student_upload_image($student_key,$_FILES['photo']);
      // If all goes well take them back to the studentMain page
      header('location:studentMain.php');
   }
   else
   {
      $student_key = $_REQUEST['student_key'];
      if ($student = student_get($student_key))
      {
         $student = $student[0];
         $student_key = $student['student_key'];
         $empl_id = $student['empl_id'];
         $first_name = $student['first_name'];
         $last_name = $student['last_name'];
         $email = $student['email'];
         $phone = $student['phone'];
         $web_addr = $student['web_addr'];
         $euid = $student['euid'];
         $comments = $student['comments'];
      }
   }
   database_disconnect();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Edit student - <? echo "{$last_name}, {$first_name}" ?></title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
   <h2>Edit student - <? echo "{$last_name}, {$first_name}" ?></h2>
   </div>
   <div id="page">
   <form enctype="multipart/form-data" action="<?php echo $_SERVER['PHP_SELF'] ?>" method="post">
   <input type="hidden" name="student_key" value="<? echo $student_key ?>" />
   <table>
   <tr>
   <td>First Name:</td><td><input type="text" name="first_name" value="<? echo $first_name ?>" /></td>
   </tr>
   <tr>
   <td>Last Name:</td><td><input type="text" name="last_name" value="<? echo $last_name ?>" /></td>
   </tr>
   <tr>
   <td>EMPLID:</td><td><input type="text" name="empl_id" value="<? echo $empl_id ?>" /></td>
   </tr>
   <tr>
   <td>EUID:</td><td><input type="text" name="euid" value="<? echo $euid ?>" /></td>
   </tr>
   <tr>
   <td>Email:</td><td><input type="text" name="email" value="<? echo $email ?>" /></td>
   </tr>
   <tr>
   <td>Web Address:</td><td><input type="text" name="web_addr" value="<? echo $web_addr ?>" /></td>
   </tr>
   <tr>
   <td>Phone:</td><td><input type="text" name="phone" value="<? echo $phone ?>" /></td>
   </tr>
   <tr>
   <td>Comments:</td><td><input type="textarea" name="comments" value="<? echo $comments ?>" /></td>
   </tr>
   <tr>
   <td>Picture:</td><td><input type="file" name="photo" value="" /></td>
   </tr>
   </table>
   <input type="submit" value="Submit" />
   </form>
   </div>
   </div>
</body>
</html>