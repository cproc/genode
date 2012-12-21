
TARG=test-select
OBJ=\
    main.o

CXXFLAGS = -DNOT_GENODE -fpermissive
LDFLAGS  = -pthread

.PHONY: all clean

all: $(TARG)

$(TARG): $(OBJ)
	$(CXX) $(LDFLAGS) $< -o $@
%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	-rm $(TARG) $(OBJ)
