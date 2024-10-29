package render

import (
	"fmt"
	"os"

	"github.com/aidosgal/zet/internal/editor"
)

type Screen struct {
    fileName string
}

func NewScreen(fileName string) *Screen {
    return &Screen{fileName: fileName}
}

func (s *Screen) OpenScreen() {
    s.newFile(s.fileName)
    editor := editor.NewEditor(s.fileName)
    editor.InputLoop()
}

func (s *Screen) newFile(fileName string) {
    file, err := os.Create(fileName)
    if err != nil {
        fmt.Println("Error creating file:", err)
        return
    }

    defer file.Close()
}
