package com.cameronpalmer.farris.command;

import com.cameronpalmer.farris.context.RequestContextFactory;
import com.cameronpalmer.farris.context.RequestContextFactory.Commands;

/**
 * The CommandMapper singleton
 * @author palmerc
 *
 */
public class CommandMapper {
	private static CommandMapper instance = null;
	
	protected CommandMapper() {
		
	}
	
	static public CommandMapper getInstance() {
		if ( instance == null ) {
			instance = new CommandMapper();
		}
		return instance;
	}
	
	public Command getCommand(Commands commandName) {
		
		return null;
	}
	
	public CommandMap getCommandMap(Commands commandId) {
		if ( commandId == Commands.FRONTPAGE ) {
			return new CommandMap(null, "main.jsp");
		}
		return null;
	}
}
