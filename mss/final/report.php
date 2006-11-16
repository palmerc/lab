<?php
$title = "2006-07 Monthly Committee Report";
require('stc-template.php');

if ($_SERVER['REQUEST_METHOD'] == "POST")
{
   if($_POST['Report'] == 'Yes')
   {
      $to = 'wng0001@unt.edu';
      $subject = 'Form';
      $message = 'Name: '.$_POST['select'].'\n'.'Committee: '.$_POST['select2'].'\n'.'Date of Submission: '.$_POST['textfield'].'\n';
      $q1 = '1. What tasks did your committee accomplish in the past month? \n'.$_POST['Q1'].'\n\n'; 
      $q2 = '2. What tasks do you have planned for the upcoming months? \n'.$_POST['Q2'].'\n\n';
      $q3 = '3. Did you hold any meetings? If so, what was the purpose and how many attended? \n'.$_POST['Q3'].'\n\n';   
      $q4 = '4. Please list the name of the volunteers who helped you this month. Describe in detail what they did for you and how much time they spent performing these duties. Please provide as much information as possible so we can credit these volunteers with the Superstar points they deserve \n'.$_POST['Q4'].'\n\n';
      $q5 = '5. Are there any committee or community-related problems that the officers or Council should know about? \n'.$_POST['Q5'].'\n\n';
      $q6 = "6. Do you have any topics that need to be discussed during this month's council meeting? If so, please list them. \n".$_POST['Q6'].'\n\n';
      $q7 = '7. If you have anything to discuss during the council meeting, please indicate how much time you will need to discuss them. Please stay within your time limit. \n'.$_POST['Q7'].'\n\n';
      $q8 = '8. Do you have any materials that are CAA worthy? Please submit them with this form and indicate below what category your materials fulfill. \n'.$_POST['Q8'].'\n\n';
      $q9 = '9. Have you updated your page on the website? Please email the webmaster at webmaster@stc-dfw.org with any necessary changes. \n'.$_POST['Q9'].'\n\n';
      $q10 = '10. Please enter the name of the newsletter article that you have submitted to the newsletter editor this month. \n'.$_POST['Q10'].'\n\n';
      $q11 = '11. Will you need any special arrangements at the meeting (e.g., table, AV equipment)?  Please list necessary items here if applicable. \n'.$_POST['Q11'].'\n\n';
      
      $newmessage = $message.$q1.$q2.$q3.$q4.$q5.$q6.$q7.$q8.$q9.$q10.$q11;
         
      $headers = 'From: wng0001@unt.edu'. "\r\n".
               'Reply-To: wng0001@unt.edu'. "\r\n".
               'X-Mailer: PHP/'. phpversion();
      
      $finalmessage = wordwrap ($newmessage, 70, "\n", 1);
      
      $success = mail($to, $subject, $finalmessage, $headers);
      if ($success == FALSE)
         echo 'ERROR: Call of mail() has failed.  Reason Unknown.';
      else
         echo '<p>Thank you for your submission.</p>';
      
   }
   else
   {
      $to      = 'wng0001@unt.edu';
      $subject = 'Form';
      $message = 'Name: '.$_POST['select'].'\n'.'Committee: '.$_POST['select2'].'\n'.'Date of Submission: '.$_POST['textfield'].'\n'.'Nothing To Report \n';
      $headers = 'From: wng0001@unt.edu' . "\r\n" .
               'Reply-To: wng0001@unt.edu' . "\r\n" .
               'X-Mailer: PHP/' . phpversion();
      $finalmessage = wordwrap($message, 70, "\n", 1);

      $success = mail($to, $subject, $finalmessage, $headers);
      if ($success == FALSE)
         echo 'ERROR: Call of mail() has failed.  Reason Unknown.';
      else
         echo '<p>Thank you for you submission.</p>';
   }
}
?>

<h1>2006-07 Monthly Committee Report</h1>
<p>You must complete and submit at least the first four items on this sheet every month.</p>

