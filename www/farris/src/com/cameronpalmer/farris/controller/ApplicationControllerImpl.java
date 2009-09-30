package com.cameronpalmer.farris.controller;

import com.cameronpalmer.farris.ValidatorException;
import com.cameronpalmer.farris.command.Command;
import com.cameronpalmer.farris.command.CommandMap;
import com.cameronpalmer.farris.command.CommandMapper;
import com.cameronpalmer.farris.context.RequestContext;
import com.cameronpalmer.farris.context.RequestContextFactory;
import com.cameronpalmer.farris.context.ResponseContext;
import com.cameronpalmer.farris.context.RequestContextFactory.Commands;

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

	public void handleResponse(RequestContext requestContext, ResponseContext responseContext) {		
	}
}
