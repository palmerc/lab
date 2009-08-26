package com.cameronpalmer.farris.storage.dao;

import java.sql.SQLException;

import org.junit.Test;

import com.cameronpalmer.farris.storage.factory.DAOFactory;
import com.cameronpalmer.farris.storage.factory.DAOFactory.DAOFactoryType;


public class PostgresBlogDAOTest {
	@Test
	public void insertionTest() throws ClassNotFoundException, SQLException {
		DAOFactory postgresDAOFactory = DAOFactory.getDAOFactory(DAOFactoryType.POSTGRES);
		BlogDAO blogDAO = postgresDAOFactory.getBlogDAO();
		blogDAO.insert(blog);
	}
}
