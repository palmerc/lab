<?php

function mprint_r($var) {
    echo "<pre>";
    print_r($var);
    echo "</pre>";
}

function sanitize_attribute($s) {
    return str_replace('"', '&#034;', $s);
}

?>