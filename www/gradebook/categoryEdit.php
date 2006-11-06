<?
require('database.php');
require('category.php');

if (isset($_REQUEST['category_key']))
   $category_key=$_REQUEST['category_key'];
else
   header('location:categoryMain.php');
   
database_connect();
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   // Validate submission
   //print_r($_REQUEST);
   $title = $_REQUEST['title'];
   $percentage = $_REQUEST['percentage'];
   if (category_edit($category_key, $title, $percentage))
      // If all goes well take them back to the studentMain page
      header('location:categoryMain.php');
}

$category = category_get($category_key);
$category = $category[0];
//print_r($category);
$title = $category['title'];
$percentage = $category['percentage'];

database_disconnect();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Edit a category</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
      <h2>Edit a category</h2>
   </div>
   <div id="page">
      <form action="<?php echo $_SERVER['PHP_SELF']?>" method="post">
      <input type="hidden" name="category_key" value="<? echo $category_key ?>" />
      <h3>Category basics</h3>
      <div class="indent">
      <table>
      <tr>
         <td>
         Title:
         </td>
         <td>
         <input type="text" id="title" name="title" size="10" value="<? echo $title ?>" />
         </td>
      </tr>
      <tr>
         <td>
         Percentage:
         </td>
         <td>
         <input type="text" id="percentage" name="percentage" size="3" value="<?echo $percentage ?>" />
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