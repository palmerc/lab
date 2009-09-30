package com.cameronpalmer.farris.controller;

import com.cameronpalmer.farris.context.RequestContext;
import com.cameronpalmer.farris.context.ResponseContext;

public interface ApplicationController {
	void initialize();
	void destroy();
	public ResponseContext handleRequest(RequestContext requestContext);
	//void handleResponse(RequestContext requestContext, ResponseContext responseContext);
	
}
