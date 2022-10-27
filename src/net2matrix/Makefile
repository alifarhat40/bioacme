VPATH = .:../BioModel:$(HOME)/include
LIBPATH = ../BioModel

SRCS = *.cpp

OBJ = net2matrix.o anyoption.o $(LIBPATH)/HashTable.o		\
$(LIBPATH)/InitialConds.o $(LIBPATH)/SLinkedList.o		\
$(LIBPATH)/SparseMatrix.o $(LIBPATH)/MatrixElement.o		\
$(LIBPATH)/Stack.o $(LIBPATH)/State.o $(LIBPATH)/StateSpace.o	\
$(LIBPATH)/Stoichiometry.o $(LIBPATH)/Network.o

CPP_DEPS = net2matrix.d anyoption.d $(LIBPATH)/HashTable.d	\
$(LIBPATH)/InitialConds.d $(LIBPATH)/SLinkedList.d		\
$(LIBPATH)/SparseMatrix.d $(LIBPATH)/MatrixElement.d		\
$(LIBPATH)/Stack.d $(LIBPATH)/State.d $(LIBPATH)/StateSpace.d	\
$(LIBPATH)/Stoichiometry.d $(LIBPATH)/Network.d

LIBS := -lm -lstdc++ -lsbml -lz -lbz2 -lxml2

all : net2matrix

net2matrix : $(OBJ)
	g++ -o"net2matrix" $(OBJ) -L$(LD) $(LIBS)

%.o : %.cpp
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<" -I$(INC)

.PHONY : install
install:
	cp net2matrix $(BIN)

.PHONY : clean
clean:
	rm -f *.o *.d net2matrix

.PHONY : distclean
distclean: clean
	rm -f $(BIN)/net2matrix 

