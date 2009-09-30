package com.cameronpalmer.farris.context;

import javax.servlet.ServletRequest;
import javax.servlet.http.HttpServletRequest;

import org.apache.log4j.Logger;

import com.cameronpalmer.farris.command.CommandMap;
import com.cameronpalmer.farris.command.CommandMapper;

public class RequestContextFactory {
	Logger logger;
	private static RequestContextFactory instance = null;
	
	public static enum Commands { FRONTPAGE, EDITPAGE, COMPOSEPAGE, MANAGEPAGE }
	
	protected RequestContextFactory() {
		logger = Logger.getLogger(RequestContextFactory.class);
	}
	
	public RequestContext createRequestContext(ServletRequest request) {
		RequestContext requestContext = null;
		
		try {
			Commands commandId = getCommandId(request);
			CommandMapper commandMapper = CommandMapper.getInstance();
			CommandMap mapEntry = commandMapper.getCommandMap(commandId);
			Class requestContextClass = mapEntry.getContextObjectClass();
			requestContext = (RequestContext) requestContextClass.newInstance();
			
			requestContext.initialize(request);
		} catch (InstantiationException e) {
			
		} catch (IllegalAccessException e) {
			
		}
		return requestContext;
	}
	
	public static RequestContextFactory getInstance() {
		if ( instance == null ) {
			instance = new RequestContextFactory();
		}
		
		return instance;
	}
	
	private Commands getCommandId(ServletRequest request) {
		Commands commandId = null;
		
		if ( request instanceof HttpServletRequest ) {
			String pathInfo = ((HttpServletRequest) request).getPathInfo();
			// pathInfo will be null if nothing follows the Servlet url
			if ( pathInfo != null ) {
				String pathString = pathInfo.substring(1);
				if ( pathString.equals("/compose") ) {
					commandId = Commands.COMPOSEPAGE;
				} else if ( pathString.equals("/edit") ) {
					commandId = Commands.EDITPAGE;
				} else if ( pathString.equals("/manage") ) {
					commandId = Commands.MANAGEPAGE;
				} else {
					commandId = Commands.FRONTPAGE;
				}
			} else {
				commandId = Commands.FRONTPAGE;
			}
		}
		logger.debug("commandId: " + commandId);
		return commandId;
	}
}


