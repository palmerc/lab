<?php
	$title = "CSE 4410 - Assignment 3";
	$section = "Familiarity Assignment 3";
	require("../../dev-template.php");


   if ($_SERVER['REQUEST_METHOD'] == "POST"){
      if ($_POST['administrative'])
      {
         $content = file_get_contents("submissions");
         echo"
         <div class=\"form\">
         <h2>Administrative Dump</h2>
         <p><a href=\"{$ptr}labs/assignment3/\">Back</a></p>
         <pre>$content</pre>
         <p><a href=\"{$ptr}labs/assignment3/\">Back</a></p>
         </div>
            ";
      }
      else
      {
         $name = $_POST['name'];
         $email = $_POST['email'];
         $sex = $_POST['sex'];
         $age = $_POST['age'];
         $category = $_POST['category'];
         $homepage = $_POST['homepage'];
         $name_blank = 0;
         $name_format = 0;
         
         $name_regex = '/^(\w+)\s*,\s*(\w+)\s*(\w+)$/';
         
         if (!$name)
            $name_blank = 1;
         else if (!preg_match($name_regex, $name, $name_array))
            $name_format = 1;
               
         $email_regex = '/\w+@\w+/';
         if (!$email)
            $email_blank = 1;
         else if (!preg_match($email_regex, $email))
            $email_format = 1;
         
         if (!$name_blank && !$name_format && !$email_blank && !$email_format)
         {
            $handle = fopen("submissions", "a");
            fputs($handle, "
            First Name: {$name_array[2]}
            Middle Name: {$name_array[3]}
            Last Name: {$name_array[1]}
            Email: {$email}
            Sex: {$sex}
            Age: {$age}
            Category: {$category}
            Homepage: {$homepage}
            ******************************\n");
            fclose($handle);
            echo "
            <div class=\"form\">
            <h2>{$name_array[2]}, thank you for entering your information.</h2>
            <p><a href=\"{$ptr}labs/assignment3/\">Try it again?</a></p>
            </div>
               ";
         }
      }
   }   
   else
   {
?>
   <div class="form">
      <form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
      <table>
      <tr>
         <td>Enter name (Last, First Middle):</td>
         <td><input type="text" name="name" /></td>
         <td>
            <?php 
               if ($name_blank) 
                  echo "Error: Failed to enter name.";
               else if ($name_format)
                  echo "Error: Name not properly formatted.";
            ?>
         </td>
      </tr>
      <tr>
         <td>Enter email address:</td>
         <td><input type="text" name="email" /></td>
         <td>
            <?php 
               if ($email_blank) 
                  echo "Error: Failed to enter email address.";
               else if ($email_format)
                  echo "Error: Email not properly formatted.";
            ?>
         </td>
      </tr>
      <tr>
         <td>Enter your sex:</td><td>Male <input type="radio" name="sex" value="male" checked /> Female <input type="radio" name="sex" value="female" /></td>
      </tr>
      <tr>
         <td>Age:</td>
         <td>
         <select name="age">
            <option>18-20</option>
            <option>21-25</option>
            <option>26-30</option>
            <option>31-38</option>
            <option>39-45</option>
            <option>46-over</option>
         </select>
         </td>
      </tr>
      <tr>
         <td>Category:</td>
         <td>
         <select name="category">
            <option>Personal</option>
            <option>Business</option>
            <option>Government</option>
            <option>Educational</option>
            <option>Watchdog</option>
         </select>
         </td>
      </tr>
      <tr>
         <td>User's Homepage URL:</td><td><input type="text" name="homepage" /></td>
      </tr>
      </table>
         <input type="submit" name="administrative" value="Administrative" />
         <input type="submit" name="submit" value="Submit" size="3" /><br />
      </form>
   </div>
<?php
   }
?>