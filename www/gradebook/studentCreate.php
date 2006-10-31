<?php
   require('database.php');
   require('student.php');
   if ($_SERVER['REQUEST_METHOD'] == "POST")
   {
      database_connect();
      // Validate submission
      $empl_id = $_REQUEST['empl_id'];
      $first_name = $_REQUEST['first_name'];
      $last_name = $_REQUEST['last_name'];
      $email = $_REQUEST['email'];
      $phone = $_REQUEST['phone'];
      $web_addr = $_REQUEST['web_addr'];
      $euid = $_REQUEST['euid'];
      $comments = $_REQUEST['comments'];
      if (student_create($empl_id, $first_name, $last_name, $email, $phone, $euid, $web_addr, $comments, $is_active=1))
      // If all goes well take them back to the studentMain page
         header('location:studentMain.php');
      
      database_disconnect();
   }
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Create a student</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
   <h2>Create a student</h2>
   </div>
   <div id="page">
   <form action="<?php echo $_SERVER['PHP_SELF'] ?>" method="post">
   <table>
   <tr>
   <td>First Name:</td><td><input type="text" name="first_name" value="" /></td>
   </tr>
   <tr>
   <td>Last Name:</td><td><input type="text" name="last_name" value="" /></td>
   </tr>
   <tr>
   <td>EMPLID:</td><td><input type="text" name="empl_id" value="" /></td>
   </tr>
   <tr>
   <td>EUID:</td><td><input type="text" name="euid" value="" /></td>
   </tr>
   <tr>
   <td>Email:</td><td><input type="text" name="email" value="" /></td>
   </tr>
   <tr>
   <td>Web Address:</td><td><input type="text" name="web_addr" value="" /></td>
   </tr>
   <tr>
   <td>Phone:</td><td><input type="text" name="phone" value="" /></td>
   </tr>
   <tr>
   <td>Comments:</td><td><input type="textarea" name="comments" value="" /></td>
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