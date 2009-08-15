package com.cameronpalmer.farris.storage.factory;

import com.cameronpalmer.farris.storage.dao.BlogDAO;

/**
 * The abstract DAO Factory
 * @author palmerc
 *
 */
public abstract class DAOFactory {
	public enum DAOFactoryType { POSTGRES, XML }
	
	public abstract BlogDAO getBlogDAO();
	
	public static DAOFactory getDAOFactory(DAOFactoryType whichFactory) {
		switch (whichFactory) {
			case POSTGRES:
				return new PostgresDAOFactory();
			case XML:
				return new XmlDAOFactory();
			default:
				return null;
		}
	}
}
