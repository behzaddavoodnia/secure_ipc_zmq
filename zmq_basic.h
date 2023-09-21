#ifndef ZMQ_BASIC_H
#define ZMQ_BASIC_H

/* UDP is not supported by 0MQ. Also, IPC is only supported on OSes which
 have a POSIX-conforming implementation of named pipes; so, on Windows,
 you can really only use 'inproc' or 'tcp' for endpoints.
*/

/*ZMQ_REQ does not throw away any messages. If there are no available services to send
the message or if the all services are busy, all send operations i.e. zmq_send(), are
blocked until a service becomes available to send the message*/


// System headers
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <msgpack.h>
#include <zmq.h>
#include <czmq.h>
#include <sodium.h>
#include <pthread.h>
#include <unistd.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <sys/stat.h>
#include <sys/time.h>


// Deallocating a memory pointed by ptr causes dangling and non-wild pointer
#ifndef safe_free
#define safe_free(ptr) ({free(ptr); ptr=NULL;})
#endif
 

// +----------------------- Socket Paths ------------------------+ //

#define SOCKET_DIR      		"/tmp/feeds/"
#define IPC_SOCKET			"ipc:///tmp/feeds/0"
#define IPC_SOCKET_INPUT		"ipc://%s"
#define TCP_SOCKET		        "tcp://0.0.0.0:5555"
#define TCP_SOCKET_INPUT	        "tcp://%s:%s"
#define TCP_SOCKET_ALL	            	"tcp://*:5555"
#define INPROC_SOCKET               	"inproc://my-inproc-endpoint"
#define INPROC_SOCKET_INPUT		"inproc://%s"

#define MAX_SIZE_OF_NAME            	256


typedef struct _prog_name_t
{
    char* name;
    char* message;
    char* ipc_socket_addr;
    char* inproc_socket_addr;
    char* tcp_socket_addr;
    unsigned id;
    void* contex;
    void* socket;
    bool ready;
    
}prog_name_t;




static void init_prog(prog_name_t* prog);

static int init_socket(prog_name_t* prog);


static int create_out_socket(prog_name_t* prog, char* connection_mode);

static int create_in_socket(prog_name_t* prog, char* connection_mode);

static int destroy_out_socket(prog_name_t* prog, char* connection_mode);

static int destroy_in_socket(prog_name_t* prog, char* connection_mode);

static int send_message(prog_name_t* prog, char* msg, int msg_len, bool multi_part_msg);

static int recv_message(prog_name_t* prog, char* msg, int msg_len, int flag);

static int term_socket(prog_name_t* prog);

static void client_task ();

static void server_task (zcert_t* args);

static zactor_t* init_secure_protocol(const char* access_ctrl, const char* proto_type, const char* ip);

static zcert_t* init_secure_server_prog (char* name);

static zcert_t* init_secure_client_prog (char* name);

static char* get_server_key_in_client(char* name);

static void* create_and_connect_client_socket (void* client, zcert_t* client_cert, const char* server_key, char* ip);

static void* create_and_bind_server_socket (void* server, zcert_t* server_cert);

static int send_secure_message(void* socket, char* msg);

static int recv_secure_message(void* socket, char* buf);

static void destroy_secure_protocol(zactor_t* auth);

static void destroy_cert(zcert_t* cert);


#endif // !ZMQ_BASIC_H
