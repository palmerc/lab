package com.cameronpalmer.farris.blog;

import java.util.UUID;

public interface BlogService {
	public Blog getBlog(UUID uuid);
	public void setBlog(Blog blogObject);
}
