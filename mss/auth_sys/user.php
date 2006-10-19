<?php
// This section of code is meant to handle the creation, deletion, modification,
// and retrieval of user records in the database.
// It assumes that the table with users is called users
// It assumes that the user record has 4 parameters email(key), first, last, password

// Authenticate user
// Requires email address and password
// Returns true on success, false otherwise
function auth_user($email, $password)
{  
   $query = "SELECT email, first_name, last_name FROM users WHERE email='{$email}' AND password=SHA('{$password}')";
   $result = mysql_query($query);
   $row = @mysql_fetch_array ($result);
	if ($row) { // If a record was returned...
		return $row;
	}
   return false;
}

// Insert a new user into the table
// Requires an email address
// Returns true on success, false otherwise
function create_user($email, $first_name, $last_name, $password, $admin)
{
   if ($email != NULL)
   {
      $query = "INSERT INTO users VALUES('{$email}','{$first_name}','{$last_name}',SHA('{$password}'),'{$admin}')";
      $result = mysql_query($query);
      if (mysql_affected_rows() == 1) {
         return true;
      } 
      else
      {
         echo '<p>Create user failed!</p>';
         return false; // Query failed
      }
   }
   else
   {
      return false; // Cannot allow users to be created without email addresses
   }
}

// Delete a user from the table
// Requires the users email address
// Returns true on success, false otherwise
function delete_user($email)
{
   $query = "DELETE FROM users WHERE email='{$email}'";
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
// Requires an email address
// Returns true on success, false otherwise
function modify_user($user_id, $email, $first_name, $last_name, $admin, $password)
{
   if ($user_id) {     
      if ($email != $user_id)
      {
         $query = "SELECT first_name, last_name, password, admin FROM users WHERE email='{$user_id}'";
         $result = mysql_query($query);
         
         if (!first_name)
            $first_name = $result['first_name'];
         if (!last_name)
            $last_name = $result['last_name'];
         if (!$password)
            $password = $result['password'];
         if (!$admin)
            $admin = $result['admin'];
         if (create_user($email, $first_name, $last_name, $password, $admin))
            delete_user($user_id);
         else
            return false;         
         return true;
      }
      
      $parameters = 0;
      if ($first_name) $parameters++;
      if ($last_name) $parameters++;
      if ($admin) $parameters++;
      if ($password) $parameters++;
      
      if ($first_name)
      {
         $updates.="first_name='{$first_name}'";
         if ($parameters > 1) $updates.=',';
         $parameters--;
      }
      if ($last_name)
      {
         $updates.="last_name='{$last_name}'";
         if ($parameters > 1) $updates.=',';
         $parameters--;
      }
      if ($password)
      {
         $updates.="password=SHA('{$password}')";
         if ($parameters > 1) $updates.=',';
         $parameters--;
      }
      if ($admin)
      {
         $updates.="admin='{$admin}'";
         if ($parameters > 1) $updates.=',';
         $parameters--;
      }
      $query = "UPDATE users SET {$updates} WHERE email='{$user_id}' LIMIT 1";
      $result = mysql_query($query);
      if (mysql_affected_rows() == 1) {
         return true;
      } 
      else
      {
         return false; // Query failed
      }
   }
   else
   {
      return false;
   }
}

// Get the details about a user
// Requires an email address
// Returns array of users first and last name
function retrieve_user($email)
{
   if ($email) {
      $query = "SELECT first_name, last_name, admin FROM users WHERE email='{$email}'";
      $result = mysql_query($query);
      while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
      {
         $results[] = $row;
      }
      return $results;
   }
   return false;
}

// Retrieve all users
// Take no parameters
// Returns an array of all users
function retrieve_all_users()
{
   $query = "SELECT email, first_name, last_name, admin FROM users";
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
function user_exists($email)
{
   $query = "SELECT $email FROM users";
   $result = mysql_query($query);
   return mysql_num_rows($result);
}

?>