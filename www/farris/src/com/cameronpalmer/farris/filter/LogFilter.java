package com.cameronpalmer.farris.filter;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;

import org.apache.log4j.Logger;

public class LogFilter implements Filter {
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

	public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
		HttpServletRequest httpServletRequest = null;
		
		if ( log != null && (request instanceof HttpServletRequest) ) {
			httpServletRequest = (HttpServletRequest) request;
			log.info("Request received from: " + httpServletRequest.getRemoteHost() + " for " + httpServletRequest.getRequestURL());
		}
		
		chain.doFilter(request, response);
	}
}