<?
require('database.php');
require('assignment.php');

if (isset($_REQUEST['class_key']))
   $class_key=$_REQUEST['class_key'];
else
   header('location:index.php');
   
database_connect();
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   // Validate submission
   //print_r($_REQUEST);
   $title = $_REQUEST['title'];
   $category_key = $_REQUEST['category_key'];
   $max_points = $_REQUEST['max_points'];
   $due_date = $_REQUEST['due_date'];
   $rank = $_REQUEST['rank'];
   if (assignment_create($class_key, $title, $category_key, $max_points, $due_date, $rank))
      // If all goes well take them back to the studentMain page
      header("location:assignmentMain.php?class_key={$class_key}");
}

// This is meant to grab all the assignment categories from a given class
$query = "SELECT categoryKey, categoryTitle
         FROM category
         WHERE classKey={$class_key}";
$result = mysql_query($query);
while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   $categories[] = $row;

database_disconnect();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Create an assignment</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
      <h2>Create an assignment</h2>
   </div>
   <div id="page">
      <form action="<?php echo $_SERVER['PHP_SELF']?>" method="post">
      <input type="hidden" name="class_key" value="<? echo $class_key ?>" />
      <h3>Assignment basics</h3>
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
         Category:
         </td>
         <td>
         <select name="category_key">
<?php
   foreach ($categories as $category)
   {
      $category_key = $category['categoryKey'];
      $category_title = $category['categoryTitle'];
      echo "
            <option value=\"{$category_key}\">{$category_title}</option>";
   }
?>

         </select>
         </td>
      </tr>
      <tr>
         <td>
         Max Points:
         </td>
         <td>
         <input type="text" id="max_points" name="max_points" size="4" value="" />
         </td>
      </tr>
      <tr>
         <td>
         Due Date:
         </td>
         <td>
         <input type="text" id="due_date" name="due_date" size="10" value="" />
         </td>
      </tr>
      <tr>
         <td>
         Rank:
         </td>
         <td>
         <input type="text" id="rank" name="rank" size="2" value="" />
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