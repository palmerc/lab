<?
require('database.php');
require('category.php');

if (isset($_REQUEST['class_key']))
   $class_key=$_REQUEST['class_key'];
else
   header('location:categoryMain.php');
   
database_connect();
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   // Validate submission
   //print_r($_REQUEST);
   $title = $_REQUEST['title'];
   $rank = $_REQUEST['percentage'];
   if (category_create($class_key, $title, $percentage))
      // If all goes well take them back to the studentMain page
      header('location:categoryMain.php');
}
   
database_disconnect();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Create a category</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
      <h2>Create a category</h2>
   </div>
   <div id="page">
      <form action="<?php echo $_SERVER['PHP_SELF']?>" method="post">
      <input type="hidden" name="class_key" value="<? echo $class_key ?>" />
      <h3>Category basics</h3>
      <div class="indent">
      <table>
      <tr>
         <td>
         Title:
         </td>
         <td>
         <input type="text" id="title" name="title" size="10" value="" />
         </td>
      </tr>
      <tr>
         <td>
         Percentage:
         </td>
         <td>
         <input type="text" id="percentage" name="percentage" size="3" value="" />
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