plugins_LTLIBRARIES = postgis.la
postgis_la_SOURCES = resultset.hh connection.hh connection_manager.hh postgis.cc  postgisfs.cc  postgis.hh
postgis_la_LIBDADD = 
postgis_la_LDFLAGS = -module -version-info 0:0:0 $(POSTGRESQL_LIBS) -lpthread

INCLUDES = -I ../ $(POSTGRESQL_CFLAGS)

