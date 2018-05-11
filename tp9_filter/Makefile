# -------------- #
# -- Makefile -- #
# -------------- #

OBJ    = main.o filterNR.o gauss.o noise.o nrutil.o random.o sampleNR.o test_filterNR.o
HEADER = def.h  filterNR.h gauss.h noise.h nrutil.h random.h sampleNR.h test_filterNR.h

#OPTION = -Wunused-parameter -W\\#pragma-messages 

OPTION = -Wno-unused-parameter -Wno-comment

tp: $(OBJ) $(HEADER)
	gcc -o tp $(OBJ) -lm

filterNR.o: filterNR.c $(HEADER)
	gcc -c $(OPTION) filterNR.c

gauss.o: gauss.c $(HEADER)
	gcc -c $(OPTION) gauss.c

main.o: main.c $(HEADER)
	gcc -c $(OPTION) main.c

noise.o: noise.c $(HEADER)
	gcc -c $(OPTION) noise.c

nrutil.o: nrutil.c $(HEADER)
	gcc -c $(OPTION) nrutil.c

random.o: random.c $(HEADER)
	gcc -c $(OPTION) random.c

sampleNR.o: sampleNR.c $(HEADER)
	gcc -c $(OPTION) sampleNR.c

test_filterNR.o: test_filterNR.c $(HEADER)
	gcc -c $(OPTION) test_filterNR.c

clean:
	rm -f $(OBJ)
	rm -f tp
