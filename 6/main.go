package main

import (
	"fmt"
	"image"
	"image/color"
	"image/gif"
	"image/png"
	"math"
	"os"

	colorful "github.com/lucasb-eyer/go-colorful"
)

type co struct{ x, y, n int }

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

func dist(x, y int, c co) int { return abs(x-c.x) + abs(y-c.y) }

func gp(n int) color.Palette {
	p := []color.Color{color.Gray{255}, color.Gray{200}, color.Gray{170}}
	for i := 0; i < 360; i += 360 / n {
		p = append(p, colorful.Hsv(float64(i), 0.4, 0.92))
	}
	for i := 0; i < 360; i += 360 / n {
		p = append(p, colorful.Hsv(float64(i), 0.55, 0.95))
	}
	return p
}

func main() {
	c := co{}
	l := []co{}
	for {
		if n, _ := fmt.Scanf("%d, %d\n", &c.x, &c.y); n != 2 {
			break
		}
		l = append(l, c)
	}

	w, h := 0, 0
	for _, c := range l {
		if c.x > w {
			w = c.x
		}
		if c.y > h {
			h = c.y
		}
	}

	pa := gp(len(l))
	r := image.Rect(0, 0, w*2+2, h*2+2)
	im := image.NewPaletted(r, pa)
	set := func(x, y int, i int) {
		im.Set(x*2+0, y*2+0, pa[i])
		im.Set(x*2+1, y*2+0, pa[i])
		im.Set(x*2+0, y*2+1, pa[i])
		im.Set(x*2+1, y*2+1, pa[i])
	}

	ac := func(x, y int) int {
		i := pa.Index(im.At(x, y))
		if i > 2 {
			return i + len(l) + 1
		}
		return i
	}

	poke := func(x, y int) {
		im.Set(x*2+0, y*2+0, pa[ac(x*2+0, y*2+0)])
		im.Set(x*2+1, y*2+0, pa[ac(x*2+1, y*2+0)])
		im.Set(x*2+0, y*2+1, pa[ac(x*2+0, y*2+1)])
		im.Set(x*2+1, y*2+1, pa[ac(x*2+1, y*2+1)])
	}

	//w, h = 500, 500
	//wp := (600 - w)/2
	//hp := (500 - h)/2
	ims := []*image.Paletted{}
	for y := 0; y <= h; y++ {
		for x := 0; x <= w; x++ {
			min := math.MaxInt32
			var m, n int
			for i, c := range l {
				if t := dist(x, y, c); t < min {
					min, m, n = t, i, 1
				} else if t == min {
					n++
				}
			}
			if n != 1 {
				set(x, y, 1)
				continue
			}

			set(x, y, m+3)
			if x == 0 || y == 0 || x == w || y == h {
				l[m].n = -1
			} else if l[m].n != -1 {
				l[m].n++
			}

		}
		if y%20 == 0 {
			im2 := image.NewPaletted(r, pa)
			for i, p := range im.Pix {
				im2.Pix[i] = p
			}
			ims = append(ims, im2)
		}

	}

	im2 := image.NewPaletted(r, pa)
	for i, p := range im.Pix {
		im2.Pix[i] = p
	}
	ims = append(ims, im2)

	max := 0
	for _, c := range l {
		if c.n > max {
			max = c.n
		}
	}
	fmt.Println(max)

	n := 0
	for y := 0; y <= h; y++ {
		for x := 0; x <= w; x++ {
			a := 0
			for _, c := range l {
				a += dist(x, y, c)
			}
			if a < 10000 {
				n++
				poke(x, y)
			}

		}
		if y%20 == 0 {
			im2 := image.NewPaletted(r, pa)
			for i, p := range im.Pix {
				im2.Pix[i] = p
			}
			ims = append(ims, im2)
		}
	}
	fmt.Println(n)

	for _, c := range l {
		set(c.x, c.y, 0)
	}

	f, _ := os.Create("a.png")
	defer f.Close()
	png.Encode(f, im)

	de := []int{}
	for _, _ = range ims {
		de = append(de, 1)
	}
	g := &gif.GIF{ims, de, 0, nil, image.Config{pa, w*2 + 2, h*2 + 2}, 0}
	f, err := os.Create("a.gif")
	if err != nil {
		panic(err)
	}
	defer f.Close()
	err = gif.EncodeAll(f, g)
	if err != nil {
		panic(err)
	}
}
