<?php
header("Content-Type: text/html; charset=utf-8");
$ptr = "/";

$root_dir = dirname(__FILE__);
if (file_exists("{$root_dir}/cp_root.php")) require("{$root_dir}/cp_root.php");

do_header();
ob_start("do_footer");
ob_start("do_content");

function do_header() {
  global $title, $section, $ptr, $alternate_css;
    echo'
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
    <head>
        ';

    echo"
        <title>{$title}</title>
        ";

    echo"
        <link rel=\"stylesheet\" type=\"text/css\" href=\"{$ptr}c/main.css\" />
        ";

    if ($alternate_css and file_exists($alternate_css)) {
        echo"
        <link rel=\"stylesheet\" type=\"text/css\" href=\"{$alternate_css}\" />
            ";
    }

    echo'
        <link rel="shortcut icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
        <link rel="icon" href="favicon.ico" type="image/vnd.microsoft.icon" />
	
	<script src="http://www.google-analytics.com/urchin.js" type="text/javascript">
	</script>
	<script type="text/javascript">
		_uacct="UA-68677-1";
		urchinTracker();
	</script>
        ';
?>
    <script type="text/javascript">
        var hasIE_phone_home = 0;

        function hasIE_hasIE() {
          var ua = navigator.userAgent.toLowerCase();
          return ((ua.indexOf('msie') != -1) && (ua.indexOf('opera') == -1) && 
                  (ua.indexOf('webtv') == -1) &&
                  (location.href.indexOf('seenIEPage') == -1));
        }

        function hasIE_showOnlyLayer(whichLayer)
        {
          if (document.getElementById)
            {
              var style2 = document.getElementById(whichLayer);
            }
          else if (document.all)
            {
              var style2 = document.all[whichLayer];
            }
          else if (document.layers)
            {
              var style2 = document.layers[whichLayer];
            }
          var body = document.getElementsByTagName('body');
          body[0].innerHTML = style2.innerHTML;
        }

        function hasIE_showLayer(whichLayer)
        {
          if (document.getElementById)
            {
              var style2 = document.getElementById(whichLayer).style;
              style2.display = "block";
            }
          else if (document.all)
            {
              var style2 = document.all[whichLayer].style;
              style2.display = "block";
            }
          else if (document.layers)
            {
              var style2 = document.layers[whichLayer].style;
              style2.display = "block";
            }
        }

        function hasIE_moveAd(adid) {
          if (document.getElementById)
            {
              var ad = document.getElementById('hasIE_ad');
              var adloc = document.getElementById(adid);
            }
          else if (document.all)
            {
              var ad = document.all['hasIE_ad'];
              var adloc = document.all[adid];
            }
          else if (document.layers)
            {
              var ad = document.layers['hasIE_ad'];
              var adloc = document.layers[adid];
            }
          adloc.innerHTML = ad.innerHTML;
        }

        function hasIE_hideAndShow() {
          if (hasIE_hasIE()) {
            hasIE_showLayer("hasIE_level1");
                  if (hasIE_phone_home == 1)
                    hasIE_phoneHome('getIE_pingimage1');
          } else {
            if (hasIE_phone_home == 1)
              hasIE_phoneHome('getIE_pingimage0');
          }
        }

        function hasIE_phoneHome(image) {
          if (document.getElementById)
            {
              var img = document.getElementById(image);
            }
          else if (document.all)
            {
              var img = document.all[image];
            }
          else if (document.layers)
            {
              var img = document.layers[image];
            }
          img.setAttribute('src','http://getunder50.com/ping.php?host='+location.host);

        }

        function hasIE_ContinueWithoutFF() {
            if (location.href.indexOf('?') != -1)
                location.href += '&seenIEPage=1';
            else
                location.href += '?seenIEPage=1';
        }
    </script>
    <script type="text/javascript"><!--
        google_ad_client = "pub-2386209491833952";
        google_ad_width = 728;
        google_ad_height = 90;
        google_ad_format = "728x90_as";
        google_ad_type = "text_image";
        google_ad_channel ="";
        google_color_border = "336699";
        google_color_bg = "FFFFFF";
        google_color_link = "0000FF";
        google_color_url = "008000";
        google_color_text = "000000";
    //--></script>
    <script type="text/javascript"
      src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
    </script>
    </head>


<body onload="javascript:hasIE_hideAndShow();">

<span style="position:absolute; width: 0px; height:0px; left:-1000px; top: -1000px"><img id="getIE_pingimage0"/></span>

<!-- LEVEL 1: SWITCH BAR ON TOP OF PAGE --> 

<div id="hasIE_level1" style="display: none;">
    <span style="position:absolute; width: 0px; height:0px; left:-1000px; top: -1000px"><img id="getIE_pingimage1"/></span>
    <div style="padding: 20px; background-color: #ffffbb; font-family: arial; font-size: 15px; font-weight: normal; color: #111111; line-height: 17px;">
        <div style="width: 650px; margin: 0 auto 0 auto;">
            <div style="padding-left: 8px; padding-top: 0px; float: right;">
                <!-- REPLACE THIS BLOCK OF SCRIPT WITH YOUR OWN FIREFOX REFERRAL BUTTON SCRIPT -->
                <script type="text/javascript"><!--
                google_ad_client = "pub-2386209491833952";
                google_ad_width = 125;
                google_ad_height = 125;
                google_ad_format = "125x125_as_rimg";
                google_cpa_choice = "CAAQo-aZzgEaCM3CU97Siy5UKK2293M";
                //--></script>
                <script type="text/javascript" src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
                </script>
            </div>
            <strong>We see you're using Internet Explorer.&nbsp;&nbsp;Try Firefox, you'll like it better.</strong>  
            <br /><br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>&middot;</strong> Firefox blocks pop-up windows.
            <br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>&middot;</strong> It stops viruses and spyware.
            <br />
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>&middot;</strong> It keeps Microsoft from controlling the future of the internet.
            <br /><br />
            Click the button on the right to download Firefox.&nbsp;&nbsp;It's free.
        </div>

    </div>
</div> 
<?php
        echo'
        <div id="pbody">
    		';
    echo"
            <div id=\"header\">
                <img src=\"{$ptr}i/bison.jpg\" alt=\"American Bison from the back of the five cent piece\"/>
                <h1>The Bitter Buffalo</h1>
                <div class=\"cleary\">&nbsp;</div>
            </div>
        		<div id=\"sidebar\">
    					<ul>
    						<li><a href=\"{$ptr}\">Home</a></li>
						<li><a href=\"{$ptr}sudoku/\">Sudoku</a></li>
                                                <li><a href=\"{$ptr}ssh/\">SSH</a></li>
    						<li><a href=\"{$ptr}gallery/\">Photo Gallery</a></li>
    						<li><a href=\"http://trac.cameronpalmer.com/\">Trac Laboratory</a></li>
    						<li><a href=\"{$ptr}wiki/\">Course Wiki</a></li>
     						<li><a href=\"{$ptr}resume/\">R&eacute;sum&eacute;</a></li> 			    			    			    			
     					</ul>
                                        <div class=\"cleary\">&nbsp;</div>
    				</div>
            <div id=\"content\">
                <h2>{$section}</h2>
        ";
}

function do_content($buf) {
  $x=$buf;
  
  return $buf;
}

function do_footer($buf) {
		echo"
					  
    		";
    return $buf.'
    				</div>
            <div id="footer">
                Copyright &copy; 2005 Cameron Palmer<br />
                cameron DOT palmer AT gmail DOT com
            </div>
        </div>
    </body>
</html>
';

}

?>