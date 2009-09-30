package com.cameronpalmer.farris.context;

import javax.servlet.ServletResponse;

import com.cameronpalmer.farris.controller.Dispatcher;

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

	public void setDispatcher(Dispatcher dispatcher) {
		// TODO Auto-generated method stub
		
	}
	
}
