<?php

class Barcode {
    function Barcode($pdf, $x, $y, $xscale=1, $height=10) { //all units in mm
        $this->pdf =& $pdf;
        $this->pdf->SetFillColor(0);
        $this->x = $x;
        $this->y = $y;
        $this->xscale = $xscale;
        $this->height = $height;
    }
    function MoveTo($x, $y) {
        $this->x = $x;
        $this->y = $y;
    }
    
    function _chars($str) {
        $ret = array();
        for($i=0;$i<strlen($str);$i++) {
            $ret[] = $str{$i};
        }
        return $ret;
    }
}

class Code39_Ascii extends Barcode {
    function Draw($text) {
        $this->DrawSingle("*");
        foreach($this->_chars($text) as $c) {
            $this->DrawSingle($c);
        }
        $this->DrawSingle("*");
    }
    function DrawSingle($c) {
        global $code39_table;
        $parts = $code39_table[$c] or die("Invalid character $c");
        foreach($this->_chars($parts) as $i=>$b) {
            $this->_render($b, $i);
        }
    }
    function _render($type, $index) {
        global $code39_widths;
        $w = $code39_widths[$type] or die("Invalid width at index $index");
        $w *= $thix->xscale;
        if($index % 2 == 0) {
            //drawing a bar
            $this->pdf->Rect($this->x, $this->y, $w, $this->height, 'F');
        }
        $this->x += $w;
    }
}

class Code128_Ascii extends Barcode {
    function Draw($text,$start_code) {
        $this->DrawSingle($code_types[$start_code]);
        foreach($this->_chars($text) as $c) {
            $this->DrawSingle($c);
        }
        $this->DrawSingle($stop);
    }
    function DrawSingle($c) {
        global $code128_table;
        $parts = $code128_table[$c] or die("Invalid character $c");
        foreach($this->_chars($parts) as $i=>$b) {
            $this->_render($b, $i);
        }
    }
    function _render($type, $index) {
        global $code128_widths;
        $w = $code128_widths[$type] or die("Invalid width at index $index");
        $w *= $thix->xscale;
        if($index % 2 == 0) {
            //drawing a bar
            $this->pdf->Rect($this->x, $this->y, $w, $this->height, 'F');
        }
        $this->x += $w;
    }
}

require("code39_table.php");
require("code128_table.php");

?>