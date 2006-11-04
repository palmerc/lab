<?php
// This section of code is meant to handle the creation, deletion, modification,
// and retrieval of user records in the database.
// It assumes that the table with users is called users
// It assumes that the user record has 4 parameters email(key), first, last, password

// Insert a new user into the table
// Requires an email address
// Returns true on success, false otherwise
function student_create($first_name, $last_name, $empl_id, $email, $phone, $euid, $web_addr, $comments, $is_active)
{
   // Query string should contain properly formatted SQL
   // student_key INT PRIMARY KEY, empl_id INT, first_name VARCHAR(), last_name VARCHAR(), email VARCHAR(), phone VARCHAR(), euid VARCHAR(), photo MEDIUMBLOB, comments TEXT, is_active TINYINT
   $query = "INSERT INTO student VALUES(null,'{$empl_id}','{$first_name}','{$last_name}','{$email}','{$phone}','{$euid}',null,'{$web_addr}','{$comments}','{$is_active}')";
   //echo $query;
   $result = mysql_query($query);
   if (!$result)
      return false;
   return true;
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

// Modify a student in the table
// Requires all fields except password to be filled out
// Returns true on success, false otherwise
function student_edit($student_key, $first_name, $last_name,  $empl_id, $email, $phone, $euid, $web_addr, $comments, $is_active)
{
   $updates = "empl_id='{$empl_id}',first_name='{$first_name}',email='{$email}',phone='{$phone}',euid='{$euid}',web_addr='{$web_addr}',comments='{$comments}',is_active='{$is_active}'";
   $query = "UPDATE student SET {$updates} WHERE student_key='{$student_key}'";
   $result = mysql_query($query);
   if (mysql_affected_rows() == 1)
      return true; 
   else 
      return false; // Update failed
}

// Get the details about a student
// Requires an empl_id
// Returns array
function student_get($student_key)
{
   // Query string should contain properly formatted SQL
   $query = "SELECT * FROM student WHERE student_key='{$student_key}'";
   $result = mysql_query($query);
   
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}
// Retrieve all students
// Take no parameters
// Returns an array of all users
function student_get_all()
{
   $query = "SELECT * FROM student";
   $result = mysql_query($query);
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}

// Check if the student is in the database
// Requires an empl_id address
// Returns boolean true if the user already exists
function student_exists($student_key)
{
   $result = student_get($student_key);
   if ($result)
      return true;
   else
      return false;
}

function student_upload_image($student_key, $file) {
   $tmp_name = $file['tmp_name'];
   $file_info = getimagesize($tmp_name);
   $mime_type = $file_info['mime'];
   if ($mime_type == 'image/jpeg' or $mime_type == 'image/gif' or $mime_type == 'image/png') {
      $fp = fopen($tmp_name, 'r');
      $photo = fread($fp, filesize($tmp_name));
      $photo = addslashes($photo);
      fclose($fp);
      $updates = "photo='$photo}'";
      $query = "UPDATE student SET {$updates} WHERE student_key='{$student_key}'";
      $result = mysql_query($query);
      //echo $query;
      if (!$result)
         return false; 
      return true;
   } 
   else
   {
      return false;
   }
}

?>