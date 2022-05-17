headers = $(wildcard *.h)
sources = $(wildcard *.c)
executable = 03_parser.out

$(executable): $(headers) $(sources)
	gcc -g -Wall -fdiagnostics-color=always -g -o $@ $^

test: $(executable)
	./$(executable)

clean:
	rm $(executable)

.PHONY: test clean