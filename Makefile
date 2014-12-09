COMPILER = g++
CFLAGS = -std=c++11 -Wall -I include

OBJ = TcpListener.o Event.o EventRegistrar.o Node.o NetworkStructure.o \
      NetworkMessage.o FileManager.o main.o out.o

pshare: $(OBJ)
	$(COMPILER) $(CFLAGS) -o $@ $^ -pthread

main.o: main.cpp
	$(COMPILER) $(CFLAGS) -c -o main.o main.cpp -pthread

%.o: src/%.cpp
	$(COMPILER) $(CFLAGS) -c -o $@ $< -pthread

clean:
	rm -f *.o pshare
