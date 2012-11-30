SRCCAT = src
DOCCAT = doc

all: sources

sources: 
	make --directory=$(SRCCAT)
doc:
	cd doc
	doxygen
