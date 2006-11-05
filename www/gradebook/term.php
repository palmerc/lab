<?php
function term_create($term_key, $semester, $year)
{
   if (!term_exists($term_key))
   {
      // Query string should contain properly formatted SQL
      $query = "INSERT INTO term VALUES('{$term_key}','{$semester}','{$year}')";
      $result = mysql_query($query);
      print_r($result);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function term_edit($term_key, $semester, $year)
{
   if (term_exists($term_key))
   {
      // Query string should contain properly formatted SQL, will want to update     
      // only changed information
      $query = "UPDATE term SET semester={$semester}, year={$year} WHERE term_key={$term_key}";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function term_delete($term_key)
{
   if (term_exists(term_key))
   {
      // Query string should contain properly formatted SQL
      $query = "DELETE FROM term WHERE term_key={$term_key}";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function term_get($term_key)
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM term WHERE term_key={$term_key}";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function term_get_all()
{
   $query = 'SELECT * FROM term';
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

function term_exists($term_key)
{
   $result = term_get($term_key);
   if ($result)
      return true;
   else
      return false;
}
?>