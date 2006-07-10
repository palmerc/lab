package com.example.web;

import javax.servlet.*;
import javax.servlet.http.*;
import java.io.*;

public class BeerSelect extends HttpServlet {
    public void doPost(HttpServletRequest request,
                    HttpServletResponse response)
                    throws IOException, ServletException {
        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
        out.println("Beer Selection Advice<br />");
        String c = request.getParameter("color");
        out.println("<br />Got beer color " + c);
    }
}