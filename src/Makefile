CPPFLAGS = -std=c++11 -Wall -pedantic -I.
LDFLAGS =
LDLIBS =
SRCS = problem.cpp main.cpp
OBJS = $(subst .cpp,.o,$(SRCS))
EXENAME = vrp

all: $(EXENAME)

vrp: $(OBJS)
	g++ $(LDFLAGS) -o $(EXENAME) $(OBJS) $(LDLIBS)

%.o: %.cpp
	g++ $(CPPFLAGS) -c $<

clean:
	rm $(OBJS)

dist-clean: clean
	rm $(EXENAME)
