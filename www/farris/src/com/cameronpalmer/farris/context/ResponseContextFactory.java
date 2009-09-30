package com.cameronpalmer.farris.context;

import org.apache.log4j.Logger;

import com.cameronpalmer.farris.to.PostTO;

public class ResponseContextFactory {
	private Logger logger;
	private static ResponseContextFactory instance = null;
	
	public ResponseContextFactory() {
		logger = Logger.getLogger(ResponseContextFactory.class);
	}
	
	public static ResponseContextFactory getInstance() {
		if ( instance == null ) {
			instance = new ResponseContextFactory();
		}

		return instance;
	}

	public ResponseContext createResponseContext(PostTO postTO,	String logicalViewName) {
		ResponseContext responseContext = null;
			
		return responseContext;
	}

}
