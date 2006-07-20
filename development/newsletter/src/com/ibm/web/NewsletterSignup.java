package com.ibm.web;

import javax.servlet.*;
import javax.servlet.http.*;
import java.io.*;
import java.net.*;
import sun.net.smtp.SmtpClient;

public class NewsletterSignup extends HttpServlet {
    public void doPost(HttpServletRequest request,
                    HttpServletResponse response)
                    throws IOException, ServletException {
    
        String email = request.getParameter("email");
        String lang = request.getParameter("language");
        String type = request.getParameter("type");
                        
        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
        
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
                out.println(line);
            }
            wr.close();
            rd.close();
            //out.println("<p>For some reason this managed to print");
        } else if (lang.equals("japanese")) {
            String from="sspatel@us.ibm.com";
            String to="cameronp@us.ibm.com";
                            
            SmtpClient client = new SmtpClient("relay.us.ibm.com");
            client.from(from);
            client.to(to);
            PrintStream message = client.startMessage();
            message.println("To: " + to);
            message.println("Subject: Japanese Newsletter Signup");

            message.println(email);
            client.closeServer();
            out.println("<p>Thank you for signing up for the Academic Initiative newsletter - " + lang + "</p>");
        }
    }
}