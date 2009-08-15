package com.cameronpalmer.farris.blog;

import java.io.Serializable;
import java.util.Date;
import java.util.List;
import java.util.UUID;

public class Blog implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = -3934962598805038129L;
	
	UUID uuid;
	Date date;
	String subject;
	String body;
	
	List<Blog> comments;
	//List<Attachment> attachments;
	// images, audio, documents UUEncoded or binary blob

	public UUID getUuid() {
		return uuid;
	}
	
	public void setUuid(UUID uuid) {
		this.uuid = uuid;
	}
	
	public Date getDate() {
		return date;
	}

	public void setDate(Date date) {
		this.date = date;
	}

	public String getSubject() {
		return subject;
	}

	public void setSubject(String subject) {
		this.subject = subject;
	}

	public String getBody() {
		return body;
	}

	public void setBody(String body) {
		this.body = body;
	}

	public List<Blog> getComments() {
		return comments;
	}

	public void setComments(List<Blog> comments) {
		this.comments = comments;
	}
}
