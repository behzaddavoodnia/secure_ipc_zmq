#include "zmq_basic.c"


int main(int argc, char** argv){

    void* my_server_socket;
    char* ip = "127.0.0.1"; // you can set your desirable ip to whitelist or blacklist
    char* access_control = "ALLOW"; // you can set "ALLOW" to whitelist the ip  or "DENY" to put it on blacklist
    char* recv_msg = "";
    
    zactor_t* proto = init_secure_protocol(access_control, "VERBOSE", ip);
      
    zcert_t* server_cert = init_secure_server_prog("p1");

    void* socket = create_and_bind_server_socket (my_server_socket, server_cert);

    send_secure_message(socket, "Hello");
    
    recv_secure_message(socket, recv_msg);
        
    destroy_cert (server_cert);

    destroy_secure_protocol (proto);
   
    return 0;
}
