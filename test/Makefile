do: lang_size.exe
	./lang_size.exe < lang_size-input

lang_size.exe: util.cc mmg.cc lang_size.cc
	g++ -std=c++11 -o $@ lang_size.cc -lgmp -lgmpxx

clean:
	rm *.exe
