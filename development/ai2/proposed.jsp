<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
    <head>
        <title>IBM Academic Initiative</title>
        <!-- ibm.com V14 OneX css -->
        <link rel="stylesheet" type="text/css" href="c/main.css" />
        <link rel="stylesheet" type="text/css" media="all" href="c/screen.css" />
        <link rel="stylesheet" type="text/css" media="screen,print" href="c/table.css" />
        <link rel="stylesheet" type="text/css" media="print" href="c/print.css" />
        <link rel="shortcut icon" href="i/favicon.ico" type="image/x-icon" />
        <link rel="alternate" title="IBM Academic Initiative RSS" href="ibm_ai_rss.jsp" type="application/rss+xml" />
    </head>
    
    <body>
        <!-- BEGIN Masthead File -->
        <table width="760" cellspacing="0" cellpadding="0" border="0">
            <tr valign="top">
                <td width="110" class="bbg">
                    <a href="http://www.ibm.com">
                        <img width="110" src="i/ibm-logo.gif" height="52" border="0" alt="IBM"/>
                    </a>
                </td>
                <td width="650" class="mbbg" align="right">
                    <table align="right" cellspacing="0" cellpadding="0" border="0">
                        <tr class="cty-tou">
                            <td class="upper-masthead-corner" width="17" rowspan="2">
                                <a href="#main"><img alt="Skip to main content" height="1" width="1" border="0" src="i/c.gif"/></a>
                            </td>
                            <td align="left">
                                <table align="left" cellspacing="0" cellpadding="0" border="0">
                                    <tr>
                                        <td>
                                            <span class="spacer">&nbsp;</span>
                                            <b class="country">Country/region</b>
                                            <span class="spacer">&nbsp;[</span>
                                            <a href="http://www.ibm.com/planetwide/select/selector.html" class="ur-link">select</a>
                                                <span class="spacer">]</span>
                                        </td>
                                        <td class="upper-masthead-divider" width="29">&nbsp;</td>
                                        <td align="left">
                                            <a href="http://www.ibm.com/legal/" class="ur-link">Terms of use</a>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td width="40">&nbsp;</td>
                        </tr>
                        <tr>
                            <td colspan="2" height="1" class="cty-tou-border">
                                <img src="i/c.gif" width="1" height="1" alt="" border="0" />
                            </td>
                        </tr>
                        <tr>
                            <td colspan="3">
                                <img src="i/c.gif" width="1" height="8" alt="" border="0"/>
                            </td>
                        </tr>
                        <tr>
                            <td>&nbsp;</td>
                            <td colspan="2" align="center">
                                <!-- BEGIN SP Search ACMEII Code -->
                                <form action="/jct09002c/university/scholars/SearchResults" name="Search" method="get">
                                    <table cellspacing="0" cellpadding="0" border="0">
                                        <tr> 
                                            <td width="18">
                                                <label for="scope"><img src="i/c.gif" width="1" height="1" alt="Search in:"/></label>
                                            </td> 
                                            <td>
                                                <select id="scope" name="scope" class="input-local" size="1">
                                                    <option value="sp">Academic Initiative</option>
                                                    <option value="ibm">ibm.com</option>
                                                </select>
                                            </td> 
                                            <td width="7">
                                                <label for="query"><img src="i/c.gif" width="1" height="1" alt="Search for:"/></label>
                                            </td>
                                            <td align="right"><input name="query" type="text" value="" id="query" size="15" maxlength="100" class="input" /></td> 
                                            <td width="7">
                                                <input type="hidden" name="v" value="14" />
                                                <input type="hidden" name="lang" value="en"/>
                                                <input type="hidden" name="cc" value="us"/>
                                                <input type="hidden" name="type" value="0" />
                                            </td>
                                            <td align="left" width="90">
                                                <input alt="Search" name="Search" src="i/search.gif" type="image" value="Search" />
                                            </td> 
                                        </tr> 
                                    </table>
                                </form>
                                <!-- END SP Search ACMEII Code -->
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr>
                <td colspan="2" class="blbg">
                    <!-- BEGIN Top Navbar -->
                    <table width="760" cellspacing="0" cellpadding="0" border="0">
                        <tr>
                            <td>
                                <table cellspacing="0" cellpadding="0" border="0">
                                    <tr>
                                        <td><span class="spacer">&nbsp;</span></td>
                                        <td><a href="http://www.ibm.com/" class="masthead-mainlink">Home</a></td>
                                        <td width="27" class="masthead-divider">&nbsp;</td>
                                        <td><a href="http://www.ibm.com/products/" class="masthead-mainlink">Products</a></td>
                                        <td width="27" class="masthead-divider">&nbsp;</td>
                                        <td><a href="http://www.ibm.com/servicessolutions/" class="masthead-mainlink">Services &amp; solutions</a></td>
                                        <td width="27" class="masthead-divider">&nbsp;</td>
                                        <td><a href="http://www.ibm.com/support/" class="masthead-mainlink">Support &amp; downloads</a></td>
                                        <td width="27" class="masthead-divider">&nbsp;</td>
                                        <td><a href="http://www.ibm.com/account/" class="masthead-mainlink">My account</a></td>
                                        <td><span class="spacer">&nbsp;</span></td>
                                    </tr>
                                </table>
                            </td>
                        </tr>
                    </table>
                    <!-- END Top Navbar -->
                </td>
            </tr>
        </table>
        <!-- END Masthead File -->

        <!-- BEGIN Main page table -->
        <table width="760" border="0" cellspacing="0" cellpadding="0" id="v14-body-table">
            <tr valign="top">
                <td width="150" id="navigation">
                    <table border="0" cellpadding="0" cellspacing="0" width="150">
                        <tr>
                            <td class="left-nav-spacer">&nbsp;</td>
                        </tr>
                    </table>
                    
                    <!-- BEGIN Left Navigation Bar -->
                    <table width="150" border="0" cellspacing="0" cellpadding="0">
                        <tr>
                            <td class="left-nav-overview" colspan="2">
                                <a class="left-nav-overview" href="/jct09002c/us/en/university/scholars/">IBM Academic Initiative</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/products/">Products &amp; technologies</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/news/">News &amp; events</a>
                            </td>
                        </tr>

                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/downloads/">Download software</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/courseware/">Download courseware</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/training/classroom.html">Training &amp; Certification</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/resources/">Forums &amp; community</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/support/">Support</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/members/">Membership</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/feedback/">Feedback</a>
                            </td>
                        </tr>
                        <tr class="left-nav-last">
                            <td width="14">
                                <img src="i/c.gif" width="14" height="1" alt="" class="display-img" />
                            </td>
                            <td width="136">
                                <img src="i/left-nav-corner.gif" width="136" height="19" alt="" class="display-img" />
                            </td>
                        </tr>
                    </table>
                    <!-- END Left Navigation Bar -->
                    <br />
                    
                    <!-- BEGIN Related Links Section -->
                    <table border="0" cellpadding="0" cellspacing="0" width="150">
                        <tr>
                            <td colspan="2" class="related">
                                <b class="related">Related links</b>
                            </td>
                        </tr>
                        <tr class="rlinks">
                            <td>
                                <img src="i/rl-bullet.gif" width="2" height="8" alt="" border="0" />
                            </td>
                            <td>
                                <a href="/jct09002c/us/en/university/students/" class="rlinks">Student Portal</a>
                            </td>
                        </tr>
                        <tr class="rlinks">
                            <td>
                                <img src="i/rl-bullet.gif" width="2" height="8" alt="" border="0" />
                            </td>
                            <td>
                                <a href="http://www.ibm.com/industries/education/doc/jsp/indseg/highereducation/" class="rlinks">Solutions for higher education</a>
                            </td>
                        </tr>
                        <tr class="rlinks">
                            <td>
                                <img src="i/rl-bullet.gif" width="2" height="8" alt="" border="0" />
                            </td>
                            <td>
                                <a href="http://www.ibm.com/developerworks/" class="rlinks">developerWorks</a>
                            </td>
                        </tr>
                        <tr class="rlinks">
                            <td>
                                <img src="i/rl-bullet.gif" width="2" height="8" alt="" border="0"/>
                            </td>
                            <td>
                                <a href="http://www.ibm.com/alphaworks" class="rlinks">alphaWorks</a>
                            </td>
                        </tr>
                        <!-- BEGIN Academic Initiative Localized Language Sites -->
                        <tr>
                            <td colspan="2" class="related">
                                <b class="related">Regional Sites</b>
                            </td>
                        </tr>

                        <tr class="rlinks">
                            <td>
                                <img src="i/rl-bullet.gif" width="2" height="8" alt="" border="0"/>
                            </td>
                            <td>
                                <a href="/jct09002c/university/scholars/it/index.html" class="rlinks">IBM per l'Universit&agrave;<br />Italia</a>
                            </td>
                        </tr>
                        <tr class="rlinks">
                            <td>
                                <img src="i/rl-bullet.gif" width="2" height="8" alt="" border="0"/>
                            </td>
                            <td>
                                <a href="/jct09002c/university/scholars/pt/index.html" class="rlinks">IBM para as Universidades<br />Portugu&ecirc;s</a>
                            </td>
                        </tr>
                        <tr class="rlinks">
                            <td>
                                    <img src="i/rl-bullet.gif" width="2" height="8" alt="" border="0"/>
                            </td>
                            <td>
                                <a href="//www.ibm.com/de/ibm/unternehmen/university_relations/index.html" class="rlinks">IBM Wissenschafts<br />beziehungen in Deutschland</a>
                            </td>
                        </tr>
                        <tr class="rlinks">
                            <td>
                                <img src="i/rl-bullet.gif" width="2" height="8" alt="" border="0"/>
                            </td>
                            <td>
                                <a href="//www.ibm.com/cn/ai/" class="rlinks">IBM Academic Initiative<br />China</a>
                            </td>
                        </tr>
                        <tr class="rlinks">
                            <td>
                                <img src="i/rl-bullet.gif" width="2" height="8" alt="" border="0"/>
                            </td>
                            <td>
                                <a href="//www.ibm.com/ru/software/info/students/" class="rlinks">IBM Academic Initiative<br />Программа</a>
                            </td>
                        </tr>
                        <tr class="rlinks">
                            <td>
                                <img src="i/rl-bullet.gif" width="2" height="8" alt="" border="0" />
                            </td>
                            <td>
                                <a href="//www.ibm.com/in/university/" class="rlinks">IBM Academic Initiative<br />India</a>
                            </td>
                        </tr>
                    <!-- END Academic Initiative Localized Language Sites -->
                        <tr>
                            <td width="14">
                                <img src="i/c.gif" width="14" height="1" alt="" class="display-img" />
                            </td>
                            <td width="136">
                                <img src="i/c.gif" width="136" height="19" alt="" class="display-img" />
                            </td>
                        </tr>
                    </table>
                    <!-- END Related Links Section -->
                </td>    
                <td width="610" valign="top">
                    <!--BEGIN CONTENT BODY-->			
                    <a name="main"></a>
                    <table width="610" cellpadding="0" cellspacing="0" border="0" id="content-table">
                        <tr valign="top">    
                            <td width="10"></td>
                            <td width="443">
                                <img alt="" height="6" src="i/c.gif" width="1" />
                                <h1>IBM Academic Initiative</h1>
                                <p id="subtitle"><em>Open standards, open source and IBM resources for academia</em></p>
                            </td>
                            <td width="7"></td>
                            <td width="150"></td>
                        </tr>
                        <tr>
                            <td width="10"></td>
                            <td valign="top" width="443" id="content">
                                <!-- BEGIN Introduction Text -->
                                <table width="443" cellpadding="0" cellspacing="0" border="0">
                                    <tr>
                                        <td colspan="2">
                                            <img src="i/c.gif" width="1" height="4" alt="" border="0" />
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="2">
                                            <div style="background-image: url(i/image_text2.jpg); background-repeat: no-repeat; height: 100px; width: 443px;">
                                                <div style="font-size: 200%; padding: 1em 0 0 1em; color: #fff; width: 60%; float: left;">Testimonials</div>
                                                <div style="padding: 1em 1em 0 0; color: #fff; width: 30%; float: right;">Find out why Chase Bank thinks that 
                                                IBM course materials are so beneficial to students and faculty. <a href="#">Flash Video</a> <a href="#">Transcript</a></div>
                                            </div>
                                        </td>
                                    </tr>
                                </table>
                                <!-- END Introduction Text -->
                                <br />
                                
                                <!-- BEGIN Top Stories -->
                                <sql:query var="rs" dataSource="jdbc/IBMDB">
                                    SELECT * FROM news
                                        WHERE (publish IS TRUE) AND (start_date <= CURDATE()) AND (end_date >= CURDATE())
                                </sql:query>

                                <table border="0" cellpadding="0" cellspacing="0" width="443">
                                    <tr>
                                        <td>
                                            <div style="border: solid; border-width: 1px; border-color: #ccc;">
                                                <div style="color:#fff; font-weight: bold; background:#3c5f84; padding: .30em 0 .30em .5em;">Top Stories</div>
                                                <ul style="padding: .6em .6em .6em 2.5em; list-style-image: url(i/fw_bold.gif);">
                                                <c:forEach var="row" items="${rs.rows}">
                                                    <li style="padding-bottom: .3em;"><b><a class="fbox" href="${row.link}">${row.headline}</a></b>.</li>
                                                </c:forEach>
                                                </ul>
                                            </div>
                                        </td>
                                    </tr>
                                </table>
                                <!-- END Top Stories -->
                                <br />

                                <sql:query var="rs" dataSource="jdbc/IBMDB">
                                    SELECT * FROM marketing
                                        WHERE (publish IS TRUE) ORDER BY RAND() limit 1
                                </sql:query>
                                
                                <c:forEach var="row" items="${rs.rows}">
                                <div style="border: solid; border-width: 1px; border-color: #ccc;">
                                    <div style="color:#fff; font-weight: bold; background:#3c5f84; padding: .30em 0 .30em .5em;">What is...</div>
                                    <div style="padding: .60em;">
                                        
                                        <h1>${row.prod_name}</h1>
                                        <a href="" class="smallplainlink">
                                        <img src="" border="0" width="137" height="21" alt="" align="middle"  /></a>
                                        <p>${row.prod_desc}</p>
                                        <div style="padding-left: 3em;">
                                            <a href="${row.cwlink}" style="text-decoration: none; color: #3e6186;">
                                            <img border="0" alt="Courseware Download" style="text-decoration: none" src="i/download.gif" width="21" height="21" />
                                            Courseware download</a>
                                            <a href="${row.swlink}" style="text-decoration: none; color: #3e6186;">
                                            <img border="0" alt="Software Download" style="text-decoration: none; padding-left: 1.5em;" src="i/download.gif" width="21" height="21" />
                                            Software download</a>
                                        </div>
                                    </div>
                                </div>
                                </c:forEach>
                                <br style="clear: both;" />                                
                            </td>
                            <td width="7">
                                <img src="i/c.gif" width="7" height="1" alt="" border="0" />
                            </td>
                            <td width="150" valign="top" id="right-nav">
                                <table cellspacing="0" cellpadding="0" border="0" width="150">
                                    <tr>
                                        <td class="v14-header-1-small">Membership</td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <div style="border: solid; border-width: 1px; border-color: #ccc;">
                                            <ul style="padding: .6em .6em .6em 2.5em; list-style-image: url(i/fw.gif);">
                                                <li style="padding-bottom: .3em;"><a class="w"  href="/jct09002c/university/scholars/members/spm/redirector.htm">Sign in</a></li>
                                                <li style="padding-bottom: .3em;"><a class="w"  href="/jct09002c/us/en/university/scholars/members/registration.html">Apply now</a> to become a member at no charge.</li>
                                                <li style="padding-bottom: .3em;"><a class="w" href="https://www.ibm.com/account/profile/us?page=forgot">Reset</a> your password or <a class="w" href="https://www.ibm.com/account/profile/us">update</a> your profile.</li>
                                            </ul>
                                            </div>
                                        </td>
                                    </tr>
                                </table>
                                <br />
                                
                                <!-- BEGIN Quick Links Section -->
                                <table border="0" cellpadding="0" cellspacing="0" width="150">
                                    <tr> 
                                        <td class="v14-header-4-small">Quick links</td>
                                    </tr>
                                    <tr>
                                        <td class="no-padding">
                                            <div style="border: solid; border-width: 0 1px 1px 1px; border-color: #ccc;">
                                            <ul style="padding: .6em .6em .6em 2.5em; list-style-image: url(i/fw.gif)">
                                                <li style="padding-bottom: .3em;"><a class="smallplainlink" href="/jct09002c/university/scholars/selfhelp/index.html">Self help</a></li>
                                                <li style="padding-bottom: .3em;"><a class="smallplainlink" href="/jct09002c/us/en/university/scholars/downloads/software.html">Download software</a></li>
                                                <li style="padding-bottom: .3em;"><a class="smallplainlink" href="/jct09002c/us/en/university/scholars/courseware/">Download courseware</a></li>
                                                <li style="padding-bottom: .3em;"><a class="smallplainlink" href="https://www14.software.ibm.com/webapp/iwm/web/preLogin.do?source=acadisdc">Give students access to software</a></li>
                                                <li style="padding-bottom: .3em;"><a class="smallplainlink" href="//jct09002c/us/en/university/students/opptysystem/index.html">Student Opportunity System</a></li>
                                                <li style="padding-bottom: .3em;"><a class="smallplainlink" href="http://www.ibm.com/developerworks/offers/techbriefings/">developerWorks! Live Technical Briefings</a></li>
                                                <li style="padding-bottom: .3em;"><a class="smallplainlink" href="//jct09002c/university/scholars/highschool/index.html">Resources for High School Students</a></li>
                                            </ul>
                                            </div>
                                        </td>
                                    </tr>
                                </table>
                                <!-- END Quick Links Section -->
                                <br />
                                    
                                <!-- BEGIN Latin American Grid -->
                                <table border="0" cellpadding="0" cellspacing="0" width="150">
                                    <tr>
                                        <td class="v14-header-4-small">Latin American (LA) Grid</td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border">
                                                <tr>
                                                    <td width="150">
                                                        <img src="i/lagridbusinessplan_callout.gif" height="70" width="148" alt=""/>
                                                        <p class="small">Latin American Innovation and Leadership Today</p>
                                                        <blockquote><a class="smallplainlink" href="http://www.ibm.com/grid/lagrid.shtml?ca=grid&amp;me=w&amp;met=lagrid&amp;p_site=gridhp">Learn more</a></blockquote>
                                                    </td>
                                                  </tr>
                                            </table>
                                        </td>
                                    </tr>
                                </table>
                                <!-- END Latin American Grid -->
                                <!--END CONTENT BODY-->
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
        <!-- BEGIN Page Footer -->
        <table width="760" cellspacing="0" cellpadding="0" border="0">
            <tr>
                <td height="19" class="bbg">
                    <table cellspacing="0" cellpadding="0" border="0">
                        <tr>
                            <td><span class="spacer">&nbsp;</span><a href="http://www.ibm.com/ibm/" class="mainlink">About IBM</a></td>
                            <td width="27" class="footer-divider">&nbsp;</td>
                            <td><a href="http://www.ibm.com/privacy/" class="mainlink">Privacy</a></td>
                            <td width="27" class="footer-divider">&nbsp;</td>
                            <td><a href="http://www.ibm.com/contact/" class="mainlink">Contact</a></td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
        <!-- END Page Footer -->
        
        <!-- BEGIN surfaid code include -->
        <script type="text/javascript" language="JavaScript1.2" src="//www.ibm.com/common/stats/stats.js"></script>
        <!-- END surfaid code include -->
    </body>
</html>
