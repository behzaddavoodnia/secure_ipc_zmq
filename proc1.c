#include "zmq_basic.c"


int main(){

system("mkdir "SOCKET_DIR);
    prog_name_t* p1 = (prog_name_t*) calloc (100, sizeof(prog_name_t));
    init_prog(p1);
    init_socket(p1);
    create_out_socket(p1, "ipc");
    create_out_socket(p1, "tcp");
    p1->message = strdup("This is a test message!");
    send_message(p1, p1->message, strlen(p1->message), false);
    
    term_socket(p1);
    safe_free(p1);

    return 0;
}
