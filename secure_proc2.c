#include "zmq_basic.c"


int main(int argc, char** argv){


    char* server_prog_name = "p1"; // we should know the (sender or server) public name (p1) and cert
    char* ip = "127.0.0.1"; // you can set your desirable ip to whitelist or blacklist
    char* access_control = "ALLOW"; // you can set "ALLOW" to whitelist the ip  or "DENY" to put it on blacklist
    void* my_client_socket;
    char* recv_msg = "";

    zactor_t* proto = init_secure_protocol (access_control, "VERBOSE", ip);
    
    zcert_t* client_cert = init_secure_client_prog("p2");
    
    const char* server_key =  get_server_key_in_client(server_prog_name);
    
    void* socket = create_and_connect_client_socket (my_client_socket, client_cert, server_key, ip);

    recv_secure_message(socket, recv_msg);
    
    send_secure_message(socket, "World!");
    
    destroy_cert (client_cert);

    destroy_secure_protocol (proto);
    
    
    return 0;
}

