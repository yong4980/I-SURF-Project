CC = g++
TARGET1 = server
SOURCES1 = mars_server.cpp ./tcp/mars_tcp.cpp
TARGET2 = client
SOURCES2 = mars_client.cpp ./tcp/mars_tcp.cpp
TARGET3 = mars
SOURCES3 = mars_test.cpp
LDFLAGS = -pthread

server:
	$(CC) -o $(TARGET1) $(SOURCES1)
client:
	$(CC) -o $(TARGET2) $(SOURCES2) $(LDFLAGS)
mars:
	$(CC) -o $(TARGET3) $(SOURCES3) $(LDFLAGS)
board: client mars
all: server client mars

clean:
	rm -rf *.o $(TARGET1) $(TARGET2) $(TARGET3)
