.SUFFIXES : .o .c

#source folders
vpath %.c src/structures
vpath %.c src/algorithms
vpath %.c src/utils
vpath %.c src

OBJ_FOLD= .obj
#forces C99
FLAGS = -std=c99 -O3

ALGORITHMS = lz77.o noCompression.o dynHuffman.o dynHuffmanCompr.o dynHuffmanDecompr.o fixHuffmanCompr.o fixHuffmanDecompr.o
STRUCTURES = lz77Entities.o matchIndex.o
UTILS = fileIO.o utils.o
DEFLATE = $(ALGORITHMS) $(STRUCTURES) $(UTILS) deflate.o

#add .obj/ to each file
DEFLATE_EXPANDED = $(shell echo $(DEFLATE) | sed 's|[^ ]* *|$(OBJ_FOLD)/&|g' )

.PHONY : directories
all : directories deflate

deflate : $(DEFLATE_EXPANDED)
	cc -o $@ $(DEFLATE_EXPANDED) -lm

$(OBJ_FOLD)/%.o : %.c
	cc $(FLAGS) -c $< -o $@

directories : $(OBJ_FOLD)

$(OBJ_FOLD) :
	mkdir -p $(OBJ_FOLD)
