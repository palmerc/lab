<?php

class cache {

	function cache() {
		$this->r =& new httpsession();
		if(!is_dir("cache")) {
			mkdir("cache");
            if(!is_dir("cache")) die("Cache dir could not be created.");
            if(!is_writable("cache")) die("Cache dir is not writable.");
		}
	}
	
	function fetch($url, $ttl) {
		$m = md5($url);
		$cf = "cache/{$m}";
		if(file_exists($cf) && (filemtime($cf) >= time() - (60 * $ttl))) {
			return file_get_contents($cf);
		} else {
			$this->r->get($url);
			$f = fopen($cf, "w");
			fputs($f, $this->r->data);
			fclose($f);
			return $this->r->data;
		}
	}

}

?>