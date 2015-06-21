do: mmg
	./mmg < input

mmg: util.cc mmg.cc main.cc
	g++ -std=c++11 -o $@ main.cc
