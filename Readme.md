# A library for standard and secure IPC based on ZMQ sockets

* You can use simple APIs like send_message() or send_secure_message() to sending information between two process or program.

* We've used the ZMQ DEALER socket to establish the bidirectional connection to sending and receiving messages.

* For build and run you need libmsgpack-dev, libzmq-dev, libczmq-dev, and curve library.

* This app uses CURVE security and ZAP as a communication protocol and also it uses Public/ Private key mechanisms to exchange messages.

* To run in an appropriate manner you need to feed the sender (or server) key to the receiver (or client) process. 

* IMPORTANT: You need first to run [./secure_proc1] and then [./secure_proc2] to make and produce the pub key and certificates into .curve and .cert directories, respectively.

* DO NOT EDIT the keys and certificates in the .curve and .cert directories.

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
