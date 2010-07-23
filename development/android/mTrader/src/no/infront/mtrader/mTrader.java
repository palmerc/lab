package no.infront.mtrader;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.TextView;

public class mTrader extends Activity {
	public static final String encoding = "ISO-8859-1";
	public static final String host = "wireless.theonlinetrader.com";
	public static final int port = 7780;

	Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			String message = (String) msg.obj;
			updateResultsInUI(message);
			Log.d("MSG", message);
		}
	};

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {    	
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		connect();
	}
	
	private void updateResultsInUI(String s) {
		TextView tv = (TextView) findViewById(R.id.stocks);
		tv.setText(s);
	}

	protected void connect() {
		Thread t = new Thread() {
			public void run() {
				Socket s = null;
				OutputStream out = null;
				BufferedReader in = null;	

				try {
					Log.d("CONNECT", "start");

					InetAddress ipAddress = InetAddress.getByName(host);
					try {
						s = new Socket(ipAddress, port);
						Log.d("CONNECTED", s.toString());
						out = s.getOutputStream();
						in = new BufferedReader(new InputStreamReader(s.getInputStream()));
					} catch (UnknownHostException e) {
						Log.e(this.toString(), "Don't know about host: " + ipAddress);
					} catch (IOException e) {
						Log.e(this.toString(), "Couldn't get I/O for the connection to: " + ipAddress);
					}
					try {

						String login = "Action: login\r\n" +
						"Authorization: cameron/sierra\r\n" +
						"Platform: Android\r\n" +
						"Client: mTrader\r\n" +
						"Protocol: 2.0\r\n" +
						"VerType: 1.0.0" +
						"ConnType: Socket\r\n" +
						"Streaming: 1\r\n" +
						"QFields: t;l;c;cp;a;b\r\n\r\n";
						byte[] b = login.getBytes(encoding);
						out.write(b);

						StringBuilder sb = null;
						while (true) {
							if (sb == null) {
								sb = new StringBuilder();
							}

							int c = in.read();
							if (c != -1) {
								sb.append((char)c);
								if (c == '\n') {
									Message m = Message.obtain();
									m.obj = sb.toString();
									mHandler.sendMessage(m);
									Log.d("LINE", sb.toString());
									sb = null;
								}
							}
						}
					} catch (IOException e) {
						e.printStackTrace();
					}

					try {
						in.close();
						out.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				} catch(UnknownHostException e) {
					Log.e("CONNECT", e.getLocalizedMessage());
				}    	

			}
		};
		t.start();
	}
}