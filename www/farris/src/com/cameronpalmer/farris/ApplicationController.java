package com.cameronpalmer.farris;

public interface ApplicationController {
	void initialize();
	void destroy();
	public ResponseContext handleRequest(RequestContext requestContext);
	//void handleResponse(RequestContext requestContext, ResponseContext responseContext);
	
}
