<?php
function vote($user, $vote_date, $vote_value)
{
   $query = "INSERT INTO votes VALUES(null,'{$user}','{$vote_date}','{$vote_value}')";
   $result = mysql_query($query);
   if (mysql_affected_rows() == 1) {
      return true;
   }
   return false;
}
function vote_results()
{
   $query = "SELECT * FROM votes";
   $result = mysql_query($query);
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   {
      $results[] = $row;
   }
   return $results;
}
?>