<?php
   database_connect();
   $query = "SELECT date, title FROM tbl_news";
   $result = mysql_query($query);
   $results = null;
   while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
   $results[] = $row;
   if ($results)
      $event = $results[0]['title'];
   else
      $event = null;
   database_disconnect();
?>
<h1>Calendar</h1>
   <dt>Nov 7</dt>
   <dd><ul><li><div class="event"><a href="/news/view.php?/2006/11/02/logistics-student-association-%28logsa%29-meeting">The comma in ethics</a></div></li>
   <li><a href="/news/view.php?/2006/11/02/logistics-student-association-%28logsa%29-meeting">Students for Semicolons meeting</a></li>
   </ul></dd>
   <dt>Nov 9 - Nov 10</dt>
   <dd><ul><li><a href="/news/view.php?/2006/07/27/logistics-management-seminar">Voting Session "The exclamation point: Do we Need it?"</a></li>
   </ul></dd>
   <dt>Nov 9</dt>
   <dd><ul><li><a href="/news/view.php?/2006/11/02/logistics-student-association-%28logsa%29-industry-day">Lone Star meeting in Fort Worth</a></li>
   </ul></dd>
   <dt>Nov 17</dt>

   <dd><ul><li><a href="http://www.murphycenter.unt.edu/leadershiplunch.php">2006 STC Leadership Luncheon</a></li>
   <li><a href="/news/view.php?/2006/11/02/1st-annual-unt-logistics-challenge">1st Annual Comma Challenge</a></li>
   </ul></dd>
   </dd>