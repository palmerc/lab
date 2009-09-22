package com.cameronpalmer.farris.filter;

import java.io.IOException;
import java.util.Enumeration;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletResponse;

public class ResponseHeaderFilter implements Filter {
	FilterConfig config;
	
	public void init(FilterConfig filterConfig) {
		this.config = filterConfig;
	}

	public void destroy() {
		this.config = null;
	}

	@SuppressWarnings("unchecked")
	public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse,
			FilterChain chain) throws IOException, ServletException {
		
		HttpServletResponse httpServletResponse = (HttpServletResponse) servletResponse;
		
		// set the provided HTTP response parameters
		for ( Enumeration<String> e = config.getInitParameterNames(); e.hasMoreElements(); ) {
			String headerName = e.nextElement();
			httpServletResponse.addHeader(headerName, config.getInitParameter(headerName));
		}
		
		// pass the request/response on
		chain.doFilter(servletRequest, httpServletResponse);
	}
}
