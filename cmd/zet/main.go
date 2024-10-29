package main

import (
	"os"

	"github.com/aidosgal/zet/internal/render"
)

func main() {
    for _, file := range os.Args[1:] {
        screen := render.NewScreen(file)
        screen.OpenScreen()
    }
}
