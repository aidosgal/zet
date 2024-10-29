package editor

import (
	"fmt"
	"os"

	"github.com/nsf/termbox-go"
)

type Editor struct {
    fileName string
    input    []rune
}

func NewEditor(fileName string) *Editor {
    return &Editor{fileName: fileName, input: []rune{}}
}

func (e *Editor) InputLoop() {
    err := termbox.Init() 
    if err != nil {
        fmt.Println(err)
    }
    defer termbox.Close()

    termbox.Clear(termbox.ColorBlack, termbox.ColorBlack)


    for {
        e.renderInput()

        event := termbox.PollEvent()

        switch event.Type {
        case termbox.EventKey:
            if event.Key == termbox.KeyCtrlC {
                return
            } else if event.Key == termbox.KeyBackspace || event.Key == termbox.KeyBackspace2 {
                if len(e.input) > 0 {
                    e.input = e.input[:len(e.input)-1]
                }
            } else if event.Key == termbox.KeyCtrlS {
                e.saveInputToFile()
            } else {
                e.input = append(e.input, event.Ch)
            }
        }
    } 
}

func (e *Editor) renderInput() {
    termbox.Clear(termbox.ColorBlack, termbox.ColorBlack)
    for i, char := range e.input {
        termbox.SetCell(i, 0, char, termbox.ColorWhite, termbox.ColorBlack)
    }
    termbox.Flush()
}

func (e *Editor) saveInputToFile() {
    file, err := os.OpenFile(e.fileName, os.O_APPEND|os.O_WRONLY, 0644)
    if err != nil {
        fmt.Println("Error opening file:", err)
        return
    }
    defer file.Close()

    _, err = file.WriteString(string(e.input) + "\n")
    if err != nil {
        fmt.Println("Error writing to file:", err)
        return
    }

    e.input = []rune{}
}
