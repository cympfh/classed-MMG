test: mmg
	./mmg -D -K 23 < input/wikiquote 2>w.log
	./mmg -D -U -K 23 < input/wikiquote 2>u.log

mmg: objs global.cc util.cc mmg.cc main.cc
	g++ -O3 -std=c++11 -o $@ setcover.o main.cc -lgmp -lgmpxx

objs: setcover.o

setcover.o: setcover.cc
	g++ -O3 -std=c++11 -c -o $@ $^ -lgmp -lgmpxx

clean:
	-rm mmg
