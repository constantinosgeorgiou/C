#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// const uint16_t PORT = 9090;
const char *PORT = "9090";

int main(int argc, char const *argv[]) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;
  hints.ai_flags = 0;

  // node can be either a numerical network
  // address or network host name.
  // hints.ai_flags &= ~AI_NUMERICHOST;

  // Make returned socket addresses suitable for binding a socket that will
  // accept connections.
  hints.ai_flags |= AI_PASSIVE;

  // If system is IPv4 only ensure that no IPv6 socket addresses are returned
  // that would always fail in connect() or bind(). Likewise for IPv6 only
  // systems.
  // hints.ai_flags |= AI_ADDRCONFIG;

  hints.ai_addr = NULL;
  hints.ai_addrlen = 0;
  hints.ai_canonname = NULL;
  hints.ai_next = NULL;

  printf("here\n");

  // TODO finish hints struct setup
  struct addrinfo *potential_addr;
  int error = getaddrinfo(NULL, PORT, &hints, &potential_addr);
  if (error != 0) {
    fprintf(stderr, "getaddringo: %s\n", gai_strerror(error));
    return 1;
  }
  int server_socket = -1;
  struct addrinfo *server_addr = NULL;
  for (server_addr = potential_addr; server_addr != NULL;
       server_addr = server_addr->ai_next) {
    server_socket = socket(        //
        server_addr->ai_family,    //
        server_addr->ai_socktype,  //
        server_addr->ai_protocol   //
    );

    if (server_socket == -1) continue;

    if (bind(                        //
            server_socket,           //
            server_addr->ai_addr,    //
            server_addr->ai_addrlen  //
            ) == 0) {
      break;
    }

    if (close(server_socket) == -1) {
      perror("close");
      return 1;
    }
  }
  freeaddrinfo(potential_addr);
  if (server_addr == NULL) {
    perror("Could not bind\n");
    return 1;
  }

  // Find server host and service
  struct sockaddr *addr;
  socklen_t addrlen;
  char host_buffer[NI_MAXHOST];
  char service_buffer[NI_MAXSERV];
  error = 0;
  error = getnameinfo(                         //
      addr, addrlen,                           //
      host_buffer, sizeof(host_buffer),        //
      service_buffer, sizeof(service_buffer),  //
      NI_NUMERICHOST | NI_NUMERICSERV);
  if (error != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
    return 1;
  }

  printf("host=%s, serv=%s\n", host_buffer, service_buffer);

  return 0;
}
