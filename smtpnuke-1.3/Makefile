VERSION =	1.3

INCLUDE =	-I/usr/local/include/Sockets 
CFLAGS =	-Wall -g -O2 $(INCLUDE) -MD `Sockets-config`
CPPFLAGS =	$(CFLAGS)
LIBS =		-L/usr/local/lib -lSockets -lsqlite -lpthread -lssl -lcrypto

PROGS =		smtpnuke

all:		$(PROGS)

SMTPNUKE =	SMTPSocket.o FwdSocket.o \
		smtpnuke.o NukeHandler.o \
		Database/Database.o \
		Database/Session.o \
		Database/Query.o \
		Mailinst.o Table.o \
		config.o
smtpnuke:	$(SMTPNUKE)
		g++ -o $@ $^ $(LIBS)

clean:
		rm -f *.o *~ *.d slask $(PROGS)
		rm -f Database/*.o Database/*~ Database/*.d

install:	all
		install --strip $(PROGS) /usr/local/bin

-include	*.d

PROJ_NAME =	smtpnuke
PROJ_DIR =	/usr/local/apache/www.alhem.net/htdocs/project/smtpnuke
tar:		clean
		tar czf $(PROJ_NAME)-$(VERSION).tar.gz *.h *.cpp Makefile \
			gpl.txt Database/*.h Database/*.cpp
		/usr/local/bin/tarfix.sh $(PROJ_NAME)-$(VERSION)
		cp $(PROJ_NAME)-$(VERSION).tar.gz $(PROJ_DIR)


docs:		clean
		./mkdot.sh
