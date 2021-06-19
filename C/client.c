#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MYBUFSIZE 1024

int main(int argc, char *argv[])
{
    // test argomenti
    if (argc != 3) {
        // errore di chiamata
        printf("%s: numero argomenti errato\n", argv[0]);
        printf("uso: %s host port [e.g.: %s 127.0.0.1 9999]\n", argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    // creo il socket in modo internet/TCP
    int cli_sock;
    if ((cli_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        // errore socket()
        printf("%s: non posso creare il socket (%s)\n", argv[0], strerror(errno));
        return EXIT_FAILURE;
    }

    // preparo la struttura sockaddr_in per il server remoto
    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;                    // set famiglia di indirizzi
    server.sin_addr.s_addr = inet_addr(argv[1]);    // set indirizzo del server
    server.sin_port = htons(atoi(argv[2]));         // set port del server

    // mi connetto al server remoto
    if (connect(cli_sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        // errore connect()
        printf("%s: errore connect (%s)\n", argv[0], strerror(errno));
        close(cli_sock);
        return EXIT_FAILURE;
    }

    // loop di comunicazione col server remoto
    for (;;) {
        // compongo un messaggio per il server remoto
        char my_msg[MYBUFSIZE];
        printf("Scrivi un messaggio per il Server remoto: ");
        scanf("%s", my_msg);

        // invio il messaggio al server remoto
        if (send(cli_sock, my_msg, strlen(my_msg), 0) == -1) {
            // errore send()
            printf("%s: errore send (%s)\n", argv[0], strerror(errno));
            close(cli_sock);
            return EXIT_FAILURE;
        }

        // ricevo una risposta dal server remoto
        memset(my_msg, 0, MYBUFSIZE);
        if (recv(cli_sock, my_msg, MYBUFSIZE, 0) == -1) {
            // errore recv()
            printf("%s: errore recv (%s)\n", argv[0], strerror(errno));
            close(cli_sock);
            return EXIT_FAILURE;
        }

        // mostro la risposta
        printf("%s: il server risponde: %s\n", argv[0], my_msg);
    }

    // esco con Ok
    return EXIT_SUCCESS;
}
