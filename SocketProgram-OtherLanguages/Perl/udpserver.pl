#udpserver.pl

use IO::Socket::INET;

#Turn on System variable for Buffering output
$| = 1;

if ($#ARGV != 0)
{
    print "usage: udpserver port#\n";
    exit;
}

$port=$ARGV[0];

# Creating a new listening socket
$recv_socket=new IO::Socket::INET->new(
				LocalPort=>$port,
				Proto=>'udp'
);

# Create a new sending socket
$send_socket=new IO::Socket::INET->new(
				PeerAddr=>'134.88.53.54', #eng-svr-1
				PeerPort=>$port,
				Proto=>'udp'
);

print "\nUDPServer Waiting for client on port " . $port;

while(1)
{
	$recv_data = "";
	$recv_socket->recv($recv_data,1024);
	$peer_address = $recv_socket->peerhost();
	$peer_port = $recv_socket->peerport();
	print "\n($peer_address , $peer_port) said: $recv_data";
        $send_data = uc $recv_data;
	$send_socket->send($send_data);
}
