CXX = g++ -std=c++11
JSONPATH = jsoncpp-src-0.5.0/include
FLAGS = -Wall -L/usr/local/lib -Wl,-rpath,/usr/local/lib -I$(JSONPATH) -g
LIBS = -lpthread

TARGET = swarm.out

JSON_OBJECTS = jsoncpp-src-0.5.0/src/lib_json/json_reader.o \
	       jsoncpp-src-0.5.0/src/lib_json/json_value.o \
	       jsoncpp-src-0.5.0/src/lib_json/json_writer.o

BASE_SOURCES = $(wildcard */*.cpp) $(wildcard *.cpp)
BASE_OBJ = ${BASE_SOURCES:.cpp=.o}
BASE_OBJECTS = ${BASE_OBJ:.c=.o}

OBJECTS = ${BASE_OBJECTS} ${JSON_OBJECTS}

all: ${TARGET}

${TARGET}: ${OBJECTS}
	${CXX} ${FLAGS} -o ${TARGET} ${OBJECTS} ${LIBS}

clean:
		rm -rf *~ *.o ${TARGET} ${JSON_OBJECTS}

.cpp.o:
	${CXX} ${FLAGS} -c $< -o $@
.c.o:
	${CXX} ${FLAGS} -c $< -o $@

.PHONY: all clean submit
