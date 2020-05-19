package main

import (
	"fmt"
	"os"
	"os/signal"
	"strconv"
	"time"
)

func main() {
	var limit, current int
	var limited bool
	var err error

	file, err := os.Create("95.txt")
	if err != nil {
		fmt.Println(err)
		return
	}

	if len(os.Args) > 1 {
		limit, err = strconv.Atoi(os.Args[1])
		if err != nil {
			fmt.Println("Invalid number of messages to send.")
			return
		}
		limited = true
	}

	c := make(chan os.Signal, 1)
	nolim := make(chan int)

	q := make(chan int)
	var i int64
	start := time.Now()
	go func() {
		q <- 1
		for {
			if limited && current > limit {
				nolim <- 1
				return
			}
			current++
			i++
			q <- <-q
		}
	}()
	go func() {
		for {
			if limited && current > limit {
				nolim <- 1
				return
			}
			current++
			q <- <-q
		}
	}()

	if !limited {
		signal.Notify(c, os.Interrupt)
		<-c
	} else {
		<-nolim
	}

	_, err = file.WriteString(fmt.Sprint(float64(i)/float64(time.Since(start))*1e9, " round trips per second"))
	if err != nil {
		fmt.Println(err)
		file.Close()
		return
	}
}
