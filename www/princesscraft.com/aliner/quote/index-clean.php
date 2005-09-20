<?php

$title="Princess Craft: Customize an Aliner";
$sec_title = "Customize Your Aliner";
$full_width = true;

require("../../pc-template-clean.php");

?>

<script type="text/javascript" src="../../c/nicetitle_labels.js"></script>


<?php

error_reporting(E_ALL);
ini_set("display_errors", true);


require("options.class.php");
$options =& new optionlist("../../choosy/alineroptions.xml");
$section = $options->get_section_list();

$model = @$_REQUEST['model'];
$m = $options->get_model($model) or die("Unknown model");

foreach($section as $sec) {
    foreach($sec->get_list() as $o) {
        if($o->is_applicable($m)) {
            echo $o->to_htmlform();
        } else {
            echo "Not applicable: {$o->name}<br />\n";
        }
    }
    //echo "<li>".$sec->to_str()."</li>\n";
}

echo "<h3>Aliners:</h3>\n";


?>
