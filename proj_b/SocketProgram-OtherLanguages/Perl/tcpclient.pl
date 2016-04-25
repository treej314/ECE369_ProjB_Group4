#tcpclient.pl

use IO::Socket;

if ($#ARGV != 0)
{
    print "usage: tcpclient port#\n";
    exit;
}

$port=$ARGV[0];

$socket = new IO::Socket::INET (
                                  PeerAddr  => 'eng-svr-2',
                                  PeerPort  =>  $port,
                                  Proto => 'tcp',
                               )                
or die "Couldn't connect to Server\n";                                                                                                                    
while (1)
{	    
    $recv_data = "";
    print "SEND: ";
    $send_data = <STDIN>;
    chop($send_data);
    $socket->send($send_data);
    $socket->recv($recv_data,1024);
    print "RECEIVED: $recv_data\n"; 
}    
    
