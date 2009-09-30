package com.cameronpalmer.farris.context;

import javax.servlet.ServletResponse;

public class ResponseContext {
	private ServletResponse response;
	
	public ResponseContext(ServletResponse response) {
		setResponse(response);
	}
	
	public void setResponse(ServletResponse response) {
		this.response = response;
	}

	public void setLogicalViewName(String viewName) {
		
	}
	
}