<form id="form1" action="<?echo $_SERVER['PHP_SELF'] ?>" method="post">
   <label>Your name:
   <select name="select">
      <option value="NULL">Select Your Name</option>
      <option value="Name 1">Name 1</option>
      <option value="Name 2">Name 2</option>
      <option value="Name 3">Name 3</option>
   </select>
   </label>
   
   <p>
   <label>Your committee:
   <select name="select2">
      <option value="NULL">Select Your Committee</option>
      <option value="Committee A">Committee A</option>
      <option value="Committee B">Committee B</option>
      <option value="Committee C">Committee C</option>
   </select>
   </label>
   </p>

   <p>
   <label>Today&rsquo;s date:
   <input type="text" name="textfield" />
   </label>
   </p>
   
   <p>Do you have anything to report?
   <label>
   <input name="Report" type="radio" value="Yes" checked="checked" />
   Yes</label>
   <label>
   <input type="radio" name="Report" value="No" />
   No</label>
   <br />
   </p>
   
   <hr />
   <p>If you have something to report, please answer the following questions, otherwise, click Submit:</p>
   <p>
   <input type="submit" name="Submit2" value="Submit" />
   </p>

   <p>1. What tasks did your committee accomplish in the past month?</p>
   <p>
   <label>Answer:
   <textarea name="Q1" cols="60" id="Q1">Enter your answer here</textarea>
   </label>
   </p>
   <p>&nbsp;</p>

   <p>2. What tasks do you have planned for the upcoming months?</p>
   <p>
   <label>Answer: 
   <textarea name="Q2" cols="60" id="Q2">Enter your answer here</textarea>
   </label>
   </p>
   <p>&nbsp;</p>
   <p>3. Did you hold any meetings? If so, what was the purpose and how many attended?</p>

  <p>
    <label>Answer:
    <textarea name="Q3" cols="60" id="Q3">Enter your answer here</textarea>
    </label>
</p>
  <p>&nbsp;</p>
  <p>4. Please list the name of the volunteers who helped you this month. Describe in detail<br />
    what they did for you and how much time they spent performing these duties. Please<br />

    provide as much information as possible so we can credit these volunteers with the<br />
    Superstar points they deserve</p>
  <p>
    <label>Answer:
    <textarea name="Q4" cols="60" id="Q4">Enter your answer here</textarea>
    </label>
</p>
  <p>&nbsp;</p>

  <p>5. Are there any committee or community-related problems that the officers or Council<br />
    should know about?</p>
  <p>
    <label>Answer:
    <textarea name="Q5" cols="60" id="Q5">Enter your answer here</textarea>
    </label>
</p>
  <p>&nbsp;</p>

  <p>6. Do you have any topics that need to be discussed during this month's council<br />
    meeting? If so, please list them.</p>
  <p>
    <label>Answer:
    <textarea name="Q6" cols="60" id="Q6">Enter your answer here</textarea>
    </label>
</p>
  <p>&nbsp;</p>

  <p>7. If you have anything to discuss during the council meeting, please indicate how<br />
    much time you will need to discuss them. Please stay within your time limit.</p>
  <p>
    <label>Answer:
    <textarea name="Q7" cols="60" id="Q7">Enter your answer here</textarea>
    </label>
</p>
  <p>&nbsp;</p>

  <p>8. Do you have any materials that are CAA worthy? Please submit them with this form<br />
    and indicate below what category your materials fulfill.</p>
  <p>
    <label>Answer:
    <textarea name="Q8" cols="60" id="Q8">Enter your answer here</textarea>
    </label>
</p>
  <p>&nbsp;</p>

  <p>9. Have you updated your page on the website? Please email the webmaster at<br />
    <a href="mailto:webmaster@stc-dfw.org">webmaster@stc-dfw.org</a> with any necessary changes.</p>
  <p>
    <label>Answer:
    <textarea name="Q9" cols="60" id="Q9">Enter your answer here</textarea>
    </label>
</p>

  <p>&nbsp;</p>
  <p>10. Please enter the name of the newsletter article that you have submitted to the<br />
    newsletter editor this month.</p>
  <p>
    <label>Answer:
    <textarea name="Q10" cols="60" id="Q10">Enter your answer here</textarea>
    </label>
</p>

  <p>&nbsp;</p>
  <p>11. Will you need any special arrangements at the meeting (e.g., table, AV equipment)?<br />
    Please list necessary items here if applicable.</p>
  <p>
    <label>Answer:
    <textarea name="Q11" cols="60" id="Q11">Enter your answer here</textarea>
    </label>
  </p>

  <p>
    <label>
    <input type="submit" name="Submit" value="Submit" />
    </label>
  </p>
</form>