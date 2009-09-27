package com.cameronpalmer.farris.servlet;

import java.io.IOException;
import java.util.Arrays;
import java.util.Date;
import java.util.Enumeration;
import java.util.List;
import java.util.UUID;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

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
		HttpSession session = request.getSession();
		
		String serviceType = null;
		String uuid = null;
		Date now = new Date();
		
		
		BlogService bs = new BlogServiceImpl();
	
		//Logger logger = Logger.getLogger(this.getClass());
		//logger.debug("Session ID: " + session.getId());
		String formType = request.getParameter("formType");
		if ( formType != null ) {
			if ( "postUpdate".equals(formType) ) {
				uuid = request.getParameter("postUuid");
				if ( isUUID(uuid) ) {
					Post post = bs.getPost(UUID.fromString(uuid));
					post.setSubject(request.getParameter("postSubject"));
					post.setBody(request.getParameter("postBody"));
					post.setUpdatedDate(now);
					bs.updatePost(post);
				}		

				String publish = request.getParameter("publishPost");
				if ( publish != null ) {
					serviceType = POSTSERVICE;
				} else {
					serviceType = EDITSERVICE;
				}
			}		
		}
		
		//logger.debug("Path: " + request.getRequestURI() + " or: " + getServletContext().getContextPath());
		if ( request.getRequestURI().equals((getServletContext().getContextPath() + "/")) ) {
			serviceType = MAINSERVICE;
		} else {
			StringBuffer url = request.getRequestURL();
			List<String> urlPieces = Arrays.asList(url.toString().split("/"));
			for ( int i = urlPieces.size() - 1; i >= 0 ; i-- ) {
				//logger.debug(urlPieces.get(i));
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
					break;
				}
			}
		}
			String page = null;		
			RequestDispatcher view = null;
			if ( COMPOSESERVICE.equals(serviceType) ) {
				Post post = new Post();
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
				Post post = bs.getPost(UUID.fromString(uuid));
				request.setAttribute("post", post);
				page = EDITPAGE;
			} else if ( POSTSERVICE.equals(serviceType) ) {
				Post post = bs.getPost(UUID.fromString(uuid));
				request.setAttribute("post", post);
				page = POSTPAGE;
			} else if ( POSTSSERVICE.equals(serviceType) ) {
				List<Post> posts = bs.getPosts();
				request.setAttribute("posts", posts);
				page = POSTSPAGE;
			} else if ( MAINSERVICE.equals(serviceType) ) {
				List<Post> posts = bs.getPosts();
				request.setAttribute("posts", posts);
				page = MAINPAGE;
			}
		

		if ( page != null ) {
			view = request.getRequestDispatcher(page);
			view.forward(request, response);
		}
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

	void getPosts(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String page = null;		
		RequestDispatcher view = null;
		
		//BlogService bs = new BlogServiceImpl();
		//List<Post> posts = bs.getPosts();
		//request.setAttribute("posts", posts);
		page = POSTSPAGE;
		view = request.getRequestDispatcher(page);
		view.forward(request, response);
	}
}
