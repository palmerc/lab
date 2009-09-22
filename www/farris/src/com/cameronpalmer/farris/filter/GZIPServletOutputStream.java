package com.cameronpalmer.farris.filter;

import java.io.IOException;
import java.util.zip.GZIPOutputStream;

import javax.servlet.ServletOutputStream;

public class GZIPServletOutputStream extends ServletOutputStream {
	GZIPOutputStream stream;
	
	GZIPServletOutputStream(ServletOutputStream sos) throws IOException {
		stream = new GZIPOutputStream(sos);
	}

	public void write(int b) throws IOException {
		stream.write(b);
	}
	
	public void write(byte[] b) throws IOException {
		stream.write(b);
	}
	
	public void write(byte[] b, int off, int len) throws IOException {
		stream.write(b, off, len);
	}
}
