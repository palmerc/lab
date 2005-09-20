<?php
    $write_file = "alumni_data.csv";
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
  <title>Real Estate Alumni Data Submission Form</title>
  <link rel="stylesheet" type="text/css" href="_scholarship.css" />
  <meta http-equiv="Content-Type" content="text/xhtml; charset=utf-8" />
</head>

<body>
<div id="title">
  <h1>Real Estate Alumni Data Submission Form</h1>
</div>

<div id="main">

<?php // MAIN
    if ($_SERVER['REQUEST_METHOD']=="POST"){
        
        // Save all the POSTed values to an array.
        $form_array = array($_REQUEST['firstname'],
            $_REQUEST['lastname'],
            $_REQUEST['company'],
            $_REQUEST['address1'],
            $_REQUEST['address2'],
            $_REQUEST['city'],
            $_REQUEST['state'],
            $_REQUEST['zipcode'],
            $_REQUEST['dayphone'],
            $_REQUEST['fax'],
            $_REQUEST['email'],
                
            $_REQUEST['degree_type'],
            $_REQUEST['major'],
            $_REQUEST['gradyear'],
            $_REQUEST['changetype'],
            $_SERVER['REMOTE_ADDR'],
            date(Ymd));

        $form_array = array_trim($form_array);
        file_writer($write_file, $form_array);
        echo "<p>Thank you for completing the registration. {$problem}</p>
            <p><a href=\"index.php\">Perform another registration</a></p>";
    } else {
        form_function();
    } 
?>

</div>
</body>
</html>

<?php // FUNCTIONS
    function array_trim($form_array){
        foreach ($form_array as $current_value){
            $current_value = preg_replace('/\s{2}/',' ',$current_value);
            $temp_array[] = trim($current_value);
            
        }
        return $temp_array;
    }

    function file_writer($write_file, $form_array){
        // Generate the line to be fputs to the file.
        $line = implode(",", $form_array) . "\n";

        // OPEN the file and create a handle
        // If the file does not exist create it and add the header line to the file.
        if (!file_exists($write_file)) {
            $handle = fopen($write_file, "w");
            if ($handle === FALSE) echo "<p>An error has occured while trying to create the file {$write_file}</p>";
            fputs($handle, $header);
        // Otherwise just append the data to the file.
        } else {
            $handle = fopen($write_file, "a");
            if ($handle === FALSE) echo "<p>An error has occured while trying to append the file {$write_file}</p>";
        }
    
        // Write the data to the file and close the file handle.
        if (fputs($handle, $line) === FALSE){
            echo "<p>An error has occured while trying to write to the file {$write_file}</p>";
        }
        fclose($handle);
    } // END file_writer()
       
    function form_function() { 
?>
        <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post" 
        onsubmit="return VerifyData()" enctype="multipart/form-data" name="Alumni">
      
    <h3>General Information</h3>
    <div id="general">
        <table>
        <tr>
            <td><label for="firstname">First Name</label></td>
            <td><input type="text" id="firstname" name="firstname" value="" size="30" /></td>
            <td><label for="lastname">Last Name</label></td>
            <td><input type="text" id="lastname" name="lastname" value="" size="30" /></td>
        </tr>
        <tr>
            <td><label for="company">Company</label></td>
            <td><input type="text" id="company" name="company" value="" size="30" /></td>
        </tr>
        <tr>
            <td><label for="address1">Address 1</label></td>
            <td><input type="text" id="address1" name="address1" value="" size="30" /></td>
            <td><label for="address2">Address 2</label></td>
            <td><input type="text" id="address2" name="address2" value="" size="30" /></td>
        </tr>
        <tr>
            <td><label for="city">City</label></td>
            <td><input type="text" id="city" name="city" value="" size="30" /></td>
            <td><label for="state">State</label></td>
            <td><input type="text" id="state" name="state" value="" size="15" /></td>
            <td><label for="zipcode">Zip Code</label></td>
            <td><input type="text" id="zipcode" name="zipcode" value="" size="10" /></td>
        </tr>
        <tr>
            <td colspan="2">Please enter the number without dashes or spaces. (e.g. 9405551212)</td>
        </tr>
        <tr>
            <td><label for="dayphone">Daytime Phone</label></td>
            <td><input type="text" id="dayphone" name="dayphone" value="" size="14" /></td>
            <td><label for="fax">Fax</label></td>
            <td><input type="text" id="fax" name="fax" value="" size="14" /></td>
        </tr>
        <tr>
            <td><label for="email">Email Address</label></td>
            <td><input type="text" id="email" name="email" value="" size="30" /></td>
        </tr>
        </table>
    </div>
  
  <h3>Scholastic Information</h3>
  <strong>Degree Type</strong>
  <table>
    <tr>
        <td><label for="bachelors">Bachelors</label></td>
        <td><input type="radio" id="bachelors" name="degree_type" value="bachelors" size="30" /></td>
    </tr>
    <tr>
        <td><label for="masters">Masters</label></td>
        <td><input type="radio" id="masters" name="degree_type" value="masters" size="30" /></td>
    </tr>
    <tr>
        <td><label for="phd">Ph.D</label></td>
        <td><input type="radio" id="phd" name="degree_type" value="phd" size="30" /></td>
    </tr>
    </table>
        <label for="major">UNT Major</label>
        <input type="text" id="major" name="major" value="" size="30" />
    <table>
    <tr>
        <td><label for="gradyear">Graduation Year</label></td>
        <td><input type="text" id="gradyear" name="gradyear" value="" size="10" /></td>
    </tr>
  </table>
    Enrollment Type (New, Update, or Removal):
    <select name="changetype">
		<option value="new">New Enrollment</option>
		<option value="update">Update</option>
		<option value="remove">Removal</option>
	</select>
    <input type="submit" value="Submit" />
  </form>
<?php }  // END form_function() ?>