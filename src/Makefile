TARGET:=lsmid
TARGET+=catmid
TARGET+=catmid2

CXXFLAGS:=-Wall -Werror
CXXFLAGS+=-std=c++17

_SYS=$(shell uname -o)
ifeq ($(_SYS),Msys)
WIN32:=1
endif

ifdef WIN32
# for ntohX (sigh..)
lsmid: LDLIBS+=-lws2_32
endif

LIBMIDIFILE:=../../mf2t/libmidifile-20150710
catmid: CXXFLAGS+=-I$(LIBMIDIFILE)
catmid: LDFLAGS+=-L$(LIBMIDIFILE)
catmid: LDLIBS+=-lmidifile

MIDIFILE:=../../midifile
catmid2: CXXFLAGS+=-I$(MIDIFILE)/include
catmid2: LDFLAGS+=-L$(MIDIFILE)/lib
catmid2: LDLIBS+=-lmidifile

all:$(TARGET)

clean:
	$(RM) $(TARGET)

clobber: clean
