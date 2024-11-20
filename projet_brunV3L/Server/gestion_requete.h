#define TAILLEBUF 80
#define BUFFER_SIZE 1024

int list_users(int socket_service);
int verify_user(int socket_service);
void gererClient(int sock_client);
int del_memory();