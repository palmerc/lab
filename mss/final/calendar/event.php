<?php
function create_event($date, $title)
{
      $query = "INSERT INTO tbl_news VALUES(null, '{$date}','{$title}')";
      $result = mysql_query($query);
      if (mysql_affected_rows() == 1) {
         return true;
      } 
      else
      {
         echo '<p>Create event failed!</p>';
         return false; // Query failed
      }
}

// Delete a user from the table
// Requires the users email address
// Returns true on success, false otherwise
function delete_event($calid)
{
   $query = "DELETE FROM tbl_news WHERE calid='{$calid}'";
   $result = mysql_query($query);
   if (mysql_affected_rows() == 1) {
      return true;
   } 
   else
   {
      return false; // Query failed
   }
}

// Modify a user in the table
// Requires all fields except password to be filled out
// Returns true on success, false otherwise
function modify_event($calid, $date, $title)
{
      $updates.="date='{$date}',";
      $updates.="title='{$title}'";
      $query = "UPDATE tbl_news SET {$updates} WHERE calid='{$calid}' LIMIT 1";
      $result = mysql_query($query);
      if (mysql_affected_rows() == 1)
         return true; 
      else 
         return false; // Update failed
}

// Get the details about a user
// Requires an email address
// Returns array of users first and last name
function retrieve_event($calid)
{
   $query = "SELECT * FROM tbl_news WHERE calid='{$calid}'";
   $result = mysql_query($query);
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

// Retrieve all users
// Take no parameters
// Returns an array of all users
function retrieve_all_events()
{
   $query = "SELECT * FROM tbl_news";
   $result = mysql_query($query);
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

// Check if the user is in the database
// Requires an email address
// Returns boolean true if the user already exists
function event_exists($calid)
{
   return retrieve_event($calid);
}

?>