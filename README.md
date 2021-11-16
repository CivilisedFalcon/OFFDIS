# SWCapstoneDesign

### Authentication System
List of relevant files:
- create_user.c : used to create new username and password
- login.c : used to authenticate existing users
- user_auth.txt: used to store user credentials

Generating executables and executing them: 
```
gcc <filename.c> -o <executable_name>
./<executable_name>
```

Note: user_auth.txt should be present in the same directory as the executables for this code to function properly.

### Message transmission system
List of relevant files:
- messaging.c: used to send messages to destinations(txt file based destination, It will be changed to IP destination later)
- user1(~3).txt: use it as a test destination to send a message

### Wireless Access
List of relevant files:
- detect_connection.c
