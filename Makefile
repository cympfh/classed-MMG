test: mmg
	./mmg -D -K 23 <input/wikiquote

mmg: objs global.cc util.cc mmg.cc main.cc
	g++ -O3 -std=c++11 -o $@ setcover.o main.cc -lgmp -lgmpxx

objs: setcover.o

setcover.o: setcover.cc
	g++ -O3 -std=c++11 -c -o $@ $^ -lgmp -lgmpxx

clean:
	-rm mmg
