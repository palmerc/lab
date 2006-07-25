<%@ page import="java.io.*" %>
<%@ page import="java.net.*" %>
<%@ page import="sun.net.smtp.SmtpClient" %>
<%
    String email = request.getParameter("email");
    String lang = request.getParameter("language");
    String type = request.getParameter("type");
                         
    if (lang.equals("english")) {
        // Construct data
        String data = URLEncoder.encode("lists", "UTF-8") + "=" + URLEncoder.encode("ibmscholars@ibm.email-publisher.com", "UTF-8");
        data += "&" + URLEncoder.encode("type", "UTF-8") + "=" + URLEncoder.encode(type, "UTF-8");
        data += "&" + URLEncoder.encode("email", "UTF-8") + "=" + URLEncoder.encode(email, "UTF-8");

        // Send data
        URL url = new URL("http://www.email-publisher.com/survey/");
        URLConnection conn = url.openConnection();
        conn.setDoOutput(true);
        OutputStreamWriter wr = new OutputStreamWriter(conn.getOutputStream());
        wr.write(data);
        wr.flush();

        // Get the response
        BufferedReader rd = new BufferedReader(new InputStreamReader(conn.getInputStream()));
        String line;
        while ((line = rd.readLine()) != null) {
%>
            <%= line %>
<%
        }
        wr.close();
        rd.close();
    } else if (lang.equals("japanese")) {
        String from="NO-REPLY@us.ibm.com";
        String to="mazzara@us.ibm.com";
                        
        SmtpClient client = new SmtpClient("austin.ibm.com");
        client.from(from);
        client.to(to);
        PrintStream message = client.startMessage();
        message.println("To: " + to);
        message.println("Subject: Japanese Newsletter Signup");

        message.println(email);
        client.closeServer();
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
    <head>
        <!-- ibm.com V14 OneX css -->
        <link rel="stylesheet" type="text/css" href="//www.ibm.com/common/v14/main.css" />
        <link rel="stylesheet" type="text/css" media="all" href="//www.ibm.com/common/v14/screen.css" />
        <link rel="stylesheet" type="text/css" media="screen,print" href="//www.ibm.com/common/v14/table.css" />
        <link rel="stylesheet" type="text/css" media="print" href="//www.ibm.com/common/v14/print.css" />
        <script type="text/javascript" language="JavaScript" src="//www.ibm.com/common/v14/detection.js"></script>

        <!-- legacy pwd css -->
        <link rel="stylesheet" type="text/css" href="/applet/css/pwd.css" />
        <link rel="SHORTCUT ICON" href="http://www.ibm.com/favicon.ico" />

        <!--
        <style type="text/css">
            P,ul,li,ol {
                font-size : 9pt;
                font-family : Arial, sans-serif;
            }
        </style>
        -->
    </head>
    <body>
        <script type="text/javascript" language="javascript" src="/us/en/university/scholars/js/browser2.js"></script>
        <!-- begin surfaid script -->
            <script type="text/javascript" language="JavaScript1.2" src="//www.ibm.com/common/stats/stats.js"></script>
            <noscript><img src="//stats.www.ibm.com/rc/images/uc.GIF?R=noscript" width="1" height="1" alt="" border="0" /></noscript>
        <!-- end surfaid script -->
        <p>Thank you for signing up for the Japanese text version of the IBM Academic Initiative newsletter.</p>
        <p><a href="javascript:window.close()">Close window</a></p>
    </body>
</html>
<%
    }
%>