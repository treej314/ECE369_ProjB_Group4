//TCPServer.java

import java.io.*;
import java.net.*;

class TCPServer 
{
	public static void main(String argv[]) throws Exception
	{
		String fromclient;
		int port = 9092;

		if (argv.length == 0)
		{
		    System.out.println("Incorrect command line arguments. Expected port");
		    return;
                }

		port = Integer.parseInt(argv[0]);

		System.out.println("TCPServer running over port: " + port);
				
		ServerSocket Server = new ServerSocket (port);
		
		System.out.println ("TCPServer Waiting for client on port port");

		while(true) 
		{
			Socket connected = Server.accept();
			System.out.println("(" + 
				connected.getInetAddress() +":"+connected.getPort()+") connected.");
			
			BufferedReader inFromClient =
				new BufferedReader(new InputStreamReader (connected.getInputStream()));
			
			PrintWriter outToClient =
				new PrintWriter(connected.getOutputStream(),true);
			
			while ( true )
			{            	
				fromclient = inFromClient.readLine();
				System.out.println("Got: " + fromclient);
				outToClient.println(fromclient.toUpperCase());	
			}
		}
	}
}
