CPPFLAGS += -std=c++14
LIB += -lgtest

BIN := tests.x

.PHONY: test clean
test: $(BIN)
	./$(BIN)

tests.x: BigInteger.cc BigInteger.hh tests.cc
	$(CXX) $(LIB) $? -o $@

clean:
	-rm $(BIN)

