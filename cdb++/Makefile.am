noinst_LIBRARIES = libcdb++.a
EXTRA_DIST = ChangeLog cdbtest.cc

INCLUDES = -I../mystring -I../fdbuf

test: cdbtest
cdbtest: cdbtest.o libcdb++.a
	$(CXX) $(CXXFLAGS) -o cdbtest cdbtest.o -L../misc -lmisc -L../mystring -lmystring -L. -lcdb++
cdbtest.o: cdbtest.cc cdb++.h

libcdb___a_SOURCES = \
	cdb++.h internal.h datum.h datum.cc \
	cdb_reader.cc cdb_writer.cc \
	cdb_getrec.cc cdb_get.cc cdb_make.cc

CLEANFILES = cdbtest.o cdbtest
