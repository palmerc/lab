package com.cameronpalmer.farris.filter;

import java.io.IOException;
import java.util.zip.GZIPOutputStream;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;

public class CompressionFilter implements Filter {
	private Logger logger;
	private FilterConfig config;
	
	public void init(FilterConfig config) throws ServletException {
		this.config = config;
		logger = Logger.getLogger(CompressionFilter.class);
		logger.info(config.getFilterName() + " initialized.");
	}
	
	public void destroy() {
		config = null;
		logger = null;
	}
	
	public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain chain) throws IOException, ServletException {
		HttpServletRequest request = (HttpServletRequest) servletRequest;
		HttpServletResponse response = (HttpServletResponse) servletResponse;
			
		String validEncodings = request.getHeader("Accept-Encoding");
		if ( (validEncodings != null) && (validEncodings.indexOf("gzip") > -1) ) {
			CompressionResponseWrapper wrappedResponse = new CompressionResponseWrapper(response);
			wrappedResponse.setHeader("Content-Encoding", "gzip");
			
			chain.doFilter(request, wrappedResponse);
			
			GZIPOutputStream gzos = wrappedResponse.getGZIPOutputStream();
			gzos.finish();
			logger.info(config.getFilterName() + ": finished the request.");
		} else {
			logger.info(config.getFilterName() + ": no encoding performed.");
			chain.doFilter(request, response);
		}
	}
}
