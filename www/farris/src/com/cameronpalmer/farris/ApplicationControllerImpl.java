package com.cameronpalmer.farris;

import com.cameronpalmer.farris.RequestContextFactory.Commands;

public class ApplicationControllerImpl implements ApplicationController {
	private CommandMapper commandMapper;

	public void initialize() {
		commandMapper = CommandMapper.getInstance();	
	}
	
	public void destroy() {
		// TODO Auto-generated method stub
		
	}

	public ResponseContext handleRequest(RequestContext requestContext) {
		ResponseContext responseContext = null;
		try {
			requestContext.validate();
			
			Commands commandName = requestContext.getCommandName();
			Command command = commandMapper.getCommand(commandName);
			
			responseContext = command.execute(requestContext);
			
			CommandMap mapEntry = commandMapper.getCommandMap(commandName);
			String viewName = mapEntry.getViewName();
			responseContext.setLogicalViewName(viewName);
		} catch (ValidatorException e) {
		}
		return responseContext;
	}

	/*
	@Override
	public void handleResponse(RequestContext requestContext,
			ResponseContext responseContext) {
		// TODO Auto-generated method stub
		
	}*/
}
