<?php
$title = "The Bitter Buffalo";
$section = "Java SSH/Telnet Application";

require ("../cp-template.php");
?>
<img class="leftside" src="<? echo "{$ptr}i/ramblo.jpg" ?>" alt="Ramblo the SSH Blowfish" width="72" height="54" border="0" />
<?
if ($_POST['applet']) {
   echo'<p>Be patient this applet is pretty big so it may take a minute to load. Closing this browser window will terminate the applet</p>';
   echo"
   <applet name=\"javassh\" archive=\"{$ptr}ssh/mindterm-3.0.1.jar\" code=\"com.mindbright.application.MindTerm.class\" height=\"0\" width=\"0\">
          ";
   echo'
      <param name="sepframe" value="true" />
      <param name="debug" value="true" />
      <param name="visual-bell" value="true" />
      <param name="server" value="" />
      <param name="geometry" value="80x40" />
   </applet>
          ';  
} else {
   echo"
   <form method=\"post\" action=\"{$_SERVER['PHP_SELF']}\" >
      <p>If you are planning to use this application a lot I recommend the Java Web Start version.</p>
      <a href=\"{$ptr}ssh/mindterm_ws.jnlp\">Java Web Start Version</a>
          ";
   echo'
      <input type="submit" name="applet" value="Java Applet" />
   </form>
          ';
}
?>
<p>The client is <a href="http://www.appgate.com/mindterm">MindTerm</a> 3.0.1, supporting SSH v1 and v2.</p>
<a href="http://www.java.com"><img src="<? echo"{$ptr}i/get_java_red_button.gif" ?>" alt="Get Java Here" /></a>

