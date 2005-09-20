<?php

require("barcodes.class.php");
$x =& new Code39($pdf, 0, 0);
$x->Draw("A");

?>