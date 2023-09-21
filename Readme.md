# A library for normal and secure IPC based on ZMQ sockets

* You can use simple APIs like send_message() or send_secure_message() to sending information between two process or program.

* We've used ZMQ DEALER socket to establish the bidirectional connection to sending and receveing messages.

* For build and run you need libmsgpack-dev, libzmq-dev, libczmq-dev and curve library.

* This app uses CURVE security and ZAP as a communication protocol and also it uses Public/ Private key mechanesem to exchange the messages.

* To run in appropriate manner you need to feed sender (or server) key to recevier (or client) process. 

* IMPORTANT: You need to first run [./secure_proc1] then [./secure_proc2] to make and produce the pub key and cetificates into .curve and .cert directories, respectivily.

* DO NOT EDIT the keys and certificates in .curve and .cert directoris.

* There is no priority to run [./proc1] or [./proc2] in normal communication.

## Compile and Run

```
mkdir build && cd build
cmake .. && make
```
### Normal

```
./proc2
./proc1
```
### Secure with Verbose descriptions

```
./secure_proc1
./secure_proc2
```
