import socket
import time

# Configuration
target_ip = "192.168.0.104"                     #change to your esp32 ip
target_port = 80                                #change to your esp32 port

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    client_socket.connect((target_ip, target_port))
    print(f"Connected to {target_ip}:{target_port}")

    while True:
        message = input("Enter the message to send (or type 'exit' to quit): ")
        if message.lower() == 'exit':
            print("Exiting the loop.")
            break
        start_time = time.perf_counter()
        
        client_socket.sendall((message+ "\n").encode()) #add \n to the end of message, so that esp32 can read it as a line

        while True:
            response = client_socket.recv(1024)
            print(len(response))
            if len(response) == 2: #bugfix always return 2 empty bytes, wait for real respond, do not use 2 char length as respond!
                continue
            if response:
                break
        end_time = time.perf_counter()
        round_trip_time_ms = (end_time - start_time) * 1000

        print(f"Response from server: {response.decode()}")
        print(f"Round-trip time: {round_trip_time_ms:.3f} ms")

finally:
    client_socket.close()