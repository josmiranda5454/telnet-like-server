# telnet-like-server
Write a simple telnet-like server that requires a login/password. Once logged in, the server should echo text back to the user. Create a secret backdoor into your server, the more sneaky/subtle the better.

## Build project
- ```cd <project_source>```
- ```make all```

### Runnning
- In one terminal run ```sudo ./server 23```
- In another terminal run ```sudo ./client```

#### References
1. [Introduction to socket programming](https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf)
2. [Sockets Tutorial](http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html)
3. [TCP/IP Sockets in C](http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html)
