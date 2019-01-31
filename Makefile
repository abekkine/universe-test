TARGET=universe
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%.o)
CXXFLAGS  = -g -std=c++11
#CXXFLAGS += -Wall -Wextra -Werror -Wfatal-errors
CXXFLAGS += -Werror -Wfatal-errors
CXXFLAGS += -I/usr/include/libnoise
CXXFLAGS += -I/usr/include/freetype2
LIBS=-lglut -lGLU -lGL -lnoise -lftgl

all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $*.o $<

clean:
	$(RM) $(TARGET) $(OBJS) *~ core*
