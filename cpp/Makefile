all: compile-gen

OBJS = 									\
	src/cmd-args.o						\
	src/mmatrix.o 						\
	src/data-io.o						\
	src/gen-algorithms.o				\

OBJ-MAIN-GEN =					 	\
	src/tp3-gen.o 					\

clean:
	@$(RM) -rf $(OBJS) $(OBJ-MAIN-GEN)

%.o: %.cpp
	@g++ -c -o $@ $<

compile-gen: $(OBJS) $(OBJ-MAIN-GEN)
	@g++ -o tp3-gen $(OBJS) $(OBJ-MAIN-GEN) $(LIBS) -lrt