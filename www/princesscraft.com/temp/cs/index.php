<?php
/* 
    Note: requires an executable known as 'cs' or 'clearsilver' to be on the path.  Set as appropriate in hdf.class.php
*/
require("hdf.class.php");
$x =& new Hdf();
$x->addLocal(); //means "use a temp file"

$x->put("title", "Blah");
$x->addCs("demo.cs");

$x->put("body", "This is a test");
$x->put("item.1.title", "Item One");
$x->put("item.1.date", "2004-02-12");
$x->put("item.2.title", "Item Two");
$x->put("item.2.date", "2004-05-01");
$x->put("item.3.title", "Item Three");
$x->put("item.3.date", "2004-09-30");

$x->close(); //change this thing to "render" perhaps?

?>