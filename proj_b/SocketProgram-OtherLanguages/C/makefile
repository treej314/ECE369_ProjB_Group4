all: udpclient udpserver tcpclient tcpserver

udpclient: 
	gcc src/udpclient.c -o udpclient

udpserver: 
	gcc src/udpserver.c -o udpserver

tcpclient: 
	gcc src/tcpclient.c -o tcpclient

tcpserver: 
	gcc src/tcpserver.c -o tcpserver

clean:
	rm -f *~ src/*~ tcpclient tcpserver udpclient udpserver
