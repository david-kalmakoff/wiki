package main

import (
	"fmt"
	"interpreter/repl"
	"log"
	"os"
	"os/user"
)

func main() {
	user, err := user.Current()
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("Hello %s! This is the Monkey programming language.\n", user.Username)
	fmt.Println("Feel free to type in commands")

	repl.Start(os.Stdin, os.Stdout)
}
