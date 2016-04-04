# UDP P2P echo program

import Queue
import socket
import sys
import thread

running = 1
if len(sys.argv) <= 1:
    print 'Usage: "python UDPpeer.py local_ip local_port peer_ip peer_port"'
    print "local_ip = IP address of this peer"
    print "local_port = UDP port of this peer"
    print "peer_ip = optional IP address of a known peer"
    print "peer_port = optional UDP port of a known peer"
    sys.exit(2)

# Create a UDP server socket: (AF_INET for IPv4 protocols, SOCK_DGRAM for UDP).
peerSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
peerSocket.settimeout(0.1)
peerSocket.bind((sys.argv[1], int(sys.argv[2])))

# Set of all known peers.
knownPeers = set()
# Thread safe queue from main thread to network thread
sendQueue = Queue.Queue()

def networkThread(peerSocket, knownPeers, sendQueue):
    while running:
        # Try to get a message from the main thread if there is one.
        try:
            message = sendQueue.get_nowait()
        except Queue.Empty:
            message = None

        # Only send if there was a message from the main thread.
        if message:
            for peer in knownPeers:
                peerSocket.sendto(message, peer)

        # Try to receive a message from the network.
        try:
            recvMessage, peerAddress  = peerSocket.recvfrom(2048)
            print 'Received message:', recvMessage, 'From:', peerAddress

            # If the message is already all upper case, it is probably one we
            # sent so don't send it again.
            if recvMessage != recvMessage.upper():
                peerSocket.sendto(recvMessage.upper(), peerAddress)

            # Add the peer to the known peer set.
            knownPeers.add(peerAddress)
        except socket.timeout:
            pass

    peerSocket.close()

# If a peer was supplied, add it to the known peers.
if len(sys.argv) == 5:
    knownPeers.add((sys.argv[3], int(sys.argv[4])))

thread.start_new_thread(networkThread, (peerSocket, knownPeers, sendQueue))

while True:
        message = raw_input("Type a message: ")
        if message == "quit" or message == "shutdown":
            print 'Peer exiting.'
            running = 0
            break

        sendQueue.put(message)

sys.exit(0)
