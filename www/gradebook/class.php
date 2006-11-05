<?php
function classy_create($course_key, $section, $term_key)
{
   $query = "SELECT * FROM class WHERE course_key='{$course_key}' AND section='{$section}' AND term_key='{$term_key}'";
   $result = mysql_query($query);
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   if (!$results)
   {
      // Query string should contain properly formatted SQL
      $query = "INSERT INTO class VALUES(null,'{$course_key}','{$section}','{$term_key}',null,null,'1')";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function classy_edit($class_key, $course_key, $section, $term_key)
{
   if (classy_exists($class_key))
   {
      // Query string should contain properly formatted SQL, will want to update     
      // only changed information
      $updates = "course_key='{$course_key}',section='{$section}',term_key='{$term_key}'";
      $query = "UPDATE class SET {$updates} WHERE class_key='{$class_key}'";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

// We never want to delete things in most of these classes.
// If there were a large number of entries in the database it could wreak havoc to delete a class. So instead we will set a flag to hide a class. If a teacher really wants a class to disappear we might need a separate front end that is dedicated to hazardous operations
function classy_delete($class_key)
{
   if (classy_exists($class_key))
   {
      // Query string should contain properly formatted SQL
      $query = "UPDATE class SET is_active='0' WHERE class_key={$class_key}";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function classy_get($class_key)
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM class WHERE class_key={$class_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function classy_get_all()
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM class";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function classy_exists($class_key)
{
   $result = classy_get($class_key);
   if ($result)
      return true;
   else
      return false;
}
?>