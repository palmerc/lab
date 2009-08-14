import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class FrontEnd extends HttpServlet {
	/**
	 * 
	 */
	private static final long serialVersionUID = -4753442072639671693L;

	public void doGet(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {
		res.setContentType("text/html");
		
	
	
		PrintWriter out = res.getWriter();
		out.println("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" " +
   "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
		out.println("<html>" +
				"<head><title>Hello, World!</title></head>" +
				"<body><p>Hello, World!</p></body>" +
				"</html>");
	
	
	}
}