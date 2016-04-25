# UDP P2P echo program

import Queue
import socket
import sys
import thread

running = 1
if len(sys.argv) <= 1:
    print 'Usage: "python TCPpeer.py local_ip local_port peer_ip peer_port"'
    print "local_ip = IP address of this peer"
    print "local_port = UDP port of this peer"
    print "peer_ip = optional IP address of a known peer"
    print "peer_port = optional UDP port of a known peer"
    sys.exit(2)

# Create a TCP server socket: (AF_INET for IPv4 protocols, SOCK_STREAM for TCP).
try:
    serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serverSocket.bind((sys.argv[1], int(sys.argv[2])))
    serverSocket.listen(5)
except socket.error:
    if serverSocket:
        serverSocket.close()
    sys.exit(1)

# Set of all known peers.
knownPeers = set()
peerLock = thread.allocate_lock()
# List of queues from main thread to network threads.
sendQueueList = []
sendQueueLock = thread.allocate_lock()

def networkThread(peerSocket, peerAddress, sendQueue):
    while True:
        # Try to get a message from the main thread if there is one.
        try:
            message = sendQueue.get_nowait()
        except Queue.Empty:
            message = None

        # Only send if there was a message from the main thread.
        if message:
            peerSocket.send(message)

        # Try to receive a message from the network.
        try:
            recvMessage = peerSocket.recv(2048)
            # If no data was received, the remote end has closed.
            if len(recvMessage) == 0:
                break
            print 'Received message:', recvMessage, 'From:', peerAddress

            # If the message is already all upper case, it is probably one we
            # sent so don't send it again.
            if recvMessage != recvMessage.upper():
                peerSocket.send(recvMessage.upper())
        except socket.timeout:
            pass
        except socket.error:
            break

    peerSocket.close()
    with peerLock:
        knownPeers.discard(peerSocket)
    with sendQueueLock:
        sendQueueList.remove(sendQueue)

def listenerThread(serverSocket, knownPeers, sendQueueList):
    while running:
        peerSocket, peerAddress = serverSocket.accept()
        peerSocket.settimeout(0.1)
        with peerLock:
            knownPeers.add(peerSocket)
        with sendQueueLock:
            sendQueue = Queue.Queue()
            sendQueueList.append(sendQueue)

        thread.start_new_thread(networkThread, (peerSocket,
                                                peerAddress,
                                                sendQueue))

    serverSocket.shutdown(0)
    serverSocket.close()

# If a peer was supplied, add it to the known peers.
if len(sys.argv) == 5:
    initPeerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        initPeerSocket.connect((sys.argv[3], int(sys.argv[4])))
        initPeerSocket.settimeout(0.1)
        with peerLock:
            knownPeers.add(initPeerSocket)
        with sendQueueLock:
            sendQueue = Queue.Queue()
            sendQueueList.append(sendQueue)
        thread.start_new_thread(networkThread, (initPeerSocket,
                                                (sys.argv[3], int(sys.argv[4])),
                                                sendQueue))
    except socket.error:
        pass

thread.start_new_thread(listenerThread, (serverSocket,
                                         knownPeers,
                                         sendQueueList))

while True:
        message = raw_input("Type a message: ")
        if message == "quit" or message == "shutdown":
            print 'Peer exiting.'
            running = 0
            break

        with sendQueueLock:
            for sendQueue in sendQueueList:
                sendQueue.put(message)

sys.exit(0)
