// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"flag"
	"fmt"
	"log"

	"gopl.io/ch5/links"
)

type urlDepth struct {
	urls  []string
	depth int
}

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

func crawl(url string) []string {
	fmt.Println(url)
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token

	if err != nil {
		log.Print(err)
	}
	return list
}

//!-sema

//!+
func main() {
	depthp := flag.Int("depth", -1, "web crawler depth")
	flag.Parse()
	url := flag.Args()
	depth := *depthp + 1

	if len(url) == 0 {
		fmt.Println("You must specify an URL.")
		return
	}

	worklist := make(chan urlDepth)
	var n int // number of pending sends to worklist

	// Start with the command-line arguments.
	n++
	go func() { worklist <- urlDepth{urls: url, depth: depth} }()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist

		for _, link := range list.urls {
			if !seen[link] && list.depth != 0 {
				seen[link] = true
				n++
				go func(link string) {
					worklist <- urlDepth{urls: crawl(link), depth: list.depth - 1}
				}(link)
			}
		}
	}
}

//!-
