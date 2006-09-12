<?php
	$title = "CSE 4410 - Assignment 2";
	$section = "Familiarity Assignment 2";
	require("../../dev-template.php");

   if ($_SERVER['REQUEST_METHOD'] == "POST"){  
      $name = $_POST['name'];
      $email = $_POST['email'];
      $sex = $_POST['sex'];
      $age = $_POST['age'];
   }
?>
<div class="form">
   <form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
      Enter name (Last, First, Middle): <input type="text" name="name" /><br />
      Enter email address: <input type="text" name="email" /><br />
      Enter your sex: Male <input type="radio" name="sex" value="male" /> Female <input type="radio" name="sex" value="female" /> Age: <input type="text" name="age" /><br />
      <input type="button" name="reset" value="Reset" /> <input type="submit" name="submit" value="Press here to submit your information" size="3" /><br />
   </form>
   <?php
         echo"
         <h3>Submitted values</h3>
         Age: {$age}<br />
         Sex: {$sex}<br />
         Email: {$email}<br />
         Name: {$name}<br />
         ";
   ?>
</div>