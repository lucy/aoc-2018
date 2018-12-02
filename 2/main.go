package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

func main() {
	raw, _ := ioutil.ReadAll(os.Stdin)
	l := strings.Split(strings.TrimSpace(string(raw)), "\n")
	twos, threes := 0, 0
	for _, s := range l {
		a := map[rune]int{} // unicode support
		for _, b := range s {
			a[b]++
		}
		two, three := 0, 0
		for _, n := range a {
			if n == 2 {
				two = 1
			}
			if n == 3 {
				three = 1
			}
		}
		twos += two
		threes += three
	}
	fmt.Println(twos * threes)
	for i, a := range l {
		for _, b := range l[i+1:] {
			diff := 0
			for i := 0; i < len(a) && i < len(b) && diff < 2; i++ {
				if a[i] != b[i] {
					diff++
				}
			}
			if diff == 1 {
				for i := 0; i < len(a) && i < len(b); i++ {
					if a[i] == b[i] {
						fmt.Print(string(a[i]))
					}
				}
				fmt.Println()
				break
			}
		}
	}
}
