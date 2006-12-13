<?php
function grade_create($assignment_key, $student_key, $grade)
{
   $query = "SELECT * FROM grades WHERE studentKey='{$student_key}' AND assignmentKey='{$assignment_key}'";
   $result = mysql_query($query);
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   if (!$results)   
   {
      // Query string should contain properly formatted SQL
      $query = "INSERT INTO grades VALUES(null, '{$assignment_key}','{$student_key}','{$grade}')";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function grade_edit($grade_key, $grade)
{
   if (grade_exists($grade_key))
   {
      // Query string should contain properly formatted SQL, will want to update     
      // only changed information
      $query = "UPDATE grades SET grade='{$grade}' WHERE gradeKey={$grade_key}";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function grade_delete($grade_key)
{
   if (grade_exists($grade_key))
   {
      // Query string should contain properly formatted SQL
      $query = "DELETE FROM grades WHERE gradeKey={$grade_key}";
      $result = mysql_query($query);
      if (result != true)
         return false;
      return true;
   }
   else
      return false;
}

function grade_key_get($assignment_key, $student_key)
{
   // Query string should contain properly formatted SQL
   $query = "SELECT gradeKey FROM grades 
   WHERE assignmentKey={$assignment_key} AND studentKey={$student_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   
   return $results[0]['gradeKey'];
}


function grade_get($grade_key)
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM grades WHERE gradeKey={$grade_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function grade_get_all()
{
   $query = "SELECT * FROM grades";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function grade_exists($grade_key)
{
   $result = grade_get($grade_key);
   if ($result)
      return true;
   else
      return false;
}
?>