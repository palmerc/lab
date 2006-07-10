package com.ibm.web;

import javax.servlet.*;
import javax.servlet.http.*;
import java.io.*;
import sun.net.smtp.SmtpClient;

public class NewsletterSignup extends HttpServlet {
    public void doPost(HttpServletRequest request,
                    HttpServletResponse response)
                    throws IOException, ServletException {
    
        String l = request.getParameter("language");
        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
                        
        if (l == "english") {
            out.println("<p>For some reason this managed to print");
        } else if (l == "japanese") {
            String from="cameronp@us.ibm.com";
            String to="cameronp@us.ibm.com";
                            
            SmtpClient client = new SmtpClient("relay.us.ibm.com");
            client.from(from);
            client.to(to);
            PrintStream message = client.startMessage();
            message.println("To: " + to);
            message.println("Subject: Japanese Newsletter Signup");
            String c = request.getParameter("email");
            message.println(c);
            client.closeServer();
        }
        
        out.println("<p>Thank you for signing up for the Academic Initiative newsletter - " + l + "</p>");
    }
}