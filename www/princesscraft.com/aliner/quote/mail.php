<?php
    $title="Princess Craft: MAIL";
    $sec_title = "Mailed Quote";
    $full_width = true;
    
    require("../../pc-template-clean.php");
    require("options.class.php");
    $email_addr = "sales@princesscraft.com";
    if (dirname(__FILE__)."/quote_email.php") require(dirname(__FILE__)."/quote_email.php");
    
    $options =& new optionlist("../../choosy/alineroptions.xml");
    $total = 0;
    $to=$email_addr;

    $subject="Quote Request - " . $_REQUEST['firstname'] . " " . 
        $_REQUEST['lastname'];

    // get the sender's name and email address
    // we'll just plug them a variable to be used later
    $from = stripslashes($_POST['email'])."<".stripslashes($_POST['email']).">";

   // generate a random string to be used as the boundary marker
   $mime_boundary="==Multipart_Boundary_x".md5(mt_rand())."x";

   // store the file information to variables for easier access
   $tmp_name = $_FILES['filename']['tmp_name'];
   $type = $_FILES['filename']['type'];
   $name = $_FILES['filename']['name'];
   $size = $_FILES['filename']['size'];
    $message = "";
            
    $message .= "First name: {$_REQUEST['First_name']}
    Last name: {$_REQUEST['Last_name']}
    Address1: {$_REQUEST['Address1']}
    Address2: {$_REQUEST['Address2']}
    City: {$_REQUEST['City']}
    State: {$_REQUEST['State']}
    Zip Code: {$_REQUEST['Zip_code']}
    Telephone: {$_REQUEST['Telephone']}
    Alternate Telephone: {$_REQUEST['Telephone2']}
    Requestors Email: {$_REQUEST['Requestors_email']}
    Comments: {$_REQUEST['Comments']}\n\n";
    
    foreach($options->get_list() as $o) {
        if($o->is_selected() !== false) {
            $message .= sprintf("%s ----> \$%0.02f\n", $o->get_title(), $o->get_cost());
            $o->add_cost($total);
        }
    }
    $message .= sprintf("%s ----> \$%0.02f\n", "Total MSRP, as configured", $total);
        
    $message = preg_replace("/\s+/g"," ", $buf);
    // now we'll build the message headers
      $headers = "From: $from\r\n" .
         "MIME-Version: 1.0\r\n" .
         "Content-Type: multipart/mixed;\r\n" .
         " boundary=\"{$mime_boundary}\"";

      // next, we'll build the message body
      // note that we insert two dashes in front of the
      // MIME boundary when we use it
      $message = "This is a multi-part message in MIME format.\n\n" .
         "--{$mime_boundary}\n" .
         "Content-Type: text/plain; charset=\"iso-8859-1\"\n" .
         "Content-Transfer-Encoding: 7bit\n\n" .
        $message . "\n\n";

      // now we'll insert a boundary to indicate we're starting the attachment
      // we have to specify the content type, file name, and disposition as
      // an attachment, then add the file content and set another boundary to
      // indicate that the end of the file has been reached
      $message .= "--{$mime_boundary}\n" .
         "Content-Type: {$type};\n" .
         " name=\"{$name}\"\n" .
         //"Content-Disposition: attachment;\n" .
         //" filename=\"{$fileatt_name}\"\n" .
         "Content-Transfer-Encoding: base64\n\n" .
         $data . "\n\n" .
         "--{$mime_boundary}--\n";

    // now we just send the message
    if (@mail($to, $subject, $message, $headers)){
        mail($from, "Receipt: {$subject}", $message, $headers);
        echo "<p>Message Sent</p>";
        echo '<p><a href="http://www.coba.unt.edu/firel/ufsc/">Return to UFSC Home</a></p>';
    } else
        echo "Failed to send";
    header("Location: $ptr");
?>