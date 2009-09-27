package com.cameronpalmer.farris.filter;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.zip.GZIPOutputStream;

import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpServletResponseWrapper;

public class CompressionResponseWrapper extends HttpServletResponseWrapper {
	private GZIPServletOutputStream outputStream = null;
	private PrintWriter printWriter = null;
	private Object streamUsed = null;

	public CompressionResponseWrapper(HttpServletResponse response) {
		super(response);
	}

	public ServletOutputStream getOutputStream() throws IOException {
		if ((streamUsed != null) && (streamUsed != outputStream)) {
			throw new IllegalStateException();
		}
		if (outputStream == null) {
			outputStream = new GZIPServletOutputStream(getResponse().getOutputStream());
			streamUsed = outputStream;
		}
		return outputStream;
	}

	public PrintWriter getWriter() throws IOException {
		if ((streamUsed != null) && (streamUsed != printWriter)) {
			throw new IllegalStateException();
		}
		if (printWriter == null) {
			outputStream = new GZIPServletOutputStream(getResponse().getOutputStream());
			OutputStreamWriter osw = new OutputStreamWriter(outputStream, getResponse().getCharacterEncoding());
			printWriter = new PrintWriter(osw);
			streamUsed = printWriter;
		}
		return printWriter;
	}


	public void setContentLength(int len) {}
	
	public GZIPOutputStream getGZIPOutputStream() throws IOException {
		if ( outputStream == null ) {
			getOutputStream();
		}
		return outputStream.stream;
	}
}
