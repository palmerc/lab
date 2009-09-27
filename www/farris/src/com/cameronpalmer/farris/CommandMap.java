package com.cameronpalmer.farris;

public class CommandMap {
	private Class contextObjectClass = null;
	private String viewName = null;

	public CommandMap(Class contextObjectClass, String viewName) {
		setContextObjectClass(contextObjectClass);
		setViewName(viewName);
	}
	
	public void setContextObjectClass(Class contextObjectClass) {
		this.contextObjectClass = contextObjectClass;
	}
	
	public void setViewName(String viewName) {
		this.viewName = viewName;
	}
	
	public Class getContextObjectClass() {
		return contextObjectClass;
	}
	
	public String getViewName() {
		return viewName;
	}

}
