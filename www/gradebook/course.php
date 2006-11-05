<?php
function course_create($dept_key, $course_no, $title)
{
   $query = "SELECT * FROM course WHERE dept_key='{$dept_key}' AND course_no='{$course_no}'";
   $result = mysql_query($query);
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   if (!$results)   
   {
      // Query string should contain properly formatted SQL
      $query = "INSERT INTO course VALUES('{$course_key}','{$dept_key}','{$course_no}','{$title}')";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function course_edit($course_key, $dept_key, $course_no, $title)
{
   if (course_exists($course_key))
   {
      // Query string should contain properly formatted SQL, will want to update     
      // only changed information
      $query = "UPDATE course SET dept_key='{$dept_key}', course_no='{$course_no}', title='{$title}' WHERE course_key={$course_key}";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function course_delete($course_key)
{
   if (course_exists($course_key))
   {
      // Query string should contain properly formatted SQL
      $query = "DELETE FROM course WHERE course_key={$course_key}";
      $result = mysql_query($query);
      if (result != true)
         return false;
      return true;
   }
   else
      return false;
}

function course_get($course_key)
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM course WHERE course_key={$course_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function course_get_all()
{
   $query = "SELECT * FROM course";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function course_exists($course_key)
{
   $result = course_get($course_key);
   if ($result)
      return true;
   else
      return false;
}
?>