CPPFLAGS += -std=c++14 -O2
LIBS += -lboost_filesystem -lboost_system -lpthread
BIN := wordcount.x

.PHONY: all clean

all: wordcount.x

$(BIN): wordcount.cc
	$(CXX) $(CPPFLAGS) $(LIBS) $< -o $(BIN)

clean:
	-rm $(BIN)

