//UDPServer.java

import java.net.*;

class UDPServer 
{
	public static void main(String argv[]) throws Exception
	{
		byte[] receive_data = new byte[1024];
		int recv_port;
		int port = 9092;

		if (argv.length == 0)
		{
		    System.out.println("Incorrect command line arguments. Expected port");
		    return;
                }

		port = Integer.parseInt(argv[0]);
		
		DatagramSocket server_socket = new DatagramSocket(port);
		
		System.out.println ("UDPServer Waiting for client on port" + port);
		
		while(true)
		{
			
			DatagramPacket receive_packet = new DatagramPacket(receive_data,
				receive_data.length);
			
			server_socket.receive(receive_packet);

			InetAddress IPAddress = receive_packet.getAddress();
			
			recv_port = receive_packet.getPort();
			
			System.out.println("( " + IPAddress + " , " + recv_port 
				+ " ) said: " + new String(receive_packet.getData() ));
		}
	}
}
