package main

import (
    "bufio"
    "fmt"
    "net"
    "os"
)

func main() {
    // test argomenti
    if len(os.Args) != 3 {
        // errore di chiamata
        fmt.Printf("%s: numero argomenti errato\n", os.Args[0])
        fmt.Printf("uso: %s host port [e.g.: %s 127.0.0.1 9999]\n", os.Args[0], os.Args[0])
        return
    }

    // mi connetto al server remoto
    addr := os.Args[1] + ":" + os.Args[2] // set indirizzo (i.e.: "host:port")
    conn, err := net.Dial("tcp", addr)    // set dial con network di tipo TCP
    if err != nil {
        // errore di connessione
        fmt.Println(err)
        return
    }

    // loop di comunicazione col server remoto
    connrdr := bufio.NewReader(conn)      // reader sulla connessione
    stdinrdr := bufio.NewReader(os.Stdin) // reader sullo standard input
    for {
        // compongo un messaggio per il server remoto
        fmt.Print("Scrivi un messaggio per il Server remoto: ")
        client_msg, _ := stdinrdr.ReadString('\n')

        // invio il messaggio al server remoto
        _, err = conn.Write([]byte(client_msg)) // scrivo sulla connessione
        if err != nil {
            // errore di invio
            fmt.Println(err)
            return
        }

        // ricevo una risposta dal server remoto
        server_msg, err := connrdr.ReadString('\n') // leggo con il conn reader
        if err != nil {
            // errore di lettura
            fmt.Println(err)
            return
        }

        // mostro la risposta
        fmt.Printf("%s: il server risponde: %s", os.Args[0], server_msg)
    }
}
