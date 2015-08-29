mmg.o: mmg.cc
	g++ -c -O3 -std=c++11 -o $@ mmg.cc -lgmp -lgmpxx

do: mmg
	./mmg -D -K 2 < sample-input

mmg: util.cc mmg.cc main.cc
	g++ -O3 -std=c++11 -o $@ main.cc -lgmp -lgmpxx

clean:
	-rm mmg
