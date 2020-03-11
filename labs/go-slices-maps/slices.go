package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	slice := make([][]uint8, dy)
	for k := range slice {
		slice[k] = make([]uint8, dx)
	}

	for x := 0; x < dx; x++ {
		for y := 0; y < dy; y++ {
			slice[x][y] = uint8(x ^ y)
		}
	}

	return slice
}

func main() {
	pic.Show(Pic)
}
