PROG=		tui
VERSION=	0.1

SRCS=		tui.cc window.cc
OBJS=		${SRCS:N*.h:R:S/$/.o/g}

CC?=		cc
CXXFLAGS+=	-g -Wall -std=c++11
CPPFLAGS+=
LDADD=		-lstdc++ -lncurses
DEFS=		-DVERSION=${VERSION}

all:		${PROG}

${PROG}:	${OBJS}
	${CC} ${CXXFLAGS} -o ${.TARGET} ${OBJS} ${LDADD}

.cc.o:
	${CC} ${CXXFLAGS} ${CPPFLAGS} ${DEFS} -c ${.IMPSRC}

check:	${SRCS}
	cppcheck --enable=all --suppress=missingIncludeSystem ${CPPFLAGS} ${.ALLSRC}

clean:
	rm -f ${PROG} ${OBJS}
