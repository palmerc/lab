<?
require('database.php');
require('dept.php');
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   database_connect();
   // Validate submission
   //print_r($_REQUEST);
   $dept_key = $_REQUEST['dept_key'];
   $dept_title = $_REQUEST['dept_title'] != null ? $_REQUEST['dept_title'] : '';
   if (dept_create($dept_key, $dept_title))
      // If all goes well take them back to the studentMain page
      header('location:studentMain.php');
   database_disconnect();
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Create a department</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
      <h2>Create a department</h2>
   </div>
   <div id="page">
      <form action="<?php echo $_SERVER['PHP_SELF']?>" method="post">
      <h3>Department basics</h3>
      <div class="indent">
      <table>
      <tr>
         <td>
         Department:
         </td>
         <td>
         <input type="text" id="dept_key" name="dept_key" value="" />
         </td>
      </tr>
      <tr>
         <td>
         Title:
         </td>
         <td>
         <input type="text" id="dept_title" name="dept_title" size="50" value="" />
         </td>
      </tr>
      </table>
      <input type="submit" id="submit" value="Submit" />
      </div>
      </form>
   </div>
   </div>
</body>
</html>