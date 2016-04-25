#udpclient.pl

use IO::Socket::INET;

if ($#ARGV != 0)
{
    print "usage: udpclient port#\n";
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
				PeerAddr=>'134.88.53.55', #eng-svr-2
				PeerPort=>$port,
				Proto=>'udp'
);

while(1)
{
	$recv_data = "";
	print "SEND: ";
	$send_data=<STDIN>;
	chop $message; # Removes last char
	$send_socket->send($send_data);
	$recv_socket->recv($recv_data,1024);
	print "RECEIVED: $recv_data\n";
}
