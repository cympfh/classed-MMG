do: mmg
	./mmg -D -K 2 < sample-input

mmg: util.cc mmg.cc main.cc
	g++ -O3 -std=c++11 -o $@ main.cc -lgmp -lgmpxx

runtest: test
	./test < input

test: util.cc mmg.cc test.cc
	g++ -std=c++11 -o $@ test.cc -lgmp -lgmpxx

clean:
	-rm mmg
