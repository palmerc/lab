<?php
    function passthru_post(){
        foreach($_POST as $k=>$v) {
            printf("<input type=\"hidden\" name=\"%s\" id=\"%s\" value=\"%s\" />", htmlspecialchars($k), htmlspecialchars($k), htmlspecialchars($v));
        }
        echo "\n";
    }
?>