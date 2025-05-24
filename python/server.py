import socket

# Server configuration
HOST = '127.0.0.1'  # Localhost
PORT = 5000         # Port to listen on

# Create a TCP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen()

print(f"Server listening on {HOST}:{PORT}...")
conn, addr = server_socket.accept()
print(f"Connected by {addr}")

# Chat loop
while True:
    # Receive message from client
    client_msg = conn.recv(1024).decode()
    if client_msg.lower() == 'exit':
        print("Client disconnected.")
        break
    print(f"Client: {client_msg}")

    # Send response to client
    server_msg = input("You: ")
    conn.send(server_msg.encode())
    if server_msg.lower() == 'exit':
        print("Connection closed.")
        break

conn.close()
