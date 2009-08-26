package com.cameronpalmer.farris.storage.factory;

import java.sql.*;

import com.cameronpalmer.farris.storage.dao.BlogDAO;
import com.cameronpalmer.farris.storage.dao.PostgresBlogDAO;

/** 
 * Concrete factory for Postgres
 * @author palmerc
 *
 */
public class PostgresDAOFactory extends DAOFactory {
	// FIXME At some point these hardcoded values should be moved to a properties file
	public static final String DRIVER="org.postgresql.Driver";
	public static final String DBURL="jdbc:postgresql://localhost:5432/farris";

	// method to create Cloudscape connections
	public static Connection createConnection() throws ClassNotFoundException, SQLException {
		Class.forName(DRIVER);
		return DriverManager.getConnection(DBURL, "postgres", "postgres");
	}

	@Override
	public BlogDAO getBlogDAO() throws ClassNotFoundException, SQLException {
		return new PostgresBlogDAO();
	}
}
