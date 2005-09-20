<?php


class httpsession {

	function httpsession() {
		//setup stuff
		$this->cookies=array();
		$this->response_code=0;
		$this->data="";
		$this->headers="";
		$this->lasturl=false;
		$this->showProgress=false;
	}
	
	function setProgress($bool) {
		$this->showProgress=$bool;
	}

	function checkpoint() {
		//this should really use a stack..
		$this->backup = array(
		"cookies" => $this->cookies,
		"lasturl" => $this->lasturl
		);
	}

	function rollback() {
		$this->cookies = $this->backup['cookies'];
		$this->lasturl = $this->backup['lasturl'];
	}
	
	//note that referer is kept in a member variable, and the first post will
	//  set it.
	function post($URL, $data=false) {
		if($URL=="") return "404";
		
	  $URL_Info=parse_url($URL);
	  $values=array();
	  if($data) foreach($data as $key=>$value)
  	  $values[]=urlencode($key)."=".urlencode($value);
  	$data_string=implode("&",$values);

	  if(!isset($URL_Info["port"]))
  	  $URL_Info["port"]=80;

	  $request="";
		if($data) {
  		$request.="POST ".$URL_Info["path"]."?".$URL_Info["query"]." HTTP/1.0\r\n";
  	} else {
  		$request.="GET ".$URL_Info["path"]."?".$URL_Info["query"]." HTTP/1.0\r\n";
  	}
  	$request.="Host: {$URL_Info["host"]}\r\n";
  	$request.="User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.7) Gecko/20040626 Firefox/0.9.1\r\n";
  	$request.="Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n";
  	$request.="Accept-Language: en-us,en;q=0.5\r\n";
  	$request.="Accept-Encoding: gzip,deflate\r\n";
  	$request.="Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n";
  	if($this->lasturl) 
  		$request.="Referer: {$this->lasturl}\r\n";
  	$cookie = $this->getCookies();
  	if($cookie) $request.="Cookie: {$cookie}\r\n";
  	if($data) {
			$request.="Content-Type: application/x-www-form-urlencoded\r\n";
  		$request.="Content-Length: ".strlen($data_string)."\r\n";
  	}
  	$request.="Connection: close\r\n\r\n";
  	if($data) $request.=$data_string;
  	
  	//echo " >> {$request}\n";
  	$fp = fsockopen($URL_Info["host"],$URL_Info["port"]);
  	if(!$fp) die("Invalid handle for $URL");
  	
  	fputs($fp, $request);
  	$result="";
  	while(!feof($fp)) {
			$result .= fgets($fp, 4096);
			if($this->showProgress) echo ".\n";
  	}
  	fclose($fp);

		$end_of_headers = strpos($result,"\r\n\r\n");
		$this->data = substr($result, $end_of_headers+4);
		$this->headers = substr($result, 0, $end_of_headers);
		$this->mergeCookies();
		$this->response_code = substr($this->headers, 9, 3)-0;
		if(strpos($this->headers, "Content-Encoding: gzip") !== false)
			$this->data = gzinflate(substr($this->data, 10));
		$this->lasturl=$URL;
		
		return $this->response_code;  
	}
	
	function get($url) {
		return $this->post($url);
	}
	
	function getCookies() {
		$x=array();
		foreach($this->cookies as $k=>$v) {
			$x[] = "$k=$v";
		}
		return join('; ', $x);
	}
	
	function mergeCookies() {
		if(preg_match_all("/^Set-Cookie: (.+?)=(.*?);/m", $this->headers, $m)) {
			foreach($m[1] as $i=>$v) {
				$this->cookies[$v] = $m[2][$i];
			}
		}
#		print_r($this->cookies);
	}
	function getHeader($name, $urldecode=false) {
		if(preg_match_all("/^$name: *(.+)\$/mi", $this->headers, $m)) {
			if($urldecode) $m[1][0] = urldecode($m[1][0]);
			return trim($m[1][0]);
		}
		return false;
	}


}
