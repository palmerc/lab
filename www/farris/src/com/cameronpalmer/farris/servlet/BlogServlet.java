package com.cameronpalmer.farris.servlet;

import java.io.IOException;
import java.util.Arrays;
import java.util.Date;
import java.util.List;
import java.util.UUID;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;

import com.cameronpalmer.farris.blog.BlogService;
import com.cameronpalmer.farris.blog.BlogServiceImpl;
import com.cameronpalmer.farris.blog.Post;

/**
 * Servlet implementation class BlogServlet
 */
public class BlogServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
	
	// Character encoding
	static final String CHARENCODING = "UTF8";
	
	// Services
	static final String COMPOSESERVICE= "compose";
	static final String EDITSERVICE = "edit";
	static final String MAINSERVICE = "main";
	static final String POSTSERVICE = "post";
	static final String POSTSSERVICE = "posts";
	
	// JSP Pages
	static final String COMPOSEPAGE = "/compose.jsp";
	static final String EDITPAGE = "/edit.jsp";
	static final String MAINPAGE = "/main.jsp";
	static final String POSTPAGE = "/post.jsp";
	static final String POSTSPAGE = "/posts.jsp";
	
    /**
     * @see HttpServlet#HttpServlet()
     */
    public BlogServlet() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		request.setCharacterEncoding(CHARENCODING);
		BlogService bs = new BlogServiceImpl();
			
		Logger logger = Logger.getLogger(this.getClass());
		
		StringBuffer url = request.getRequestURL();
		List<String> urlPieces = Arrays.asList(url.toString().split("/"));
		String serviceType = null;
		String uuid = null;
		for ( int i = urlPieces.size() - 1; i >= 0 ; i-- ) {
			logger.debug(urlPieces.get(i));
			String piece = urlPieces.get(i);
			
			if ( COMPOSESERVICE.equals(piece) ) {
				serviceType = COMPOSESERVICE;
			} else if ( EDITSERVICE.equals(piece) ) {
				serviceType = EDITSERVICE;
			} else if ( MAINSERVICE.equals(piece) ) {
				serviceType = MAINSERVICE;
			} else if ( POSTSERVICE.equals(piece) ) {
				serviceType = POSTSERVICE;
			} else if ( POSTSSERVICE.equals(piece) ) {
				serviceType = POSTSSERVICE;
			} else if ( isUUID(piece) ) {
				uuid = piece;
			}
			
			if ( serviceType != null ) {
				logger.debug("Looped " + (urlPieces.size()-i) + " times.");
				break;
			}
		}
		
		String page = null;		
		RequestDispatcher view = null;
		if ( COMPOSESERVICE.equals(serviceType) ) {
			Post post = new Post();
			Date now = new Date();
			post.setUuid(UUID.randomUUID());
			post.setSubject(request.getParameter("postSubject"));
			post.setBody(request.getParameter("postBody"));
			post.setAuthor(request.getParameter("postAuthor"));
			post.setPlace(request.getParameter("postPlace"));
			post.setDate(now);
			post.setPublishedDate(now);
			post.setUpdatedDate(now);
		
			bs.insertPost(post);
			
			page = POSTSPAGE;			
		} else if ( EDITSERVICE.equals(serviceType) ) {
			page = EDITPAGE;		
		} else if ( POSTSERVICE.equals(serviceType) ) {
			Post post = bs.getPost(UUID.fromString(uuid));
			request.setAttribute("post", post);
			page = POSTPAGE;
		} else if ( POSTSSERVICE.equals(serviceType) ) {
			List<Post> posts = bs.getPosts();
			request.setAttribute("posts", posts);
			page = POSTSPAGE;
		} else {
			List<Post> posts = bs.getPosts();
			request.setAttribute("posts", posts);
			page = MAINPAGE;			
		}

		view = request.getRequestDispatcher(page);
		view.forward(request, response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		this.doGet(request, response);
	}
	
	private boolean isUUID(String uuid) {
		return uuid.matches("[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}");
	}

}
