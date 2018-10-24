CC = g++

CFLAGS  = -O2 -I. -I../pqp/include
LDFLAGS	= -L. -L../pqp/lib
LDLIBS  = -lPQP -lm      

.SUFFIXES: .cpp

SRCS    = pqp_client.cpp

OBJECTS	= pqp_client.o

TARGET  = pqp_client

CLEAN   = $(OBJECTS) $(TARGET)

.cpp.o:
	$(CC) ${CFLAGS} -c $<

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -L. $(LDFLAGS) $(LDLIBS)

run: $(TARGET)
	$(TARGET)

clean:
	/bin/rm -f $(CLEAN)

