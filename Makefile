# $Id: Makefile,v 1.4 1996/07/31 18:48:31 plm Exp plm $

HEADERS=#include/Waveguide.h\
# bla bla \
# bla bla

SOURCES=gsl_example.C\
# bla bla \
# bla bla

OBJECTS=gsl_example.o\
# bla bla \
# bla bla

PRODUCT=propagate

#VENDOR?=mac

#ifeq (${VENDOR}, mac)
LIBS+= -lm  -L/opt/local/lib -lgsl -lgslcblas 
INCLUDE=-Wall  -I/opt/local/include  -Iinclude
CC=g++
CFLAGS=-O3
#endif

LINK=$(CC)
LINKFLAGS= $(FORTRAN OBJECTS) $(LIBDIRS) $(LIBS)

#$(LIBDIRS) $(LIBS)

#-bloadmap:loadmap.log

RM=rm
RMFLAGS=-f
EMACSBACK=*~ include/*.h~
PDBFILES=*.pdb

all: $(PRODUCT) 

$(PRODUCT): $(SOURCES) $(HEADERS) $(OBJECTS) 
	$(LINK) -o $(PRODUCT) $(OBJECTS) $(LINKFLAGS) $(CFLAGS) 


include rules.make

# DO NOT DELETE THIS LINE -- make depend depends on it.
