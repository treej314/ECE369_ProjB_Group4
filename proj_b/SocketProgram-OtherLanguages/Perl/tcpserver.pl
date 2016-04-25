#tcpserver.pl

use IO::Socket;

if ($#ARGV != 0)
{
    print "usage: tcpserver port#\n";
    exit;
}

$port=$ARGV[0];

$| = 1;

$socket = new IO::Socket::INET (
                                  LocalHost => '134.88.53.55', #doesn't do dns lookup
                                  LocalPort => $port,
                                  Proto => 'tcp',
                                  Listen => 5,
                                  Reuse => 1
                               );
                                
die "Coudn't open socket" unless $socket;

print "\nTCPServer Waiting for client on port " . $port;

while(1)
{
	$client_socket = "";
	$client_socket = $socket->accept();
	
	$peer_address = $client_socket->peerhost();
	$peer_port = $client_socket->peerport();
	
	print "\n( $peer_address , $peer_port ) connected.";
	
	 while (1) 
	 {
	     $client_socket->recv($recv_data,1024);
	     print "\n Received: $recv_data";
	     $send_data = uc $recv_data;
	     $client_socket->send ($send_data);
	}
}
                                
