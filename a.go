package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"sort"
	"strconv"
	"strings"
)

func factorize(n int64) (f []int64) {
	for n%2 == 0 {
		f = append(f, 2)
		n = n / 2
	}
	for i := int64(3); i*i <= n; i = i + 2 {
		for n%i == 0 {
			f = append(f, i)
			n = n / i
		}
	}
	if n > 2 {
		f = append(f, n)
	}
	return f
}

func group(x []int64) (a [][]int64) {
	for len(x) > 0 {
		var v int64
		v, x = x[0], x[1:]
		r := []int64{v}
		for len(x) > 0 && x[0] == v {
			r, x = append(r, x[0]), x[1:]
		}
		a = append(a, r)
	}
	return a
}

func cartN(a ...[]int64) [][]int64 {
	c := 1
	for _, a := range a {
		c *= len(a)
	}
	if c == 0 {
		return nil
	}
	p := make([][]int64, c)
	b := make([]int64, c*len(a))
	n := make([]int64, len(a))
	s := 0
	for i := range p {
		e := s + len(a)
		pi := b[s:e]
		p[i] = pi
		s = e
		for j, n := range n {
			pi[j] = a[j][n]
		}
		for j := int64(len(n)) - 1; j >= 0; j-- {
			n[j]++
			if n[j] < int64(len(a[j])) {
				break
			}
			n[j] = 0
		}
	}
	return p
}

type Int64Slice []int64

func (p Int64Slice) Len() int           { return len(p) }
func (p Int64Slice) Less(i, j int) bool { return p[i] < p[j] }
func (p Int64Slice) Swap(i, j int)      { p[i], p[j] = p[j], p[i] }

var dmemo map[int64][]int64 = map[int64][]int64{}

func divisors(n int64) (f []int64) {
	if x, ok := dmemo[n]; ok {
		return x
	}
	fa := group(factorize(n))
	za := [][]int64{}
	for _, a := range fa {
		l := []int64{1}
		x := int64(1)
		for _, v := range a {
			x *= v
			l = append(l, x)
		}
		za = append(za, l)
	}
	for _, a := range cartN(za...) {
		if len(a) < 2 {
			f = append(f, a[0])
		} else {
			f = append(f, a[0]*a[1])
		}
	}
	sort.Sort(Int64Slice(f))
	dmemo[n] = f
	return f
}

func product(f []int64) int64 {
	x := int64(1)
	for _, y := range f {
		x *= y
	}
	return x
}

func a(n int64, m int64) int64 {
	if n <= m {
		return 0
	}
	divs := divisors(n)
	max := int64(0)
	for _, x := range divs {
		if x > m {
			y := a(n/x, x)
			if y > max {
				max = y
			}
		}
	}
	return max + 1
}

func main() {
	in, err := ioutil.ReadAll(os.Stdin)
	if err != nil {
		panic(err)
	}
	n, err := strconv.ParseInt(strings.TrimSpace(string(in)), 10, 64)
	if err != nil {
		panic(err)
	}
	fmt.Println(a(n, 1))
}
