<?php

$cscmd="cs";
/**
    Copied, for the most part, from http://groups.yahoo.com/group/ClearSilver/message/345
    Original author: Dan Janowski
    
    Todo:
    * support for dumping entire arrays
    * Support for getting data once set?
*/
class Hdf {

    var $hdfTarget;
    var $hdfFile;
    var $level=1;
    var $tmpfile;
    var $hdfList="";
    var $csList="";

    function Hdf() {
    }

    function addLocal() {
        if ( $this->tmpfile != "" )
            return;

        $this->tmpfile = tempnam("/tmp", "hdf-");
        $this->hdfFile = fopen($this->tmpfile, "w+");
        $this->hdfList .= " -h {$this->tmpfile}";
    }

    function push($hname) {
        fwrite($this->hdfFile, str_repeat(" ", $this->level) . $hname . "{\n");
        $this->level++;
    }

    function pop() {
        $this->level--;
        fwrite($this->hdfFile, str_repeat(" ", $this->level) . "}\n");
    }

    function put($key, $val) {
        if ( strstr($val, "\n") ) {
            fwrite($this->hdfFile,
                str_repeat(" ", $this->level) .
                $key . " <<EOM\n" . $val . "\nEOM\n");
        } else {
            fwrite($this->hdfFile,
                str_repeat(" ", $this->level) .
                $key . " = " . $val . "\n");
        }
    }

    function addHdf($hdf_file) {
        if ( file_exists($hdf_file) ) {
            $this->hdfList .= " -h $hdf_file";
        } elseif ( file_exists( $_SERVER[HDF_DIR] . "/" . $hdf_file ) ) {
            $this->hdfList .= " -h " . $_SERVER[HDF_DIR] . "/" . $hdf_file;
        } else {
            trigger_error("unable to find $hdf_file");
        }
    }

    function addCs($cs_file) {
        $this->csList .= " -c $cs_file";
    }

    function close($rem=1) {
        if ( $this->hdfFile != "" )
            fclose($this->hdfFile);
        global $cscmd;
        $cmd="$cscmd {$this->hdfList} {$this->csList}";
        passthru($cmd);
        if ( $this->tmpfile != "" && $rem ) unlink($this->tmpfile);
    }

    function getHdfFile() {
        return $this->tmpfile;
    }

    function getCommand() {
        return "{$this->hdfList} {$this->csList}";
    }

    function pass($hdf, $cs, $addl="") {
        passthru("$cscmd -h $hdf -c $cs $addl");
    }

    function abort($rem=1) {
        fclose($this->hdfFile);
        if ( $this->tmpfile && $rem ) unlink($this->tmpfile);
    }
}

function hdfLoc($subpath) {
    return $_SERVER[HDF_DIR] . "/" . $subpath;
}
?>