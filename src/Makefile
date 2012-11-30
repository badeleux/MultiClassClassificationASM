CC=gcc
CFLAGS=-Wall -g -O3 -m32 
LDFLAGS= -Wl,-m,elf_i386,-lpthread,-larmadillo,-lstdc++,-ldl,-lm
SOURCES=Image.cpp main.cpp MultiClassClassificator.cpp 
SOURCESASM = MultiClassClassificator.asm
OBJECTSASM=$(SOURCESASM:.asm=.o)
EXECUTABLE=Predict
LIBRARY=libclass.so
BINCAT=../bin/

all: $(EXECUTABLE) $(LIBRARY)


$(LIBRARY):$(OBJECTSASM)
	ld -shared $(OBJECTSASM) -o $@ -m elf_i386
	mv $(OBJECTSASM) $(BINCAT)

$(OBJECTSASM):$(SOURCESASM)
	nasm -O1 -f elf -g -F dwarf $< -o $@

$(EXECUTABLE):$(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $@
	mv $(EXECUTABLE) $(BINCAT)
