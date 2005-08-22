<?php

$title = 'Bruce Hall - Calendar';
$sec_title = 'Events Calendar';

require('bha-template.php');

?>
<form method="post" name="calendarPage" action="calendar.php">
<table class="calendar" cellspacing="0" cellpadding="0" border="0">
<tr>
<td class="calmenu" style="width: 100%; text-align: center" colspan="7">
<table cellspacing="0" cellpadding="0" border="0">
<tr>
<td nowrap class="calmenu" style="width: 250px">
&nbsp;<b>Today's Date: </b>August 22, 2005
</td>
<td class="calmenu" style="width: 460px; text-align: right">
<b>Month: </b>
<select name="month">
<option value="1">January</option>
<option value="2">February</option>

<option value="3">March</option>
<option value="4">April</option>
<option value="5">May</option>
<option value="6">June</option>
<option value="7">July</option>
<option value="8" selected>August</option>
<option value="9">September</option>
<option value="10">October</option>
<option value="11">November</option>

<option value="12">December</option>
</select>
&nbsp;&nbsp;
<b>Year: </b>
<select name="year">
<option value="2003">2003</option>
<option value="2004">2004</option>
<option value="2005" selected>2005</option>
<option value="2006">2006</option>
<option value="2007">2007</option>

<option value="2008">2008</option>
<option value="2009">2009</option>
<option value="2010">2010</option>
<option value="2011">2011</option>
<option value="2012">2012</option>
<option value="2013">2013</option>
<option value="2014">2014</option>
<option value="2015">2015</option>
</select>&nbsp;&nbsp;

<input type="button" style="width: 80px" name="refresh" value="Refresh" onclick="ensureRefresh();" />&nbsp;&nbsp;&nbsp;
<br />
<a class="calmenu" href="caladmin/caladmin.php" target="caladmin"><u>Administration</u></a>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td class="calhead" colspan="7">
<table class="calhead" cellspacing="0" cellpadding="0" border="0">
<tr>
<td class="caltitle" colspan="3">
<input type="hidden" name="day" value="" />
<input type="hidden" name="view" value="calendar" />

</td>
</tr>
<tr>
<td class="calmonths" style="text-align: left">
&nbsp;<a class="calmonths" href="calendar.php?month=7&amp;year=2005"><b>&lt;&lt; July 2005</b></a>
</td>
<td class="calmonths" style="width: 300px; font-size: 16px">
<b>August 2005</b>
</td>
<td class="calmonths" style="text-align: right">
<a class="calmonths" href="calendar.php?month=9&amp;year=2005"><b>September 2005 &gt;&gt;</b></a>
</td>
</tr>

</table>
</td>
</tr>
<tr>
<td class="calweekday" nowrap>
<b>Sunday</b>
</td>
<td class="calweekday" nowrap>
<b>Monday</b>
</td>
<td class="calweekday" nowrap>
<b>Tuesday</b>
</td>
<td class="calweekday" nowrap>

<b>Wednesday</b>
</td>
<td class="calweekday" nowrap>
<b>Thursday</b>
</td>
<td class="calweekday" nowrap>
<b>Friday</b>
</td>
<td class="calweekday" nowrap>
<b>Saturday</b>
</td>
</tr>
<tr>

<td class="calday" style="cursor: default">
&nbsp;
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(1, 8, 2005)">
1<br />
rewrtwer<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(2, 8, 2005)">
2<br />
Taekwon-do<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(3, 8, 2005)">
3<br />

</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(4, 8, 2005)">
4<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(5, 8, 2005)">
5<br />
Party<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(6, 8, 2005)">
6<br />
</td>
</tr>
<tr>

<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(7, 8, 2005)">
7<br />
On tour<br />
kill my dog<br />
Test Event<br />
Test Event<br />
<center>&lt;&lt; more &gt;&gt;</center>
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(8, 8, 2005)">
8<br />

</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(9, 8, 2005)">
9<br />
Taekwon-do<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(10, 8, 2005)">
10<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(11, 8, 2005)">
11<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(12, 8, 2005)">
12<br />

Party<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(13, 8, 2005)">
13<br />
</td>
</tr>
<tr>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(14, 8, 2005)">
14<br />
steph man<br />
On tour<br />
kill my dog<br />

Test Event<br />
<center>&lt;&lt; more &gt;&gt;</center>
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(15, 8, 2005)">
15<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(16, 8, 2005)">
16<br />
Taekwon-do<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(17, 8, 2005)">

17<br />
Rickins<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(18, 8, 2005)">
18<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(19, 8, 2005)">
19<br />
jims meeting<br />
Party<br />
</td>

<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(20, 8, 2005)">
20<br />
TEST EVENT<br />
</td>
</tr>
<tr>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(21, 8, 2005)">
21<br />
Sunny day<br />
any<br />
On tour<br />

kill my dog<br />
<center>&lt;&lt; more &gt;&gt;</center>
</td>
<td nowrap class="calday" style="background-color: #FFFFFF" onmouseover="tdmv(this)" onmouseout="tdtmo(this)" onclick="sendDay(22, 8, 2005)">
22<br />
rererer<br />
tester<br />
day<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(23, 8, 2005)">

23<br />
Test entry<br />
Book Conference room<br />
Taekwon-do<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(24, 8, 2005)">
24<br />
test<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(25, 8, 2005)">
25<br />

</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(26, 8, 2005)">
26<br />
fun stuff<br />
asdf<br />
Party<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(27, 8, 2005)">
27<br />
Buy New Calendar<br />
</td>

</tr>
<tr>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(28, 8, 2005)">
28<br />
Test Event<br />
On tour<br />
kill my dog<br />
Test Event<br />
<center>&lt;&lt; more &gt;&gt;</center>
</td>

<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(29, 8, 2005)">
29<br />
All Day Event<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(30, 8, 2005)">
30<br />
Taekwon-do<br />
</td>
<td nowrap class="calday" onmouseover="tdmv(this)" onmouseout="tdmu(this)" onclick="sendDay(31, 8, 2005)">
31<br />
</td>
<td class="calday" style="cursor: default">

&nbsp;
</td>
<td class="calday" style="cursor: default">
&nbsp;
</td>
<td class="calday" style="cursor: default">
&nbsp;
</td>
</tr>
</table>
</form>