all: FitPedestal FitData

FitPedestal: FitPedestal.cc
	g++ -g -o FitPedestal FitPedestal.cc -lEG `root-config --cflags` `root-config --libs` -lTreePlayer

FitData: FitData.cc
	g++ -g -o FitData FitData.cc -lEG `root-config --cflags` `root-config --libs` -lTreePlayer

clean:
	rm ./*~ ./*.o ./FitPedestal ./FitData

 