package main

import (
	"fmt"
	"os"
	"runtime"
	"strconv"
	"time"
)

func stage(from, to chan int) {
	v := <-from
	to <- v
}

func main() {
	var goroutines int
	var err error

	if len(os.Args) > 1 {
		goroutines, err = strconv.Atoi(os.Args[1])
		if err != nil {
			fmt.Println("Invalid number of goroutines to create.")
			return
		}
	} else {
		goroutines = 1000000000
	}

	f, err := os.Create("94.txt")
	if err != nil {
		fmt.Println(err)
		return
	}

	_, err = f.WriteString("Starting to generate goroutines:\n")
	if err != nil {
		fmt.Println(err)
		f.Close()
		return
	}

	started := time.Now()

	in := make(chan int)
	a := in
	b := make(chan int)

	for i := 0; i < goroutines; i++ {
		go stage(a, b)
		a = b
		b = make(chan int)
		if i%100000 == 0 {
			_, err = f.WriteString(PrintMemUsage(i, fmt.Sprintf("%s", time.Since(started))))
			if err != nil {
				fmt.Println(err)
				f.Close()
				return
			}
		}
	}

	_, err = f.WriteString(PrintMemUsage(goroutines, fmt.Sprintf("%s", time.Since(started))))
	if err != nil {
		fmt.Println(err)
		f.Close()
		return
	}

	err = f.Close()
	if err != nil {
		fmt.Println(err)
		return
	}
}

// PrintMemUsage returns a strin with the current, total and OS memory being used. As well as the number
// of garage collection cycles completed.
func PrintMemUsage(i int, elapsed string) string {
	res := fmt.Sprintf("Created goroutines: %d\tElapsed time since the start of the program: %v\n", i, elapsed)

	var m runtime.MemStats
	runtime.ReadMemStats(&m)

	res += fmt.Sprintf("Alloc = %d MiB", bToMb(m.Alloc))
	res += fmt.Sprintf("\tTotalAlloc = %d MiB", bToMb(m.TotalAlloc))
	res += fmt.Sprintf("\tSys = %d MiB", bToMb(m.Sys))
	res += fmt.Sprintf("\tNumGC = %d \n", m.NumGC)

	return res
}

func bToMb(b uint64) uint64 {
	return b / 1024 / 1024
}
