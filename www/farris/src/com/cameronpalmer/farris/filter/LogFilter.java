package com.cameronpalmer.farris.filter;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;

public class LogFilter implements Filter {
	@SuppressWarnings("unused")
	private FilterConfig config;
	private Logger log;
	
	public void init(FilterConfig filterConfig) throws ServletException {
		this.config = filterConfig;
		
		//PropertyConfigurator.configure(config.getServletContext().getRealPath("/") + "WEB-INF/classes/log4j.properties");
		
		log = Logger.getLogger(LogFilter.class);
		
		log.info("Logger instantiated in " + getClass().getName());
	}
	
	public void destroy() {
		log = null;
	}

	public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain chain) throws IOException, ServletException {
		HttpServletRequest request = null;
		HttpServletResponse response = null;
		
		if ( log != null && (servletRequest instanceof HttpServletRequest) || (servletResponse instanceof HttpServletResponse) ) {
			request = (HttpServletRequest) servletRequest;
			response = (HttpServletResponse) servletResponse;
			log.info(response.getCharacterEncoding());
			log.info("Request received from: " + request.getRemoteHost() + " for " + request.getRequestURL());
		}
		
		chain.doFilter(servletRequest, servletResponse);
	}
}