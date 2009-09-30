package com.cameronpalmer.farris.command;

import java.util.UUID;

import com.cameronpalmer.farris.context.RequestContext;
import com.cameronpalmer.farris.context.ResponseContext;
import com.cameronpalmer.farris.context.ResponseContextFactory;
import com.cameronpalmer.farris.delegate.PostDelegate;
import com.cameronpalmer.farris.delegate.PostDelegateImpl;
import com.cameronpalmer.farris.to.PostTO;

public class ViewPostCommand implements Command {
	public ViewPostCommand() {
		
	}

	public ResponseContext execute(RequestContext requestContext) {
		String uuidString = requestContext.getStringParameter("Uuid");
		UUID uuid = UUID.fromString(uuidString);
		
		PostDelegate delegate = new PostDelegateImpl();
		PostTO postTO;
		postTO = delegate.getPost(uuid);
		
		String logicalViewName = "postObject";
		
		ResponseContextFactory factory = ResponseContextFactory.getInstance();
		
		ResponseContext responseContext = factory.createResponseContext(postTO, logicalViewName);
		
		return responseContext;
	}
	
}
