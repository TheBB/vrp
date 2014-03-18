CPPFLAGS = -std=c++11 -Wall -pedantic -I.
LDFLAGS = -fopenmp
LDLIBS =
RM = rm -f
SRCS = problem.cpp main.cpp assigners.cpp cwsavings.cpp
OBJS = $(subst .cpp,.o,$(SRCS))
EXENAME = vrp

all: $(EXENAME)

from-scratch: dist-clean $(EXENAME)

vrp: $(OBJS)
	g++ $(LDFLAGS) -o $(EXENAME) $(OBJS) $(LDLIBS)

%.o: %.cpp
	g++ $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) $(EXENAME)
