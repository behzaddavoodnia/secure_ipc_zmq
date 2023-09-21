#include "zmq_basic.c"

int main(){
    
system("mkdir "SOCKET_DIR);
    prog_name_t* p2 = (prog_name_t*) calloc (100, sizeof(prog_name_t));
    char buffer [50] = "";
    init_prog(p2);
    init_socket(p2);
    create_in_socket(p2, "ipc");
    create_in_socket(p2, "tcp");
    recv_message(p2, buffer, sizeof buffer, 0);
    
    term_socket(p2);
    safe_free(p2);

    return 0;
}
