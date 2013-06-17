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

OBJ-MAIN-TESTS =					 	\
	src/matrix-tests.o 					\

clean:
	@$(RM) -rf $(OBJS) $(OBJ-MAIN-GEN) $(OBJ-MAIN-CLASSIF) $(OBJ-MAIN-TESTS)

%.o: %.cpp
	@g++ -c -o $@ $<

compile-gen: $(OBJS) $(OBJ-MAIN-GEN)
	@g++ -o tp3-gen $(OBJS) $(OBJ-MAIN-GEN) $(LIBS) -lrt -Wall -pedantic -Wextra

compile-classif: $(OBJS) $(OBJ-MAIN-CLASSIF)
	@g++ -o tp3-classif $(OBJS) $(OBJ-MAIN-CLASSIF) $(LIBS) -lrt -Wall -pedantic -Wextra

tests: $(OBJS) $(OBJ-MAIN-TESTS)
	@g++ -o tests $(OBJS) $(OBJ-MAIN-TESTS) $(LIBS) -lrt -Wall -pedantic -Wextra