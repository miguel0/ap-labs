// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	dist := 0.0
	for i := range path {
		if (i + 1) == len(path) {
			dist = path[i].Distance(path[0])
			sum += dist
			fmt.Printf("%v ", dist)
		} else {
			dist = path[i].Distance(path[i+1])
			sum += dist
			fmt.Printf("%v + ", dist)
		}
	}
	return sum
}

//!-path

func onSegment(p, q, r Point) bool {
	if (q.X() <= math.Max(p.X(), r.X())) && (q.X() >= math.Min(p.X(), r.X())) && (q.Y() <= math.Max(p.Y(), r.Y())) && (q.Y() >= math.Min(p.Y(), r.Y())) {
		return true
	}
	return false
}

func orientation(p, q, r Point) int {
	val := ((q.Y() - p.Y()) * (r.X() - q.X())) - ((q.X() - p.X()) * (r.Y() - q.Y()))
	if val > 0 {
		return 1
	} else if val < 0 {
		return 2
	} else {
		return 0
	}
}

func doIntersect(p1, q1, p2, q2 Point) bool {
	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)

	if (o1 != o2) && (o3 != o4) {
		return true
	}

	if (o1 == 0) && onSegment(p1, p2, q1) {
		return true
	}
	if (o2 == 0) && onSegment(p1, q2, q1) {
		return true
	}
	if (o3 == 0) && onSegment(p2, p1, q2) {
		return true
	}
	if (o4 == 0) && onSegment(p2, q1, q2) {
		return true
	}

	return false
}

func main() {
	if len(os.Args) < 2 {
		fmt.Printf("Wrong number of arguments.\n")
		return
	}

	rand.Seed(time.Now().UnixNano())
	sides, _ := strconv.Atoi(os.Args[1])
	if sides < 3 {
		fmt.Printf("Enter a numebr greater than 2.\n")
		return
	}
	path := make(Path, sides)

	for i := 0; i < sides; i++ {
		path[i] = Point{-100 + rand.Float64()*200, -100 + rand.Float64()*200}
		if i > 2 {
			for j := 0; j+2 < i; j++ {
				if doIntersect(path[j], path[j+1], path[i-1], path[i]) {
					i--
					break
				}
				if i == len(path)-1 && j != 0 && doIntersect(path[j], path[j+1], path[i], path[0]) {
					i--
					break
				}
			}
		}
	}

	fmt.Printf("- Generating a [%v] sides figure\n", sides)
	fmt.Printf("- Figure's vertices\n")
	for _, v := range path {
		fmt.Printf("\t- (  %v,   %v)\n", v.X(), v.Y())
	}
	fmt.Printf("- Figure's Perimeter\n")
	fmt.Printf("\t- ")
	fmt.Printf("= %v\n", path.Distance())
}
