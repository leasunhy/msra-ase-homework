CPPFLAGS += -std=c++14
LIB += -lgtest

BIN := tests.x

.PHONY: test clean
test: $(BIN)
	./$(BIN)

$(BIN): BigInteger.o tests.cc
	$(CXX) $(LIB) $? -o $@

%.o: %.cc
	$(CXX) $(LIB) $< -c -o $@

clean:
	-rm $(BIN) *.o

