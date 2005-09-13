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
<?php
    
   if ($_SERVER['REQUEST_METHOD']=="POST"){
    if (!file_exists("alumni_data.csv")) {
        $handle = fopen("alumni_data.csv", "w");
        $header = "firstname,lastname,company,address1,address2,city,state,"
        ."zipcode,dayphone,fax,email,degree_type,major,gradyear,ipaddr,date,"
        ."changetype\n";
        fputs($handle, $header);
    } else {
        $handle = fopen("alumni_data.csv", "a");
    }
    $ipaddr = $_SERVER['REMOTE_ADDR'];
    $today = date(Ymd);
    $line = "{$_REQUEST['firstname']},"
        ."{$_REQUEST['lastname']},"
        ."{$_REQUEST['company']},"
        ."{$_REQUEST['address1']},"
        ."{$_REQUEST['address2']},"
        ."{$_REQUEST['city']},"
        ."{$_REQUEST['state']},"
        ."{$_REQUEST['zipcode']},"
        ."{$_REQUEST['dayphone']},"
        ."{$_REQUEST['fax']},"
        ."{$_REQUEST['email']},"
        ."{$_REQUEST['degree_type']},"
        ."{$_REQUEST['major']},"
        ."{$_REQUEST['gradyear']},"
        ."{$ipaddr},"
        ."{$today},"
        ."{$_REQUEST['changetype']}\n";
    fputs($handle, $line);
    fclose($handle);
    } else {
?>

<form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post" 
   enctype="multipart/form-data" name="form1">
   
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
<?php } ?>
</div>
</body>
</html>