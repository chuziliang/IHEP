all: test
test: test.cc
	g++ -g -o test test.cc -lEG `root-config --cflags` `root-config --libs` -lTreePlayer

clean:
	rm ./*~ ./*.o ./test

