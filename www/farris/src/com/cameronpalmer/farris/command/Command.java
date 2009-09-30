package com.cameronpalmer.farris.command;

import com.cameronpalmer.farris.context.RequestContext;
import com.cameronpalmer.farris.context.ResponseContext;

public interface Command {
	public ResponseContext execute(RequestContext requestContext);
}
