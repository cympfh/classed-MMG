test: mmg
	cat sample-input
	./mmg -D -K 2 < sample-input
	cat ~/study/reuters-exp/reuters-21578/train.tagged.input| head -n 40 | grep -v '^#'  | ./mmg -K 10

mmg: util.cc mmg.cc main.cc
	g++ -O3 -std=c++11 -o $@ main.cc -lgmp -lgmpxx

clean:
	-rm mmg
