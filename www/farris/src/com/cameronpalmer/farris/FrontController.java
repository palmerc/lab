package com.cameronpalmer.farris;

import java.io.IOException;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;

/**
 * Servlet implementation class FrontController
 */
public class FrontController extends HttpServlet {
    /**
	 * 
	 */
	private static final long serialVersionUID = 2543430469810556308L;
	private ApplicationController applicationController;
	private Logger log;
   
    public void init(ServletConfig servletConfig) throws ServletException {
    	super.init(servletConfig);
    	
    	log = Logger.getLogger(FrontController.class);
    	
    	applicationController = new ApplicationControllerImpl();
    	applicationController.initialize();
    }

    protected void processRequest(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    	log.debug("Entering FrontController::processRequest");
    	
    	RequestContextFactory requestContextFactory = RequestContextFactory.getInstance();
    	RequestContext requestContext = requestContextFactory.createRequestContext(request);
    	/*
    	ResponseContext responseContext = applicationController.handleRequest(requestContext);
    	Dispatcher dispatcher = new Dispatcher(request, response);
    	responseContext.setDispatcher(dispatcher);
    	applicationController.handleResponse(requestContext, responseContext);*/
    }
    
	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		processRequest(request, response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		processRequest(request, response);
	}

}
