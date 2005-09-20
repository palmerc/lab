<?php

$title="Princess Craft - Request Quote";
$sec_title = "Customer Quote Request Form";
$full_width = true;

require("form_functions.php");
require("../../pc-template-clean.php");

?>
<div id="main">
  <form action="mail.php" method="post">
  <table cellpadding="4" cellspacing="2" border="0" width="450">
    <tr class="row">
      <td colspan="2">
        <h3 class="formheader">Requester Information:</h3>
      </td>
    </tr>

    <tr class="row">
      <td>
        First Name<br />
        <input type="text" name="First_name" value="" size="30" />
      </td>
      <td colspan="2">
        Last Name<br />
        <input type="text" name="Last_name" value="" size="30" />
      </td>
    </tr>

    <tr class="row">
      <td>
        Address 1<br />
        <input type="text" name="Address1" value="" size="30" />
      </td>
      <td colspan="2">
        Address 2<br />
        <input type="text" name="Address2" value="" size="30" />
      </td>
    </tr>

    <tr class="row">
      <td>
        City<br />
        <input type="text" name="City" value="" size="30" />
      </td>
      <td>
        State<br />
        <input type="text" name="State" value="" size="5" />
      </td>
      <td>
        Zip/Postal Code<br />
        <input type="text" name="Zip_code" value="" size="10" />
      </td>
    </tr>

    <tr class="row">
      <td>
        Telephone<br />
        <input type="text" name="Telephone" value="" size="30" />
      </td>
      <td colspan="2">
        Alternate Phone<br />
        <input type="text" name="Telephone2" value="" size="30" />
      </td>
    </tr>

    <tr class="row">
      <td>
        Requester's Email<br />
        <input type="text" name="Requesters_email" value="" size="30" />
      </td>
    </tr>

    <tr class="row">
      <td colspan="3">
        Additional Comments<br />
        <textarea NAME="Comments" ROWS=5 COLS=40></textarea>
      </td>
    </tr>
</table>
<?php
passthru_post();
?>
<input type="submit" value="Submit" />
</form>
</div>