package main

import (
	"fmt"
	"math"
	"os"
	"path/filepath"
	"strings"
)

var numb [5]int
var leng [5]int

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {
	err := filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			fmt.Printf("Error accessing path %s: %v\n", path, err)
			os.Exit(1)
		}

		switch mode := info.Mode(); {
		case mode&os.ModeDir != 0:
			numb[0]++
		case mode&os.ModeSymlink != 0:
			numb[1]++
		case mode&os.ModeDevice != 0:
			numb[2]++
		case mode&os.ModeSocket != 0:
			numb[3]++
		default:
			numb[4]++
		}

		return nil
	})
	if err != nil {
		fmt.Printf("Error walking the path %s: %v\n", dir, err)
		os.Exit(1)
	}

	maxLen := len(dir)
	for i := 0; i < len(numb); i++ {
		leng[i] = int(math.Ceil(math.Log10(float64(numb[i]))))
		if numb[i] == 0 || numb[i] == 1 {
			leng[i] = 1
		}
		if leng[i] > maxLen {
			maxLen = leng[i]
		}
	}

	fmt.Println("Directory Scanner Tool")
	fmt.Printf("+-------------------------+%s+\n", strings.Repeat("-", maxLen+2))
	fmt.Printf("| Path                    | %s%s |\n", dir, strings.Repeat(" ", maxLen-len(dir)))
	fmt.Printf("+-------------------------+%s+\n", strings.Repeat("-", maxLen+2))
	fmt.Printf("| Directories             | %v%s |\n", numb[0], strings.Repeat(" ", maxLen-leng[0]))
	fmt.Printf("| Symbolic Links          | %v%s |\n", numb[1], strings.Repeat(" ", maxLen-leng[1]))
	fmt.Printf("| Devices                 | %v%s |\n", numb[2], strings.Repeat(" ", maxLen-leng[2]))
	fmt.Printf("| Sockets                 | %v%s |\n", numb[3], strings.Repeat(" ", maxLen-leng[3]))
	fmt.Printf("| Other files             | %v%s |\n", numb[4], strings.Repeat(" ", maxLen-leng[4]))
	fmt.Printf("+-------------------------+%s+\n", strings.Repeat("-", maxLen+2))

	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	err := scanDir(os.Args[1])
	if err != nil {
		fmt.Printf("Error scanning directory %s: %v\n", os.Args[1], err)
		os.Exit(1)
	}
}
