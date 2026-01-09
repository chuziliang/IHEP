all: analysis
analysis: analysis.cc
	g++ -g -o analysis analysis.cc -lEG `root-config --cflags` `root-config --libs` -lTreePlayer

clean:
	rm ./*~ ./*.o ./analysis

