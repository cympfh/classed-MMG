test: mmg
	./mmg -D -K 20 < input/wikiquote 2>&1 | tee log

mmg: util.cc mmg.cc main.cc
	g++ -O3 -std=c++11 -o $@ main.cc -lgmp -lgmpxx

clean:
	-rm mmg
