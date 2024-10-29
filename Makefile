build:
	@go build -o bin/zet cmd/zet/main.go

test:
	@go test -v ./...

run: build
	@./bin/jumystap
