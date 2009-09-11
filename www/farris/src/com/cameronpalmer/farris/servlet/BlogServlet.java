package com.cameronpalmer.farris.servlet;

import java.io.IOException;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.UUID;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.cameronpalmer.farris.blog.BlogService;
import com.cameronpalmer.farris.blog.BlogServiceImpl;
import com.cameronpalmer.farris.blog.Post;

/**
 * Servlet implementation class BlogServlet
 */
public class BlogServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
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
		request.setCharacterEncoding("UTF8");
		BlogService bs = new BlogServiceImpl();
		
		List<Post> posts = bs.getPosts();
		request.setAttribute("posts", posts);
		
		RequestDispatcher view = null;
		if ( "compose".equals(request.getParameter("type"))  ) { // compose
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
			
			view = request.getRequestDispatcher("posts.jsp");
		} else if ( "postsEditRedirect".equals(request.getParameter("type")) ) {
			view = request.getRequestDispatcher("postsEditPage.jsp");
		} else if ( "postsDisplayRedirect".equals(request.getParameter("type")) ) {	
			view = request.getRequestDispatcher("frontpage.jsp");
		} else {
			view = request.getRequestDispatcher("frontpage.jsp");
		}
		view.forward(request, response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		this.doGet(request, response);
	}

}
