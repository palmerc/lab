<?php
   $title = "Lone Star Community - Vote";
   require('../stc-template.php');
?>

<h1>Cast Your Vote</h1>
<p align="left">Honestly, do you all agree that this is the last time we have kids do our website?</p>

<form action="<? echo $_SERVER['PHP_SELF']?>" method="post">
<table>
   <tr>
       <td><input type="radio" name="vote" value="0" /></td><td>Yes</td>
   </tr>
   <tr>
       <td><input type="radio" name="vote" value="1" /></td><td>No</td>
   </tr>
</table>
<p><input type="submit" name="submit" value="Vote" /></p>  			 
</form>



