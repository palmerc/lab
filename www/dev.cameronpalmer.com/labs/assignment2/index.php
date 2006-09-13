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
      Enter your sex: Male <input type="radio" name="sex" value="male" /> Female <input type="radio" name="sex" value="female" /><br />
      Age: <input type="text" name="age" /><br />
      <input type="button" name="reset" value="Reset" />
      <input type="submit" name="submit" value="Press here to submit your information" size="3" /><br />
   </form>
   <?php
      if (!$email)
         echo "Error: Failed to enter email address.<br />";
      if (!$name)
         echo "Error: Failed to enter name.<br />";
      if ($age >= 1 && $age <= 21)
         echo"Too young to respond to this offer.<br />";
      else if ($age >= 22 && $age <= 29)
         echo"You are ideal for the information. We will get into the mail our full-color brochure as soon as the next mail truck arrives at our offices.<br />";
      else if ($age >= 30 && $age <= 45)
         echo"We have a senior's package that will be more suited to your needs and sedintary lifestyle. We will send further information to you as soon as we can.<br />";
      else if ($age >= 46)
         echo"Due to various state and national laws and statutes, we are not allowed to send you information about our new, innovative products. Sorry.<br />";
      else
         echo "Error: Age out of range (1-100)<br />";
         
      echo"
         <h3>Submitted values:</h3>
         Age: {$age}<br />
         Sex: {$sex}<br />
         Email: {$email}<br />
         Name: {$name}<br />
         ";
   ?>
</div>