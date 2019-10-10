INCLUDEPATH = ./

csv.o: src/csv.cpp
	g++ -c src/csv.cpp -o csv.o -I$(INCLUDEPATH)

stoneage.o: src/stoneage.cpp
	g++ -c src/stoneage.cpp -o stoneage.o -I$(INCLUDEPATH)

utils.o: src/utils.cpp
	g++ -c src/utils.cpp -o utils.o -I$(INCLUDEPATH)

main.o: main.cpp
	g++ -c main.cpp -o main.o -I$(INCLUDEPATH)

all: csv.o stoneage.o utils.o main.o main.o

sa: all
	g++ *.o -o sa

clean:
	rm *.o