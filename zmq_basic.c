#include "zmq_basic.h"

static void init_prog(prog_name_t* prog){
    
    prog->tcp_socket_addr =     TCP_SOCKET;
    prog->ipc_socket_addr =     IPC_SOCKET;
    prog->inproc_socket_addr =  INPROC_SOCKET;
    prog->ready = true;
    prog->id = 0;
    prog->message = strdup("Hello!");
    prog->name = strdup("process0");
    prog->contex = NULL;

}



static int init_socket(prog_name_t* prog){

    prog->contex = zmq_ctx_new();
    if(prog->contex == NULL){
        printf ("init_socket on contex creation error: %s\n", zmq_strerror(errno));
        return -1;
    }
    return 0;

}

static int create_out_socket(prog_name_t* prog, char* connection_mode){

    prog->socket = zmq_socket(prog->contex, ZMQ_DEALER);

    if (prog->socket == NULL){
        printf ("create out zmq_socket error: %s\n", zmq_strerror(errno));
        return -1;
    }
    if (!strcmp(connection_mode, "ipc")){
        if (zmq_connect(prog->socket, prog->ipc_socket_addr) <0){
            printf ("create ipc connect socket error: %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    if (!strcmp(connection_mode, "inproc")){
        if(zmq_connect(prog->socket, prog->inproc_socket_addr) < 0){
            printf ("create inproc connect socket error: %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    if (!strcmp(connection_mode, "tcp")){
        if (zmq_connect(prog->socket, prog->tcp_socket_addr) <0){
            printf ("create tcp connect socket error: %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    return 0;

}

static int create_in_socket(prog_name_t* prog, char* connection_mode){

    prog->socket = zmq_socket(prog->contex, ZMQ_DEALER);
    
    if (prog->socket == NULL){
        printf ("create in zmq_socket error: %s\n", zmq_strerror(errno));
        return -1;
    }
    if (!strcmp(connection_mode, "ipc")){
        if (zmq_bind(prog->socket, prog->ipc_socket_addr) < 0){
            printf ("create ipc bind socket error: %s\n", zmq_strerror(errno));
            return -1;
        }    
    }

    if (!strcmp(connection_mode, "inproc")){
        if(zmq_bind(prog->socket, prog->inproc_socket_addr) < 0){
            printf ("create inproc bind socket error: %s\n", zmq_strerror(errno));
            return -1;}
    }

    if (!strcmp(connection_mode, "tcp")){
        if (zmq_bind(prog->socket, prog->tcp_socket_addr) < 0){
            printf ("create tcp bind socket error: %s\n", zmq_strerror(errno));
             return -1;
        }
    }
    return 0;

}


static int destroy_out_socket(prog_name_t* prog, char* connection_mode){

   
    if (prog->socket == NULL){
        printf ("destroy zmq out-socket error: %s\n", zmq_strerror(errno));
        return -1;
    }
    if (!strcmp(connection_mode, "ipc")){
        if (zmq_disconnect(prog->socket, prog->ipc_socket_addr) <0){
            printf ("destroy ipc connect socket error: %s\n", zmq_strerror(errno));
            return -1;        
        }
    }
    if (!strcmp(connection_mode, "inproc")){
        if(zmq_disconnect(prog->socket, prog->inproc_socket_addr) < 0){
            printf ("destroy inproc connect socket error: %s\n", zmq_strerror(errno));
            return -1;        
        }
    }
    if (!strcmp(connection_mode, "tcp")){
        if (zmq_disconnect(prog->socket, prog->tcp_socket_addr) <0){
            printf ("destroy tcp connect socket error: %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    return 0;

}

static int destroy_in_socket(prog_name_t* prog, char* connection_mode){


    if (prog->socket == NULL){
        printf ("destroy zmq in_socket error: %s\n", zmq_strerror(errno));
        return -1;
    }
    if (!strcmp(connection_mode, "ipc")){
        if (zmq_unbind(prog->socket, prog->ipc_socket_addr) < 0){
            printf ("destroy ipc bind socket error: %s\n", zmq_strerror(errno));
            return -1;        
        }
    }
    if (!strcmp(connection_mode, "inproc")){
        if(zmq_unbind(prog->socket, prog->inproc_socket_addr) < 0){
            printf ("destroy inproc bind socket error: %s\n", zmq_strerror(errno));
            return -1;        
        }
    }
    if (!strcmp(connection_mode, "tcp")){
        if (zmq_unbind(prog->socket, prog->tcp_socket_addr) < 0){
            printf ("destroy tcp bind socket error: %s\n", zmq_strerror(errno));
            return -1;
        }
    }

    return 0;

}


static int send_message(prog_name_t* prog, char* msg, int msg_len, bool multi_part_msg){

    int flag = -1;

    if (multi_part_msg)
        flag = ZMQ_SNDMORE;
    else
        flag = 0;

    if (zmq_send(prog->socket, msg, msg_len, flag) < 0){
        printf ("send error: %s\n", zmq_strerror(errno));
        return -1;
    }
    printf("Sent message: %s\n", msg);

    return 0;
}

static int recv_message(prog_name_t* prog, char* buf, int msg_len, int flag){
   
    int _flag = -1;
    
    if (flag == 0)  _flag = 0;
    else if (flag == 1)  _flag = ZMQ_RCVMORE;
    else if (flag == 2)  _flag = ZMQ_NOBLOCK;
    else{
        perror("Invalid flag argument!\n");
        exit(EXIT_FAILURE);
    }
        

    if (zmq_recv(prog->socket, buf, msg_len, flag) < 0){
        printf ("recv error: %s\n", zmq_strerror(errno));
        return -1;
    }
    
    printf("Received message: %s\n", buf);
    return 0;
    
}

static int term_socket(prog_name_t* prog){
    
    zmq_close(prog->socket);
    int term = zmq_ctx_destroy (prog->contex);
    if(term < 0)
        printf ("socket contex termination error: %s\n", zmq_strerror(errno));

    
    return 0;
}

static zactor_t* init_secure_protocol(const char* access_ctrl, const char* proto_type, const char* ip){
    
    zactor_t* auth = zactor_new (zauth, NULL);
    assert (auth);
    zstr_send  (auth, proto_type);
    zstr_sendx (auth, access_ctrl, ip, NULL);
    zstr_sendx (auth, "CURVE", ".curve", NULL);
    assert (zsys_dir_create (".curve") == 0);
    assert (zsys_dir_create (".cert") == 0);
    assert (auth);

    return auth;
}



static zcert_t* init_secure_server_prog(char* name){
    
    char* sn = "";
    asprintf(&sn, ".cert/%s_cert.txt", name);
    
    zcert_t* server_cert = zcert_new ();
    zcert_set_meta (server_cert, "name", "Server certificate");
    zcert_save (server_cert, sn);
    server_cert = zcert_load (sn);
    
    return server_cert;

}

static zcert_t* init_secure_client_prog(char* name){
    
    char* sn = "";
    asprintf(&sn, ".cert/%s_cert.txt", name);
    
    zcert_t* client_cert = zcert_new ();
    zcert_set_meta (client_cert, "name", "Client certificate");
    zcert_save_public (client_cert, ".curve/client_cert.pub");
    zcert_save (client_cert, sn);
    client_cert = zcert_load (sn);
    
    return client_cert;

}

static char* get_server_key_in_client(char* name){

    char* sn = "";
    asprintf(&sn, ".cert/%s_cert.txt", name);

    // Load server cert and crate server key
    const char* server_key = zcert_public_txt (zcert_load (sn));
    assert(server_key);

    return server_key;
}

static void* create_and_bind_server_socket (void* server, zcert_t* server_cert){

    //  Create and bind server socket
    server = zsock_new (ZMQ_DEALER);
    assert(server);
    zcert_apply (server_cert, server);
    zsock_set_curve_server (server, 1);
    zsock_bind (server, "tcp://*:9000");
    
    return server;
}


static void* create_and_connect_client_socket (void* client, zcert_t* client_cert, const char* server_key, char* ip){

    client = zsock_new (ZMQ_DEALER);
    assert(client);
    zcert_apply (client_cert, client);
    zsock_set_curve_serverkey (client, server_key);

    char* sn = "";
    asprintf(&sn, "tcp://%s:9000", ip);

    zsock_connect (client, sn);
  

    return client;
}


static int send_secure_message(void* socket, char* msg){

    printf("\nSend secure message:[%s]\n\n", msg);
    int rc = zstr_send(socket, msg);
    if(rc >= 0)
        return 1;
    else{
        fprintf(stderr, "Error on sending message\n");
        return -1;
        }
}

static int recv_secure_message(void* socket, char* buf){
        
        buf = zstr_recv (socket);
        if (buf != NULL) {
            printf("\nReceived secure message:[%s]\n\n", buf);
            return 1;
            }
        
        else {
            fprintf(stderr, "Error on receveing message\n");
            return -1;
            }
}

static void destroy_secure_protocol(zactor_t* auth){

    
    zactor_destroy (&auth);

}

static void destroy_cert(zcert_t* cert){

    zcert_destroy (&cert);

}

