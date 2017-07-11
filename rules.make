# some possible flags..

#LIBS= 
#-lslatec -limsl -lxlf90
#-lm -lcomplex -lessl -limsl
#-lnetcdf_c++ -lnetcdf
#-lC -limsl      if standard input "cin" and IMSL routine is needed

#LIBDIRS=
#-L/a/plm/lib
#-L/mpqrzg1/plm/lib

#INCLUDE=
#-Iinclude -I/mpqrzg1/plm/include -I/mpqrzg1/plm/include/stl

#CFLAGS=
#-O3 -pg      #for profile
#-pg
#-qbrowse 
#-qignerrno   #allows for additional optimizations if errno not used.
#-qinlglue    #fast external linkage...
#-qlist       #produce object listing in .lst file
#-qtocdata    #place small scalar external data in TableOfContents
#-qtwolink    #link twice to minimize static constructors from libraries.
#-qusepcomp[=dirname] #use pretokeninzed include files.
#-qarch=pwr2  #Code for POWER 2 architecture
#-qdisjoint=  #Tell optimizer about things that don't alias.
#-qtune=pwr2  #Produce object optimized for POWER 2 architecture.
#-g           # debugging information
#-g -qhd -lhd  #heap view debugger

.C.o:	
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

clean:
	$(RM) $(RMFLAGS) $(OBJECTS) $(PRODUCT) $(PDBFILES) \
	core tempinc/* 

order:
	$(RM) $(RMFLAGGS) $(EMACSBACK)

revision:
	ci -l Makefile $(SOURCES) $(HEADERS)
	chmod 644 Makefile $(SOURCES) $(HEADERS)

printout:
	a2ps -nr -c -nP Makefile $(HEADERS) $(SOURCES) TODO > printout.ps

wc:
	wc Makefile $(HEADERS) $(SOURCES)

rlog:
	rlog RCS/* include/RCS/* > rlog.txt

gprof: gmon.out
	gprof > gprof_report

etags:
	etags -C $(SOURCES) $(HEADERS)

depend:
	makedepend -- $(CFLAGS) -- $(SOURCES) $(INCLUDE) -I/usr/lpp/xlC/include
