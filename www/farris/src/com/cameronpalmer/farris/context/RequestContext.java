package com.cameronpalmer.farris.context;

import javax.servlet.ServletRequest;

import com.cameronpalmer.farris.ValidatorException;
import com.cameronpalmer.farris.context.RequestContextFactory.Commands;

public class RequestContext {
	private ServletRequest request;

	public RequestContext(ServletRequest request) {
		setRequest(request);
	}

	public void initialize(ServletRequest request) {
		setRequest(request);
	}
	
	public void setRequest(ServletRequest request) {
		this.request = request;
	}
	
	public void validate() throws ValidatorException {
		
	}

	public Commands getCommandName() {
		return null;
	}

	public String getStringParameter(String string) {
		// TODO Auto-generated method stub
		return null;
	}
}
