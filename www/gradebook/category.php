<?php
function category_create($class_key, $title, $percentage)
{
   // Query string should contain properly formatted SQL
   $query = "INSERT INTO category 
            VALUES(null,'{$class_key}','{$title}','{$percentage}')";
   $result = mysql_query($query);
   if (!$result)
      return false;
   return true;
}

function category_edit($category_key, $title, $percentage)
{
   if (category_exists($category_key))
   {
      // Query string should contain properly formatted SQL, will want to update     
      // only changed information
      $updates = "title='{$title}',percentage='{$percentage}'";
      $query = "UPDATE category SET {$updates} WHERE category_key='{$category_key}'";
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
function category_delete($category_key)
{
   if (category_exists($category_key))
   {
      // Query string should contain properly formatted SQL
      $query = "UPDATE category SET is_active='0' WHERE category_key={$category_key}";
      $result = mysql_query($query);
      if (!$result)
         return false;
      return true;
   }
   else
      return false;
}

function category_get($category_key)
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM category WHERE category_key='{$category_key}'";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
      $results[] = $row;
      
   return $results;
}

function category_get_all()
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM category";
   $result = mysql_query($query);
   while (@$row = mysql_fetch_array($result, MYSQL_ASSOC))
      $results[] = $row;

   return $results;
}

function category_exists($category_key)
{
   $result = category_get($category_key);
   if ($result)
      return true;
   else
      return false;
}
?>