match: match.exe
	./match.exe

all: lang_size
	make clean

lang_size: lang_size.exe
	./lang_size.exe < ./lang-size-input

%.exe: %.cc
	g++ -std=c++11 -o $@ $^ -lgmp -lgmpxx

clean:
	rm *.exe
