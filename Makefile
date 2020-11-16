CXX ?=gcc

FLAGS +=-O3 -Wall

EXE=wavext

main: main.c
	$(CXX) main.c -o $(EXE) $(FLAGS)

clean:
ifeq ($(OS),Windows_NT)
	del $(EXE).exe
else
	rm -f $(EXE)
endif
