<?php

function dept_create($dept_key, $dept_title)
{
   if (!dept_exists($dept_key))
   {
      // Query string should contain properly formatted SQL
      $query = "INSERT INTO dept VALUES('{$dept_key}','{$dept_title}')";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function dept_edit($dept_key, $title)
{
   if (dept_exists($dept_key))
   {
      // Query string should contain properly formatted SQL, will want to update     
      // only changed information
      $query = 'UPDATE dept SET title={$title} WHERE dept_key={$dept_key}';
      $result = mysql_query($query);
      if (result != true)
         return false;
      return true;
   }
   else
      return false;  
}

function dept_delete($dept_key)
{
   if (dept_exists($dept_key))
   {
      // Query string should contain properly formatted SQL
      $query = 'DELETE FROM dept WHERE dept_key={$dept_key}';
      $result = mysql_query($query);
      if ($result != true)
         return false;
      return true;
   }
   else
      return false;
}

function dept_get($dept_key)
{
   // Query string should contain properly formatted SQL
   $query = 'SELECT * FROM dept WHERE dept_key={$dept_key}';
   $result = mysql_query($query);
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function dept_get_all()
{
   $query = 'SELECT * FROM dept';
   $results = mysql_query($query);
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function dept_exists($dept_key)
{
   $result = dept_get($dept_key);
   if ($result)
      return true;
   else
      return false;
}

?>