package main

import (
	"fmt"
	"image"
	"image/color"
	"image/gif"
	"io"
	"os"
)

type claim struct {
	x, y, w, h int
}

var P = []color.Color{
	color.RGBA{255, 255, 255, 0},
	color.RGBA{249, 174, 164, 0},
	color.RGBA{239, 99, 139, 0},
	color.RGBA{172, 62, 144, 0},
	color.RGBA{95, 41, 142, 0},
	color.RGBA{56, 21, 86, 0},
}

func main() {
	w, h := 1000, 1000
	c := claim{}
	id := 0
	l := []claim{}
	for {
		r, err := fmt.Scanf("#%d @ %d,%d: %dx%d\n",
			&id, &c.x, &c.y, &c.w, &c.h)
		if r != 5 || err == io.EOF {
			break
		}
		if err != nil {
			panic(err)
		}
		l = append(l, c)
	}

	d := 2
	r := image.Rect(0, 0, w/d+1, h/d+1)
	ims := []*image.Paletted{}
	im := image.NewPaletted(r, P)

	fabric := make([]int, w*h)
	for i, c := range l {
		im3 := image.NewPaletted(r, P)
		for i := r.Min.X; i < r.Max.X; i++ {
			for j := r.Min.Y; j < r.Max.Y; j++ {
				im3.Pix[i*im.Stride+j] = im.Pix[i*im.Stride+j]
			}
		}
		for x := c.x; x < c.x+c.w; x++ {
			for y := c.y; y < c.y+c.h; y++ {
				fabric[x+w*y]++
				a := im3.Pix[x/d*im.Stride+y/d]
				if int(a) <= len(P) {
					a++
				}
				im.Pix[x/d*im.Stride+y/d] = a
			}
		}
		im2 := image.NewPaletted(r, P)
		for i := r.Min.X; i < r.Max.X; i++ {
			for j := r.Min.Y; j < r.Max.Y; j++ {
				im2.Pix[i*im.Stride+j] = im.Pix[i*im.Stride+j]
			}
		}
		if i%4 == 0 {
			ims = append(ims, im2)
		}
	}
	de := []int{}
	for _, _ = range ims {
		de = append(de, 1)
	}
	g := &gif.GIF{ims, de, 0, nil, image.Config{color.Palette(P), w/d + 1, h/d + 1}, 0}
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
