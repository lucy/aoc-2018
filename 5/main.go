package main

import (
	"bytes"
	"fmt"
	"image"
	"image/color"
	"image/gif"
	"io/ioutil"
	"os"

	"github.com/lucasb-eyer/go-colorful"
)

func gp(n int) color.Palette {
	p := []color.Color{color.Gray{255}}
	for i := 0; i < 360; i += 360/n {
		p = append(p, colorful.Hsv(float64(i), 0.5, 0.9))
	}
	p = append(p, color.Gray{255})
	for i := 0; i < 360; i += 360/n {
		p = append(p, colorful.Hsv(float64(i), 0.45, 0.95))
	}
	return p
}

func lower(c byte) byte   { return c | 0x20 }
func test(a, b byte) bool { return lower(a) == lower(b) && a != b }

func put(im *image.Paletted, i int, c byte, a bool) {
	color := lower(c) - 'a'
	if a {
		color += 26 + 1
	}
	x := i / 250 * 2
	y := (i % 250) * 2
	im.Pix[(x+0)*500+(y+0)] = color
	im.Pix[(x+1)*500+(y+0)] = color
	im.Pix[(x+0)*500+(y+1)] = color
	im.Pix[(x+1)*500+(y+1)] = color
}

func react(p []byte, buf []byte, skip byte) (int, []*image.Paletted) {
	r := image.Rect(0, 0, 250*2, 50000/250*2)
	ims := []*image.Paletted{}
	pa := gp(26)
	im := image.NewPaletted(r, pa)

	i := 0
	for j, a := range p {
		if lower(a) == skip {
			continue
		}
		if i > 0 && test(a, buf[i-1]) {
			i--
		} else {
			buf[i] = a
			i++
		}

		if j%75 == 0 {
			fmt.Println("frame", i)
			for i := range im.Pix {
				im.Pix[i] = 0
			}
			for k := 0; k < i; k++ {
				//x*width+y
				//x=k/width, y=k%width
				put(im, j-k, buf[k], true)
				//im.Pix[k*500/250+(k%250)*2] = lower(buf[k]) - 'a'
				//im.Pix[k] = lower(buf[k]) - 'a'
			}
			avail := float64(len(p) - i)
			step := avail / float64(len(p)-j)
			fmt.Println(step)
			for o := 0; o < len(p)-j; o++ {
				x := p[j+o]
				//for k := 0; k < step; k++ {
				//im.Pix[
				//}
				put(im, j+o, x, false)
			}

			im2 := image.NewPaletted(r, pa)
			for i, c := range im.Pix {
				im2.Pix[i] = c
			}

			ims = append(ims, im2)
		}
	}

	return i, ims
}

func main() {
	p, _ := ioutil.ReadAll(os.Stdin)
	p = bytes.TrimSpace(p)
	buf := make([]byte, len(p))
	n, ims := react(p, buf, 0)
	fmt.Println(n)
	//min := -1
	//for a := byte('a'); a <= 'z'; a++ {
	//	n, ims2 := react(buf[:n], p, a)
	//	ims = append(ims, ims2...)
	//	if min == -1 || n < min {
	//		min = n
	//	}
	//}
	//fmt.Println(min)
	de := []int{}
	for _, _ = range ims {
		de = append(de, 1)
	}
	g := &gif.GIF{ims, de, 0, nil, image.Config{color.Palette(gp(26)), 250 * 2, 200 * 2}, 0}
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
