package com.cameronpalmer.farris.servlet;

import java.io.IOException;
import java.net.MalformedURLException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.juli.*;
import org.easymock.EasyMock;
import org.easymock.EasyMockSupport;
import org.easymock.MockControl;
import org.junit.Before;
import org.junit.Test;
import org.xml.sax.SAXException;

import static org.junit.Assert.*;

import com.meterware.httpunit.PostMethodWebRequest;
import com.meterware.httpunit.WebRequest;
import com.meterware.httpunit.WebResponse;
import com.meterware.servletunit.InvocationContext;
import com.meterware.servletunit.ServletRunner;
import com.meterware.servletunit.ServletUnitClient;


public class BlogServletTest {
	HttpServletRequest blogServletRequest = null;
	HttpServletResponse blogServletResponse = null;
	BlogServlet blogServlet = null;
	
	@Before
	public void init() throws MalformedURLException, IOException, ServletException, SAXException {
		ServletRunner sr = new ServletRunner("/home/palmerc/Development/farris/WebContent/WEB-INF/web.xml");
		sr.registerServlet("BlogServlet", BlogServlet.class.getName());
		ServletUnitClient sc = sr.newClient();

		WebRequest request = new PostMethodWebRequest("http://localhost/BlogServlet");
		InvocationContext ic = sc.newInvocation(request);
		blogServlet = (BlogServlet) ic.getServlet();
		assertNull("A session already exists", ic.getRequest().getSession(false));
		blogServletRequest = ic.getRequest();
		blogServletResponse = ic.getResponse();
	}
	
	
	// basic - get main page without login
	@Test
	public void viewPostsTest() throws ServletException, IOException {
		blogServlet.getPosts(blogServletRequest, blogServletResponse);
	}
	
	// basic - get specific post without login
	public void test() {
		
	}
	
	// advanced - create new post
	
	// advanced - edit post
	
	// advanced - delete post
	
}