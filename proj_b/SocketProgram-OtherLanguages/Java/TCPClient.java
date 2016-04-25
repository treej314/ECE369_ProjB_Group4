//TCPClient.java

import java.io.*;
import java.net.*;

class TCPClient 
{
	public static void main(String argv[]) throws Exception
	{
		String fromServer;
		String toServer;
		int port = 9092;

		if (argv.length == 0)
		{
		    System.out.println("Incorrect command line arguments. Expected port");
		    return;
                }

		port = Integer.parseInt(argv[0]);

		Socket clientSocket = new Socket("localhost", port);

		BufferedReader inFromUser =
			new BufferedReader(new InputStreamReader(System.in));
		
		PrintWriter outtoServer = new PrintWriter(
			clientSocket.getOutputStream(),true);
		
		BufferedReader infromServer = new BufferedReader(new InputStreamReader(
			clientSocket.getInputStream()));
		
		while (true)
		{    
			System.out.print("Send: ");        
			toServer = inFromUser.readLine();      
			outtoServer.println(toServer);
			fromServer = infromServer.readLine();
			System.out.println("Received: " + fromServer);
		}

	}
}
