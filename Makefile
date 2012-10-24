CC=gcc
CFLAGS=-Wall -g -O0 -m32 
LDFLAGS= -Wl,-m,elf_i386,-larmadillo,-lstdc++,-ldl,-lm
SOURCES=Image.cpp main.cpp MultiClassClassificator.cpp 
SOURCESASM = MultiClassClassificator.asm
OBJECTSASM=$(SOURCESASM:.asm=.o)
EXECUTABLE=Predict
LIBRARY=libclass.so

all: $(SOURCES) $(EXECUTABLE) $(LIBRARY)



$(LIBRARY):$(OBJECTSASM)
	ld -shared $(OBJECTSASM) -o $@ -m elf_i386

$(OBJECTSASM):$(SOURCESASM)
	nasm -O1 -f elf -g -F dwarf $< -o $@




$(EXECUTABLE):$(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $@
