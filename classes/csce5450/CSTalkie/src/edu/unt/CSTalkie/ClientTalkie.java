package edu.unt.CSTalkie;

import java.io.*;
import java.net.*;

public class ClientTalkie {
	public static void main(String[] args) throws IOException {
		// Turns the stdin byte stream into a chracter stream
		System.out.println("This is the client.");
		System.out.print("Connect to: ");
	    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	    String hostname = br.readLine();
		
		Socket s = new Socket(hostname, 80);
		PrintStream sout = new PrintStream( s.getOutputStream() );
		BufferedReader d = new BufferedReader( new InputStreamReader( s.getInputStream() ) );

		sout.println("GET / HTTP/1.1");
		sout.println("Host: cameronpalmer.com");
		sout.println();
		
		String line = d.readLine();
		
		while (line != null) {
			System.out.println( line );
			
			line = d.readLine();
		}
		
		s.close();
	}

}
