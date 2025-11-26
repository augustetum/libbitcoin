ifeq ($(OS),Windows_NT)
    # Windows
    RM = del /Q
    RM_DIR = rmdir /S /Q
    EXE = .exe
    LIBS = -lssl -lcrypto -lws2_32 -lcrypt32
    SEP = \\
    OMPFLAG = -fopenmp
else
    # macOS/Linux
    RM = rm -f
    RM_DIR = rm -rf
    EXE = 
    LIBS = -lssl -lcrypto
    SEP = /
    OMPFLAG = -fopenmp
endif

CXX = g++
CXXFLAGS = -std=c++17 -O3 $(OMPFLAG)
INCLUDES = -Iinclude -Ihash

# Main program
blockchain$(EXE): Program.o Block.o BlockHeader.o Transaction.o Functions.o merkleTree.o customGenerator.o
	$(CXX) $(CXXFLAGS) Program.o Block.o BlockHeader.o Transaction.o Functions.o merkleTree.o customGenerator.o -o blockchain$(EXE) $(LIBS)

Program.o: Program.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c Program.cpp -o Program.o

Block.o: Block.cpp Block.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c Block.cpp -o Block.o

BlockHeader.o: BlockHeader.cpp BlockHeader.h
	$(CXX) $(CXXFLAGS) -c BlockHeader.cpp -o BlockHeader.o

Transaction.o: Transaction.cpp Transaction.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c Transaction.cpp -o Transaction.o

Functions.o: Functions.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c Functions.cpp -o Functions.o

merkleTree.o: merkleTree.cpp merkleTree.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c merkleTree.cpp -o merkleTree.o

customGenerator.o: customGenerator.cpp customGenerator.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c customGenerator.cpp -o customGenerator.o

usergen$(EXE): generators/userGenerator.o customGenerator.o
	$(CXX) $(CXXFLAGS) generators/userGenerator.o customGenerator.o -o usergen$(EXE) $(LIBS)

generators/userGenerator.o: generators/userGenerator.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c generators/userGenerator.cpp -o generators/userGenerator.o

txgen$(EXE): generators/transactionGenerator.o Transaction.o customGenerator.o
	$(CXX) $(CXXFLAGS) generators/transactionGenerator.o Transaction.o customGenerator.o -o txgen$(EXE) $(LIBS)

generators/transactionGenerator.o: generators/transactionGenerator.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c generators/transactionGenerator.cpp -o generators/transactionGenerator.o

clean:
ifeq ($(OS),Windows_NT)
	-$(RM) *.o 2>nul
	-$(RM) generators$(SEP)*.o 2>nul
	-$(RM) *.exe 2>nul
else
	$(RM) *.o generators/*.o blockchain usergen txgen
endif

run: blockchain$(EXE)
	.$(SEP)blockchain$(EXE)

setup: usergen$(EXE) txgen$(EXE)
	.$(SEP)usergen$(EXE)
	.$(SEP)txgen$(EXE)

all: usergen$(EXE) txgen$(EXE) blockchain$(EXE)
	.$(SEP)usergen$(EXE)
	.$(SEP)txgen$(EXE)
	.$(SEP)blockchain$(EXE)

.PHONY: clean run setup all