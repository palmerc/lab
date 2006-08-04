<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
    <head>
        <title>IBM Academic Initiative</title>
        <!-- ibm.com V14 OneX css -->
        <link rel="stylesheet" type="text/css" href="c/main.css" />
        <link rel="stylesheet" type="text/css" media="all" href="c/screen.css" />
        <link rel="stylesheet" type="text/css" media="screen,print" href="c/table.css" />
        <link rel="stylesheet" type="text/css" media="print" href="c/print.css" />
        <link rel="shortcut icon" href="i/favicon.ico" type="image/x-icon" />
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
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/downloads/">Downloads &amp; CDs</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/training/classroom.html">Training</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/courseware/">Curriculum &amp; courseware</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/resources/">Forums &amp; community</a>
                            </td>
                        </tr>
                        <tr>    
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/skills/">Skills for the 21st century</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/certification/">Certification</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/library/">Library</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/support/">Support</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/news/">News &amp; events</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/members/">Membership</a>
                            </td>
                        </tr>
                        <tr>
                            <td class="left-nav" colspan="2">
                                <a class="left-nav" href="/jct09002c/us/en/university/scholars/sitemap/index.html">Site map</a>
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
                    
                    <table border="0" cellpadding="0" cellspacing="0" width="150">
                        <tr>
                            <td colspan="2" class="related">
                            </td>
                        </tr>
                        <tr>
                            <td width="14">
                                <img src="i/c.gif" width="14" height="1" alt="" class="display-img"/>
                            </td>
                            <td width="136">
                                <img src="i/c.gif" width="136" height="19" alt="" class="display-img"/>
                            </td>
                        </tr>   
                    </table>
                    <br />
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
                                            <img src="i/c.gif" width="1" height="4" alt="" border="0" />
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="2">
                                            <img src="i/image_text2.jpg" width="443" height="160" alt="IBM Academic Initiative" border="0" />
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="2">
                                            <img src="i/c.gif" width="1" height="4" alt="" border="0" />
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="2">
                                            <img src="i/dblue_rule.gif" width="443" height="4" alt="" border="0" />
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border">
                                                <tr>
                                                    <td width="443">
                                                        <p>Would you like to...</p>
                                                        <ul>
                                                            <li>Stay on top of the leading technologies?</li>
                                                            <li>Enjoy the benefits of the open source community?</li>
                                                            <li>Prepare your students to land the best jobs in the hottest fields?</li>
                                                            <li>And, get valuable resources to help reach these goals, at no charge?</li>
                                                        </ul>
                                                        <p>The IBM Academic Initiative offers all this and more.  Become a member and gain access to software,
                                                        hardware, courseware, training, tools, books, and lots of discounts.  What are you waiting for?
                                                        <a href="/jct09002c/university/scholars/members/"> Start here.</a></p>
                                                    </td> 
                                                </tr>
                                            </table>
                                        </td> 
                                    </tr>
                                </table>
                                <!-- END Introduction Text -->
                                <br />
                                
                                <!-- BEGIN Top Stories -->
                                <table border="0" cellpadding="0" cellspacing="0" width="443">
                                    <tr>
                                        <td class="v14-header-1">Top stories</td>
                                    </tr>
                                    <tr>
                                    <sql:query var="rs" dataSource="jdbc/IBMDB">
                                        SELECT * FROM news
                                            WHERE (publish IS TRUE) AND (start_date <= CURDATE()) AND (end_date >= CURDATE())
                                    </sql:query>

                                        <td>
                                            <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border">
                                                <tr>
                                                    <td width="443">
                                                        <table border="0" cellpadding="0" cellspacing="0" width="430">
                                                        <c:forEach var="row" items="${rs.rows}">
                                                            <tr valign="top">
                                                                <td align="right" width="18" class="ipt"><img alt="" height="16" src="//www.ibm.com/i/v14/icons/fw_bold.gif" width="16"/></td>
                                                                <td class="nlbp">
                                                                    <p><b><a class="fbox" href="${row.link}">${row.headline}</a></b>
                                                                    ${row.summary}</p>
                                                                </td>			
                                                            </tr>
                                                        </c:forEach>
                                                            <tr>
                                                                <td colspan="2"><img src="i/c.gif" border="0" height="5" width="1" alt=""/></td>
                                                            </tr>
                                                            <tr>
                                                                <td colspan="2"><img src="i/c.gif" border="0" height="5" width="1" alt=""/></td>
                                                            </tr>
                                                        </table>
                                                    </td>
                                                </tr>
                                            </table>
                                        </td>
                                    </tr>
                                </table>
                                <!-- END Top Stories -->
                                <br />

                                <table border="0" cellpadding="0" cellspacing="0" width="443">
                                    <tr>
                                        <td width="218" valign="top">
                                            <img alt="" height="1" src="i/gray_rule.gif" width="218" />
                                            <br />
                                            
                                            <!-- BEGIN Open Standards Section -->
                                            <table border="0" cellpadding="0" cellspacing="0" width="218" class="v14-gray-table-border"> 
                                                <tr>
                                                    <td width="1" colspan="3">
                                                        <img src="i/c.gif" width="1" height="3" alt="" border="0" />
                                                    </td>
                                                </tr>
                                                <tr> 
                                                    <td width="3" valign="top">
                                                        <img src="i/c.gif" width="3" height="1" alt="" border="0" />
                                                    </td>
                                                    <td width="50" valign="top">
                                                        <img src="i/AI_portlet_openstandard.jpg" alt="Open standards" width="50" height="100" />
                                                    </td>
                                                    <td width="165" height="120" valign="top">
                                                        <p><b><a href="/jct09002c/us/en/university/scholars/openstandards/">Open standards</a></b>
                                                        <br />
                                                        Here, we've gathered an extensive list of the best resources on 
                                                        our sites for learning about open-standards computing.</p>
                                                    </td>
                                                </tr>
                                            </table>
                                            <!-- END Open Standards Section -->
                                        </td>
                                        <td width="7">
                                            <br />
                                        </td>
                                        <td width="218" valign="top">
                                            <img alt="" height="1" src="i/gray_rule.gif" width="218" />
                                            <br />
                                            
                                            <!-- BEGIN Student Opportunity Section -->
                                            <table border="0" cellpadding="0" cellspacing="0" width="218" class="v14-gray-table-border"> 
                                                <tr>
                                                    <td width="1" colspan="3">
                                                        <img src="i/c.gif" width="1" height="3" alt="" border="0" />
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td width="3" valign="top">
                                                        <img src="i/c.gif" width="3" height="1" alt="" border="0" />
                                                    </td>
                                                    <td width="50" valign="top">
                                                        <img src="i/AI_portlet_studentopp.jpg" alt="Student Opportunity System" width="50" height="100" />
                                                    </td>
                                                    <td width="165" height="120" valign="top">
                                                        <p><b><a href="/jct09002c/us/en/university/students/opptysystem/index.html">IBM Academic Initiative Student Opportunity System</a></b>
                                                        <br />
                                                        Let your students know about our new resume database!</p>
                                                    </td>
                                                    </tr>
                                            </table>
                                            <!-- END Student Opportunity Section -->
                                        </td>
                                    </tr>
                                </table>
                                <br />
                            
                                <table border="0" cellpadding="0" cellspacing="0" width="443">
                                    <tr> 
                                        <td width="218" valign="top">
                                            <img alt="" height="1" src="i/gray_rule.gif" width="218" />
                                            <br />
                                            
                                            <!-- BEGIN Getting Started Section -->
                                            <table border="0" cellpadding="0" cellspacing="0" width="218" class="v14-gray-table-border"> 
                                                <tr>
                                                    <td width="1" colspan="3">
                                                        <img src="i/c.gif" width="1" height="3" alt="" border="0" />
                                                    </td>
                                                </tr>
                                                <tr> 
                                                    <td width="3" valign="top">
                                                        <img src="i/c.gif" width="3" height="1" alt="" border="0" />
                                                    </td>
                                                    <td width="50" valign="top">
                                                        <img src="i/getstarted.jpg" alt="Getting Started" width="50" height="100" />
                                                    </td>
                                                    <td width="165" height="120" valign="top">
                                                        <p><b><a href="/jct09002c/us/en/university/scholars/products/">Getting started</a></b>
                                                        <br />
                                                        If you're new to the IBM Academic Initiative, we've developed some resources to help you get started quickly.</p>
                                                    </td>
                                                </tr>
                                            </table>
                                            <!-- END Getting Started Section -->
                                        </td>
                                        <td width="7">
                                            <br />
                                        </td>
                                        <td width="218" valign="top">
                                            <img alt="" height="1" src="i/gray_rule.gif" width="218" />
                                            <br />
                                            
                                            <!-- BEGIN High School Section -->
                                            <table border="0" cellpadding="0" cellspacing="0" width="218" class="v14-gray-table-border"> 
                                                <tr>
                                                    <td width="1" colspan="3">
                                                        <img src="i/c.gif" width="1" height="3" alt="" border="0" />
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td width="3" valign="top">
                                                        <img src="i/c.gif" width="3" height="1" alt="" border="0" />
                                                    </td>
                                                    <td width="50" valign="top">
                                                        <img src="i/HighSchool_50x100.jpg" alt="Resources for high schools." width="50" height="100" />
                                                    </td>
                                                    <td width="165" height="120" valign="top">
                                                        <p><b><a href="/jct09002c/university/scholars/highschool/index.html">Resources for high schools</a></b>
                                                        <br />
                                                        The IBM Academic Initiative has resources for high schools too!</p>
                                                    </td>
                                                </tr>
                                            </table>
                                            <!-- END High School Section -->
                                        </td>
                                    </tr>
                                </table>
                                <br />
                            
                                <table border="0" cellpadding="0" cellspacing="0" width="443">
                                    <tr> 
                                        <td width="218" valign="top">
                                            <img alt="" height="1" src="i/gray_rule.gif" width="218" />
                                            <br />
                                            <!-- BEGIN developerWorks Section -->
                                            <table border="0" cellpadding="0" cellspacing="0" width="218" class="v14-gray-table-border"> 
                                                <tr>
                                                    <td width="1" colspan="3">
                                                        <img src="i/c.gif" width="1" height="3" alt="" border="0" />
                                                    </td>
                                                </tr>
                                                <tr> 
                                                    <td width="3" valign="top">
                                                        <img src="i/c.gif" width="3" height="1" alt="" border="0" />
                                                    </td>
                                                    <td width="50" valign="top">
                                                        <img src="i/devworks.jpg" alt="Developer Works" width="50" height="100" />
                                                    </td>
                                                    <td width="165" height="120" valign="top">
                                                        <p><b><a href="http://www.ibm.com/developerworks/offers/techbriefings/">developerWorks Live! technical briefings</a></b>
                                                        <br />
                                                        Attend these no-charge technical demonstrations and enjoy direct contact with leading technology experts.</p>
                                                    </td>
                                                </tr>
                                            </table>
                                            <!-- END developerWorks Section -->
                                        </td>

                                        <td width="7">
                                            <br />
                                        </td>
                                        <td width="218" valign="top">
                                            <img alt="" height="1" src="i/gray_rule.gif" width="218" />
                                            <br />
                                            <!-- BEGIN Certification Tests Section -->
                                            <table border="0" cellpadding="0" cellspacing="0" width="218" class="v14-gray-table-border"> 
                                                <tr>
                                                    <td width="1" colspan="3"><img src="i/c.gif" width="1" height="3" alt="" border="0" /></td>
                                                </tr>
                                                <tr> 
                                                    <td width="3" valign="top">
                                                        <img src="i/c.gif" width="3" height="1" alt="" border="0" />
                                                    </td>
                                                    <td width="50" valign="top">
                                                        <img src="i/certification.jpg" alt="Certification" width="50" height="100" />
                                                    </td>
                                                    <td width="165" height="120" valign="top">
                                                        <p><b><a href="/jct09002c/us/en/university/scholars/certification/#b">Certification test discounts</a></b><br />
                                                        Faculty members and their students can receive a 50% discount on certification tests at Thompson Prometric Centers.</p>
                                                    </td>
                                                </tr>
                                            </table>
                                            <!-- END Certification Tests Section -->
                                        </td>
                                    </tr>
                                </table>
                                <img alt="" height="40" src="i/c.gif" width="1" />
                            </td>
                            <td width="7">
                                <img src="i/c.gif" width="7" height="1" alt="" border="0" />
                            </td>
                            <td width="150" valign="top" id="right-nav">
                                <table cellspacing="0" cellpadding="0" border="0" width="150">
                                    <tr>
                                        <td class="v14-header-1-small">Membership</td>
                                    </tr>
                                </table>        
                                <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border" width="150">
                                    <tr>
                                        <td  valign="top"  class="ipt" width="17">
                                            <img src="i/fw.gif" height="16" width="16" alt=""/>
                                        </td>
                                        <td  class="npl">
                                            <p class="small"><a class="w"  href="/jct09002c/university/scholars/members/spm/redirector.htm">Sign in</a></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="4" class="dotted"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                    </tr>
                                    <tr>
                                        <td  valign="top" class="ipt" width="17">
                                            <img src="i/fw.gif" height="16" width="16" alt="" />
                                        </td>
                                        <td  class="npl">
                                            <p class="small"><a class="w"  href="/jct09002c/us/en/university/scholars/members/registration.html">Apply now</a> to become a member at no charge.</p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="3" class="dotted"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                    </tr>
                                    <tr>
                                        <td  valign="top"  class="ipt" width="17"><img src="i/fw.gif" height="16" width="16" alt=""/></td>
                                        <td  class="npl"><p class="small"><a class="w" href="https://www.ibm.com/account/profile/us?page=forgot">Reset</a> your password or <a class="w" href="https://www.ibm.com/account/profile/us">update</a> your profile.</p></td>
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
                                            <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border" width="150"> 
                                                <tr>
                                                    <td  valign="top"  class="ipt" width="17"><img src="i/fw.gif" height="16" width="16" alt=""/></td>
                                                    <td  class="npl"><p><a class="smallplainlink" href="/jct09002c/university/scholars/selfhelp/index.html">Self help</a></p></td>		
                                                </tr>
                                                <tr> 
                                                    <td class="dotted" colspan="2"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>
                                                <tr> 
                                                    <td valign="top"  class="ipt" width="17"><img src="i/fw.gif" height="16" width="16" alt=""/></td>
                                                    <td  class="npl"><p><a class="smallplainlink" href="/jct09002c/us/en/university/scholars/downloads/software.html">Download software</a></p></td>		
                                                </tr>
                                                <tr> 
                                                    <td class="dotted" colspan="2"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>
                                                <tr>
                                                    <td  valign="top"  class="ipt" width="17"><img src="i/fw.gif" height="16" width="16" alt=""/></td>
                                                    <td  class="npl"><p><a class="smallplainlink" href="/jct09002c/us/en/university/scholars/courseware/">Download courseware</a></p></td>		
                                                </tr>
                                                <tr> 
                                                    <td class="dotted" colspan="2"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>
                                                <tr>
                                                    <td  valign="top"  class="ipt" width="17"><img src="i/fw.gif" height="16" width="16" alt=""/></td>
                                                    <td  class="npl"><p><a class="smallplainlink" href="https://www14.software.ibm.com/webapp/iwm/web/preLogin.do?source=acadisdc">
                                                    Give students access <br />to software</a></p></td>		
                                                </tr>
                                            </table>
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
                                                    <td width="150"><img src="i/lagridbusinessplan_callout.gif" height="70" width="148" alt=""/></td>
                                                </tr>
                                                <tr>
                                                    <td><p class="small">Hispanic Innovation and Leadership Today</p></td>
                                                </tr>
                                                <tr>
                                                    <td class="dotted"><img src="i/c.gif" height="1" width="1" alt=""/></td>
                                                </tr>
                                                <tr>
                                                    <td class="no-padding">
                                                        <table border="0" cellpadding="0" cellspacing="0" width="143">
                                                            <tr valign="top">
                                                                <td align="right" width="18" class="ipt"><img src="i/fw.gif" height="16" width="16" alt=""/></td>
                                                                <td width="125" class="npl"><p><a class="smallplainlink" href="http://www.ibm.com/grid/lagrid.shtml?ca=grid&amp;me=w&amp;met=lagrid&amp;p_site=gridhp">Learn more</a></p></td>
                                                            </tr>
                                                        </table>
                                                    </td>
                                                </tr>
                                            </table>
                                        </td>
                                    </tr>
                                </table>
                                <!-- END Latin American Grid -->
                                <br />

                                <table border="0" cellpadding="0" cellspacing="0" width="150">
                                    <tr> 
                                        <td class="v14-header-4-small">Products &amp; technologies</td> 
                                    </tr> 
                                    <tr>
                                        <td>
                                            <!-- BEGIN Brand Logo Section -->
                                            <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border"> 
                                                <tr> 
                                                    <td width="150"><p><a href="/jct09002c/us/en/university/scholars/products/data/" class="smallplainlink">
                                                        <img src="i/db2_markhome.gif" border="0" alt="DB2 Information Management Software" align="left" width="140" height="21" /></a></p></td>
                                                </tr> 
                                                <tr> 
                                                    <td class="dotted" colspan="2"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>
                                                <tr> 
                                                    <td width="150"><p><a href="/jct09002c/us/en/university/scholars/products/iseries/index.html" class="smallplainlink">
                                                        <img src="i/elogo.gif" border="0" alt="eServer" align="bottom" width="53" height="10" />  iSeries</a></p></td> 
                                                </tr>
                                                <tr>
                                                    <td class="dotted" colspan="2"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>
                                                <tr> 
                                                    <td width="150"><p><a href="/jct09002c/us/en/university/scholars/products/zseries/" class="smallplainlink">
                                                    <img src="i/elogo.gif" border="0" alt="eServer" align="bottom" width="53" height="10" />  zSeries</a></p></td> 
                                                </tr> 	
                                                <tr> 
                                                    <td class="dotted" colspan="2"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>
                                                <tr> 
                                                    <td width="150"><p><a href="/jct09002c/us/en/university/scholars/products/ondemand/index.html" class="smallplainlink">
                                                    <img src="i/Cert_OD_Mark_Home.gif" width="140" height="21" border="0" alt="Certified for IBM On Demand Business" align="middle"  /></a></p></td> 
                                                </tr> 
                                                <tr> 
                                                    <td class="dotted" colspan="2"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>
                                                <tr> 
                                                    <td width="150"><p><a href="http://www.ibm.com/software/lotus/" class="smallplainlink">
                                                    <img src="i/lotus_logo.gif" border="0" width="118" height="21" alt="Lotus Software" align="middle"  /></a></p></td> 
                                                </tr> 
                                                <tr> 
                                                    <td class="dotted" colspan="2"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>

                                                <tr> 
                                                    <td width="150"><p><a href="/jct09002c/us/en/university/scholars/products/rational/" class="smallplainlink">
                                                    <img src="i/rational_logo.gif" border="0" width="137" height="21" alt="Rational Software" align="middle"  /></a></p></td> 
                                                </tr> 
                                                <tr> 
                                                    <td class="dotted" colspan="2"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>
                                                <tr> 
                                                    <td width="150"><p><a href="http://www.ibm.com/software/tivoli/" class="smallplainlink">
                                                    <img src="i/tivoli_logo.gif" border="0" width="127" height="21" alt="Tivoli Software" align="middle" /></a></p></td> 
                                                </tr>
                                                <tr>
                                                    <td class="dotted" colspan="2"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>

                                                <tr> 
                                                    <td width="150">
                                                        <p><a href="/jct09002c/us/en/university/scholars/products/websphere/" class="smallplainlink">
                                                        <img src="i/websphere_logo.gif" border="0" width="113" height="21" alt="WebSphere Software" align="middle" /></a></p>
                                                    </td>
                                                </tr>
                                            </table>
                                            <!-- END Brand Logo Section -->
                                        </td>
                                    </tr>
                                </table>
                                <br />
                                    
                                <!-- BEGIN Newsletter Signup Section -->
                                <table cellspacing="0" cellpadding="0" border="0" width="150">
                                    <tr>
                                        <td class="v14-header-3-small">Newsletter</td>
                                    </tr>
                                </table>
                                <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border" width="150">
                                    <tr>
                                        <td colspan="3"><img alt="" height="5" src="i/c.gif" width="1" /></td>
                                    </tr>
                                    <tr>
                                        <td width="150" valign="middle">
                                            <form action="NewsletterSignup.jsp" onSubmit="return emailCheck(this.email.value)" method="post" target="New_Window">
                                                <input type="hidden" name="lists" value="ibmscholars@ibm.email-publisher.com" />
                                                <table cellpadding="0" cellspacing="0" border="0" >
                                                    <tr>
                                                        <td>
                                                            Subscribe<br />
                                                            <label for="thetext"></label>
                                                            <input type="text" id="thetext" size="15" name="email" value="enter e-mail"  class="iform" onblur="if (this.value == '') this.value='e-mail address';" onfocus="if (this.value == 'e-mail address') this.value=''"/>
                                                        </td> 
                                                    </tr>
                                                    <tr>
                                                        <td>Language<br />
                                                            <select name="language" id="language" onChange="toggle()">
                                                                <option value="english">English</option>
                                                                <option value="japanese">Japanese</option>
                                                            </select>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td class="no-padding">
                                                            <table cellpadding="0" cellspacing="0">
                                                                <tr>
                                                                    <td><input type="radio" name="type" id="news_html" value="HTML" checked="checked" />HTML</td>
                                                                    <td><input type="radio" name="type" id="news_text" value="TEXT" /><label for="text">Text</label></td>
                                                                    <td><label for="go"></label><input type="image" id="go" value="submit" src="i/go.gif" border="0" width="21" height="21" alt="Go" /></td>
                                                                </tr>
                                                            </table>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td><img src="i/c.gif" width="1" alt="" height="4" border="0" /></td>
                                                    </tr>
                                                    <tr>
                                                        <td>View <a href="/jct09002c/university/scholars/newsletter/" class="w">current</a> issue.</td>
                                                    </tr>
                                                    <tr>
                                                        <td><img src="i/c.gif" width="1" alt="" height="6" border="0" /></td>
                                                    </tr>
                                                </table> 
                                            </form>
                                        </td>
                                    </tr>
                                </table>
                                <!-- END Newsletter Signup Section -->
                                <br />
                                
                                <!-- BEGIN Resources Section -->
                                <table border="0" cellpadding="0" cellspacing="0" width="150"> 
                                    <tr>
                                        <td class="v14-header-3-small">Resources</td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border" width="150"> 
                                                <tr>
                                                    <td class="no-padding">
                                                        <table border="0" cellpadding="0" cellspacing="0" width="143">
                                                            <tr valign="top">
                                                                <td  valign="top"  class="ipt" width="17"><img src="i/fw.gif" height="16" width="16" alt=""/></td>
                                                                <td width="125" class="npl"><p><a class="smallplainlink" href="/jct09002c/university/scholars/facultyawards/index.html">Faculty Awards</a></p></td>
                                                            </tr>
                                                        </table>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td class="dotted"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>
                                                <tr>
                                                    <td class="no-padding">
                                                        <table border="0" cellpadding="0" cellspacing="0" width="143">
                                                            <tr valign="top"> 
                                                                <td  valign="top"  class="ipt" width="17"><img src="i/fw.gif" height="16" width="16" alt=""/></td>
                                                                <td width="125" class="npl"><p><a class="smallplainlink" href="/jct09002c/us/en/university/scholars/fellowship/phd/">Ph. D. Fellowships</a></p></td>
                                                            </tr>
                                                        </table>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td class="dotted"><img src="i/c.gif" width="1" height="1" alt=""/></td> 
                                                </tr>
                                                <tr>
                                                    <td class="no-padding">
                                                        <table border="0" cellpadding="0" cellspacing="0" width="143">
                                                            <tr valign="top"> 
                                                                <td  valign="top"  class="ipt" width="17"><img src="i/fw.gif" height="16" width="16" alt=""/></td>
                                                                <td width="125" class="npl"><p><a class="smallplainlink" href="/jct09002c/us/en/university/scholars/sur/">Shared University Research</a></p></td>
                                                            </tr>
                                                        </table>
                                                    </td>
                                                </tr>
                                            </table>
                                        </td>
                                    </tr>
                                </table>
                                <!-- END Resources Section -->
                                <br />
                                
                                <!-- BEGIN World Community Grid Section -->
                                <table border="0" cellpadding="0" cellspacing="0" width="150">
                                    <tr>
                                        <td class="v14-header-3-small">FightAIDS@Home<br />Join now!</td>
                                    </tr>
                                    <tr>
                                        <td>	
                                            <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border">
                                                <tr>
                                                    <td width="150" align="center"><a class="smallplainlink" href="http://www.ibm.com/ibm/ibmgives/news/wcg.shtml">
                                                        <img alt="World Community Grid" width="144" height="96" hspace="0" vspace="0" border="0" src="http://www.ibm.com/ibm/ibmgives/images/thumbs/wcg.gif" /></a>
                                                    </td>
                                                </tr>
                                            </table>
                                        </td>
                                    </tr>
                                </table>
                                <!-- END World Community Grid Section -->
                                <br />
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
                            <td><span class="spacer">&nbsp;&nbsp;&nbsp;&nbsp;</span><a href="http://www.ibm.com/ibm/" class="mainlink">About IBM</a></td>
                            <td width="27" class="footer-divider">&nbsp;&nbsp;&nbsp;&nbsp;</td>
                            <td><a href="http://www.ibm.com/privacy/" class="mainlink">Privacy</a></td>
                            <td width="27" class="footer-divider">&nbsp;&nbsp;&nbsp;&nbsp;</td>
                            <td><a href="http://www.ibm.com/contact/" class="mainlink">Contact</a></td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
        <!-- END Page Footer -->
        
        <!-- BEGIN surfaid code include -->
        <script type="text/javascript" language="JavaScript1.2" src="//www.ibm.com/common/stats/stats.js"></script>
        <noscript><img src="//i.ihost.com/i/c.gif" width="1" height="1" alt="" border="0" /></noscript>
        <!-- END surfaid code include -->
    </body>
</html>

