// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"net"
	"strings"
	"time"
)

type user struct {
	client   chan<- string
	username string
	ip       string
	admin    bool
	active   bool
	conn     net.Conn
}

var (
	entering  = make(chan user)
	leaving   = make(chan user)
	messages  = make(chan string) // all incoming client messages
	hasAdmin  = false
	firstUser = true
	nameMap   = make(map[string]string)
	kickMap   = make(map[string]bool)
)

func showPrompt(u user) {
	u.client <- (u.username + " > ")
}

func broadcaster() {
	clients := make(map[user]bool) // all connected clients
	msgbool := false

	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			msgArr := strings.Split(msg, " ")
			command := "placeholder"
			command = msgArr[2]

			// Find user sending the message
			var currentUser user
			for cli := range clients {
				if cli.username == msgArr[0][1:] {
					currentUser = cli
					break
				}
			}

			if msgArr[0][1:] == "irc-server" {
				currentUser.active = true
			}

			if len(command) > 0 && command[0] == '/' {
				switch command {

				case "/users":
					userStr := "irc-server > "
					for cli := range clients {
						userStr += (cli.username + ", ")
					}

					currentUser.client <- (userStr[:len(userStr)-2])

				case "/msg":
					if len(msgArr) > 4 && len(msgArr[4]) > 0 {
						found := false

						if currentUser.username != msgArr[3] {
							for cli := range clients {
								if cli.username == msgArr[3] {
									msgbool = true

									cli.client <- ("\n" + currentUser.username + " > " + msg[len(currentUser.username)+len(cli.username)+10:] + "\n")
									showPrompt(cli)
									found = true
									break
								}
							}
							if !found {
								currentUser.client <- "User not found."
							}
						} else {
							currentUser.client <- "Don't send a message to yourself!"
						}
					} else {
						currentUser.client <- "No message written."
					}

				case "/time":
					currentUser.client <- ("irc-server > Local Time: " + time.Now().String()[11:18] + " " + strings.Split(time.Now().String(), " ")[3])

				case "/user":
					if len(msgArr) > 3 && msgArr[3] != "" {
						found := false
						for cli := range clients {
							if cli.username == msgArr[3] {
								currentUser.client <- ("irc-server > username: " + cli.username + ", IP: " + cli.ip)

								found = true
								break
							}
						}
						if !found {
							currentUser.client <- "User not found."
						}
					} else {
						currentUser.client <- "No user written."
					}

				case "/kick":
					if len(msgArr) > 3 && msgArr[3] != "" {
						found := false
						if currentUser.admin {
							if currentUser.username != msgArr[3] {
								for cli := range clients {
									if cli.username == msgArr[3] {
										fmt.Printf("irc-server > [%s] was kicked\n", cli.username)

										fmt.Fprintln(cli.conn, "\nirc-server > You're kicked from this channel\nirc-server > Bad language is not allowed on this channel")

										if cli.admin {
											hasAdmin = false
										}

										for cli2 := range clients {
											if cli.username == cli2.username {
												delete(clients, cli2)
											}
										}
										cli.active = false
										delete(nameMap, cli.username)
										close(cli.client)
										kickMap[cli.username] = true
										cli.conn.Close()

										for cli2 := range clients {
											if cli2.username == currentUser.username {
												cli2.client <- ("irc-server > [" + cli.username + "] was kicked from channel for bad language policy violation")
											} else {
												cli2.client <- ("\nirc-server > [" + cli.username + "] was kicked from channel for bad language policy violation\n")
												showPrompt(cli2)
											}
										}

										found = true
										break
									}
								}
								if !found {
									currentUser.client <- "User not found."
								}
							} else {
								currentUser.client <- "Don't kick yourself!"
							}
						} else {
							currentUser.client <- "You are not ADMIN."
							break
						}
					} else {
						currentUser.client <- "No user written."
					}

				default:
					currentUser.client <- "Unrecognized command!"
				}

				if !msgbool {
					currentUser.client <- "\n"
				} else {
					msgbool = false
				}
				showPrompt(currentUser)
			} else {
				if currentUser.active {
					for cli := range clients {
						if cli != currentUser {
							cli.client <- (msg + "\n")
							showPrompt(cli)
						} else {
							showPrompt(cli)
						}
					}
				}

			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			if clients[cli] {
				fmt.Printf("irc-server > [%s] left\n", cli.username)
				if cli.admin {
					hasAdmin = false
				}
				delete(clients, cli)
				delete(nameMap, cli.username)
				close(cli.client)
			}

		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	data := make([]byte, 512)
	n, _ := conn.Read(data)
	who := string(data[:n])

	u := user{client: ch, username: who, ip: strings.Split(conn.RemoteAddr().String(), ":")[0], conn: conn, active: true}

	for name := range nameMap {
		if name == u.username {
			fmt.Fprintln(conn, "That username is already taken. Exit and try again.")
			return
		}
	}

	nameMap[u.username] = ""

	fmt.Printf("irc-server > New connected user [%s]\n", who)

	ch <- "irc-server > Welcome to the Simple IRC Server\nirc-server > Your user [" + who + "] is successfully logged"

	if firstUser {
		ch <- "\nirc-server > Congrats, you were the first user."
		firstUser = false
	}

	if !hasAdmin {
		u.admin = true
		hasAdmin = true
		fmt.Printf("irc-server > [%s] was promoted as the channel ADMIN\n", u.username)
		ch <- "\nirc-server > You're the new IRC Server ADMIN"
	} else {
		u.admin = false
	}
	ch <- "\n"

	messages <- "\nirc-server > New connected user [" + who + "]"
	entering <- u

	showPrompt(u)

	input := bufio.NewScanner(conn)
	for input.Scan() {
		messages <- "\n" + who + " > " + input.Text()
	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- u
	if !kickMap[who] {
		messages <- "\nirc-server > [" + who + "] left channel"
	} else {
		delete(kickMap, who)
	}
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprint(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	host := flag.String("host", "localhost", "host name")
	port := flag.String("port", "9000", "port number")
	flag.Parse()

	hostport := *host + ":" + *port

	listener, err := net.Listen("tcp", hostport)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("irc-server > Simple IRC Server started at %s\n", hostport)
	fmt.Println("irc-server > Ready for receiving new clients")

	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
