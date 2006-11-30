<?php
   database_connect();
   $query = "SELECT date, title FROM tbl_news";
   $result = mysql_query($query);
   $results = null;
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   $results[] = $row;
   database_disconnect();
?>
<dl>
<?
   foreach ($results as $event)
      echo"
         <dt>{$event['date']}</dt>
         <dd>
            <ul>
               <li>{$event['title']}</li>
            </ul>
         </dd>
         ";
?>
</dl>