//UDPClient.java

import java.net.*;
import java.io.*;

class UDPClient 
{
	public static void main(String argv[]) throws Exception
	{
		
		byte[] send_data = new byte[1024];
		int port = 9092;

		if (argv.length == 0)
		{
		    System.out.println("Incorrect command line arguments. Expected port");
		    return;
                }

		port = Integer.parseInt(argv[0]);

		System.out.println("Connecting to UDPServer over port: " + port);
		
		BufferedReader infromuser = 
			new BufferedReader(new InputStreamReader(System.in));
		DatagramSocket client_socket = new DatagramSocket();
		InetAddress IPAddress =  InetAddress.getByName("localhost");
		
		while (true)
		{
			System.out.print("Send: ");
			
			send_data = infromuser.readLine().getBytes();
			
			DatagramPacket send_packet = new DatagramPacket(send_data,
				send_data.length, IPAddress, port);
			
			client_socket.send(send_packet);
		}       
	}
}
