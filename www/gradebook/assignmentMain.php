<?php
require('database.php');
require('assignment.php');

database_connect();
// Hard coded 1 should be replaced by something that makes sense
$class_key = isset($_REQUEST['class_key']) ? $_REQUEST['class_key'] : 1;

// This should get you the official name of the class
$query = "SELECT course.dept_key, course.course_no, class.section
         FROM class, course
         WHERE course.course_key=class.course_key
            AND class.class_key={$class_key}";
$result = mysql_query($query);
while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   $results[] = $row;
$class_title = $results[0];

// This is meant to grab all the assignment categories from a given class
$query = "SELECT category_key, title
         FROM category
         WHERE class_key={$class_key}";
$result = mysql_query($query);
while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   $categories[] = $row;


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>The Gradebook - Assignment management</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf8" />
   <link rel="stylesheet" type="text/css" href="c/main.css" />
   <link rel="icon" href="i/favicon.ico" type="image/vnd.microsoft.icon" />
</head>

<body>
   <div id="container">
   <div id="header">
   <h2>Assignment management</h2>
   </div>
   <div id="page">
   <h3>Assignment managment options</h3>
   <ul>
      <li><a href="assignmentCreate.php?class_key=<? echo $class_key ?>">Add an Assignment</a></li>
      <li><a href="categoryMain.php?class_key=<? echo $class_key ?>">Add an Category</a></li>
      <li><a href="?">Undelete an Assignment</a></li>
      <li><a href="gradebook.php?class_key=<? echo $class_key ?>">Go to gradebook</a></li>
   </ul>
   <h3>Edit <? printf("%s %d.%03d", $class_title['dept_key'], $class_title['course_no'], $class_title['section']); ?> assignments</h3>
   <form action="<?php echo $_SERVER['PHP_SELF'] ?>" method="post">
   <input type="hidden" name="class_key" value="<? echo $class_key ?>" />
<?php
   foreach ($categories as $category)
   {
      $category_key = $category['category_key'];
      $category_title = $category['title'];
      // This is supposed to fetch the assignments under a given category type
      $query = "SELECT assignment_key, title, category_key, max_points, due_date, rank
               FROM assignment WHERE class_key={$class_key} AND category_key={$category_key}";
      $result = mysql_query($query);
      $results = null;
      while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
         $results[] = $row;
      //print_r($results);
      if ($results)
      {
?>
      <h4><? echo $category_title ?></h4>
      <table id="students">
      <tr>
      <th>Assignment</th><th>Max Points</th><th>Due Date</th><th>Delete</th>
      </tr>
<?php
         foreach ($results as $row)
         {
            $assignment_key = $row['assignment_key'];
            $title = $row['title'];
            $max_points = $row['max_points'];
            $due_date = $row['due_date'];
            $rank = $row['rank'];
?>
      <tr>
      <td>
         <a href="assignmentEdit.php?assignment_key=<? echo $assignment_key ?>">
            <? echo $title ?></a>
      </td>
      <td><? echo $max_points ?></td>
      <td><? echo $due_date ?></td>
      <td><input type="checkbox" name="studentDelete"/></td>
      </tr>
<?php
         }
         echo"
      </table>
            ";
      }
      else
      {
      echo "<p>No assignments in this category</p>";
      }
   }
   database_disconnect();
?>
   <input type="submit" value="Submit" />
   </form>
   <div style="position:absolute;" id="popup">
      <table id="sData" class="studentDetail" border="0" cellspacing="2" cellpadding="2">
         <tbody id="sDataBody"></tbody>
      </table>
   </div>
   </div>
   </div>
</body>
</html>