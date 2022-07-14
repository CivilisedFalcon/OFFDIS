# OFFDIS: A Backend Framework for an Offline Discord-like Communication Platform in C

### Final Backend System


#### List of relevant files (Final Offline System):
- final_integration_server.c : Server for the Authentication amd messaging system.
- final_integration_client.c : Client for messenging. Requires Username and Password to start chatting.
- user_auth.txt: used to store user credentials

Generating executables and executing them: 
```
gcc -pthread final_integration_server.c -o server
gcc -pthread final_integration_client.c -o client

./server <port> <server password>
./client <IP> <port>
```
Command(client):
```
> (No parameter)<message>
> SEND <DestIP> <DestPort> <filename>
> VOTE#<Vote message> (not complete)
```

### Authentication System

#### List of relevant files (Basic Authentication standalone code):
- create_user.c : used to create new username and password
- login.c : used to authenticate existing users
- user_auth.txt: used to store user credentials

Generating executables and executing them: 
```
gcc <filename.c> -o <executable_name>
./<executable_name>
```
Note: user_auth.txt should be present in the same directory as the executables for this code to function properly.


#### List of relevant files (Client and Server Authentication and Messenging):
- auth&messenging_server.c : Server for the Authentication amd messaging system.
- auth&messenging_client.c : Client for messenging. Requires Username and Password to start chatting.
- user_auth.txt: used to store user credentials

Generating executables and executing them: 
```
gcc -pthread auth_messenging_server.c -o server
gcc -pthread auth_messenging_client.c -o client

./server <port> <server password>
./client <IP> <port>
```

### Message transmission system
List of relevant files:
- messaging.c: used to send messages to destinations(txt file based destination, It will be changed to IP destination later)
- user1(~3).txt: use it as a test destination to send a message
- log_chattingroom_server.c: server source with make log file(chatting.log, current_time / from / message)
- log_chattingroom_client.c: client works with server

### Wireless Access
List of relevant files:
- detect_connection.c

### File Send

#### Execute
```
gcc filesend_server.c -o <S_name>
gcc filesend_client.c -o <C_name>
```
Run `./<S_name> <PORT>` first and then run `./<C_name> <SERVER_ADDR> <PORT>`  
Then, you can check the IP and PORT of connected user with log `<IP>:<PORT>  "<UserName>" has joined` in server  
To send file, type `SEND <IP> <PORT> <FileName>` in client.  
Receiver receive file `<FileName>`  

### Voting System
Generating executables and executing them: 
```
gcc <filename.c> -o <executable_name>
./<executable_name>

#vote <voting name>
<voting name>
1)YES
2)NO
3)NONE
1
0
#result
```
- vote.txt : One file continues to be overwritten.
