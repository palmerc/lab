<?php

start_form();
show_checkbox("option_privacy", "Privacy Panels");
end_form();


function show_checkbox($name, $label) {
  $checked = ($_POST[$name] ? "checked=\"checked\"" : "");
  echo "<label for=\"$name\"><input type=\"checkbox\" name=\"$name\" id=\"$name\" $checked />$label</label>\n";
}

function show_radio($name, $label) {
  echo "<label for=\"$name\"><input type=\"radio\" name=\"$name\" id=\"$name\" />$label</label>\n";
}

function start_form() {
  echo "<form action=\"{$_SERVER['PHP_SELF']}\" method=\"post\">\n";
}

function end_form() {
  echo "</form>\n";
}

?>