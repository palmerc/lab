package com.cameronpalmer.farris.storage.dao;

import static org.junit.Assert.*;

import java.io.UnsupportedEncodingException;
import java.sql.SQLException;
import java.util.Date;
import java.util.UUID;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.cameronpalmer.farris.storage.factory.DAOFactory;
import com.cameronpalmer.farris.storage.factory.DAOFactory.DAOFactoryType;
import com.cameronpalmer.farris.to.PostTO;


public class PostgresBlogDAOTest {
	PostTO blog;
	
	@Before
	public void init() throws UnsupportedEncodingException {
		Date now = new Date();
		UUID uuid = UUID.randomUUID();
		
		blog = new PostTO();
		blog.setAuthor("palmerc");
		blog.setBody("The body.");
		blog.setDate(now);
		blog.setUuid(uuid);
		blog.setPlace("Vikersund, Norway");
		blog.setPublishedDate(now);
		blog.setSubject("The subject.");
		blog.setUpdatedDate(now);
	}
	
	@After
	public void teardown() throws ClassNotFoundException, SQLException {
		DAOFactory postgresDAOFactory = DAOFactory.getDAOFactory(DAOFactoryType.POSTGRES);
		BlogDAO blogDAO = postgresDAOFactory.getBlogDAO();
		blogDAO.delete(blog.getUuid());
	}
	
	@Test
	public void insertionTest() throws ClassNotFoundException, SQLException, UnsupportedEncodingException {
		DAOFactory postgresDAOFactory = DAOFactory.getDAOFactory(DAOFactoryType.POSTGRES);
		BlogDAO blogDAO = postgresDAOFactory.getBlogDAO();
		blogDAO.insert(blog);
		PostTO result = blogDAO.select(blog.getUuid());
		
		UUID expected = blog.getUuid();
		UUID actual = result.getUuid();
		assertEquals(expected, actual);
	}
	
	@Test
	public void nullInsertionTest() throws ClassNotFoundException, SQLException, UnsupportedEncodingException {
		DAOFactory postgresDAOFactory = DAOFactory.getDAOFactory(DAOFactoryType.POSTGRES);
		BlogDAO blogDAO = postgresDAOFactory.getBlogDAO();
		blogDAO.insert(null);
	}
}
