all: compile-gen compile-classif

OBJS = 									\
	src/cmd-args.o						\
	src/mmatrix.o 						\
	src/data-io.o						\
	src/algorithms.o					\

OBJ-MAIN-GEN =					 		\
	src/tp3-gen.o 						\

OBJ-MAIN-CLASSIF =					 	\
	src/tp3-classif.o 					\

clean:
	@$(RM) -rf $(OBJS) $(OBJ-MAIN-GEN) $(OBJ-MAIN-CLASSIF)

%.o: %.cpp
	@g++ -c -o $@ $<

compile-gen: $(OBJS) $(OBJ-MAIN-GEN)
	@g++ -o tp3-gen $(OBJS) $(OBJ-MAIN-GEN) $(LIBS) -lrt

compile-classif: $(OBJS) $(OBJ-MAIN-CLASSIF)
	@g++ -o tp3-classif $(OBJS) $(OBJ-MAIN-CLASSIF) $(LIBS) -lrt