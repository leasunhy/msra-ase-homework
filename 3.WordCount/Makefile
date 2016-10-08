CPPFLAGS += -std=c++14 -Ofast -Wall
LIBS += -lboost_filesystem -lboost_system -lpthread -lboost_regex
BIN := wordcount.x

.PHONY: all clean

all: wordcount.x

$(BIN): wordcount.cc
	$(CXX) $(CPPFLAGS) $(LIBS) $< -o $(BIN)

clean:
	-rm $(BIN)

