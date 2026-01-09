all: test
test: test.cc
	g++ -g -o test test.cc -lEG `root-config --cflags` `root-config --libs` -lTreePlayer -I /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/offline/InstallArea/include

clean:
	rm ./*~ ./*.o ./test

