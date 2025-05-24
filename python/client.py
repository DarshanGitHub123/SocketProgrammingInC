
import socket

# Server configuration
HOST = '127.0.0.1'  # Localhost
PORT = 5000         # Must match server port

# Create a TCP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((HOST, PORT))

print("Connected to the server. Type 'exit' to disconnect.")

# Chat loop
while True:
    # Send message to server
    msg = input("You: ")
    client_socket.send(msg.encode())
    if msg.lower() == 'exit':
        break

    # Receive response from server
    reply = client_socket.recv(1024).decode()
    if reply.lower() == 'exit':
        print("Server disconnected.")
        break
    print(f"Server: {reply}")

client_socket.close()
