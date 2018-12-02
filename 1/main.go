package main

import (
	"fmt"
	"image"
	"image/color"
	"image/color/palette"
	"image/gif"
	"io/ioutil"
	"os"
	"strconv"
	"strings"
)

func main() {
	raw, _ := ioutil.ReadAll(os.Stdin)
	in := []int{}
	for _, v := range strings.Split(strings.TrimSpace(string(raw)), "\n") {
		x, _ := strconv.Atoi(v)
		in = append(in, x)
	}
	freq := 0
	for _, v := range in {
		freq += v
	}
	fmt.Println(freq)
	m := map[int]struct{}{}
	freq = 0
	a := [][]int{}
L:
	for {
		s := make([]int, len(in))
		for i, v := range in {
			freq += v
			s[i] = freq
			if _, ok := m[freq]; ok {
				break L
			}
			m[freq] = struct{}{}
		}
		a = append(a, s)
	}
	fmt.Println(freq)
	min, max := 0, 0
	for _, s := range a {
		for _, v := range s {
			if v > max {
				max = v
			}
			if v < min {
				min = v
			}
		}
	}
	fmt.Println(max - min)
	r := image.Rect(0, 0, 800, 400)
	ims := []*image.Paletted{}
	im := image.NewPaletted(r, palette.Plan9)
	for _, s := range a {
		for i, v := range s {
			im.Set(i*800/len(s), (v-min)*400/(max-min), color.Gray{255})
		}
		im2 := image.NewPaletted(r, palette.Plan9)
		for i := r.Min.X; i < r.Max.X; i++ {
			for j := r.Min.Y; j < r.Max.Y; j++ {
				r, _, _, _ := im.At(i, j).RGBA()
				r8 := uint8(r / (0xffff / 0xff))
				if r8 > 10 {
					r8 -= 10
				}
				im.Set(i, j, color.Gray{r8})
				im2.Set(i, j, color.Gray{r8})
			}
		}
		ims = append(ims, im2)
	}
	d := []int{}
	for _, _ = range ims {
		d = append(d, 1)
	}
	g := &gif.GIF{ims, d, 0, nil, image.Config{color.Palette(palette.Plan9), 800, 400}, 0}
	f, _ := os.Create("a.gif")
	defer f.Close()
	gif.EncodeAll(f, g)
}
