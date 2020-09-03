all : replace.o prac03.o replace.h.gch prac01.o prac02.o
	gcc -o prac03 prac03.o replace.o
	gcc -o prac02 prac02.o
	gcc -o prac01 prac01.o


replace.o:
	gcc -c replace.c
replace.h.gch:
	gcc -c replace.h
prac03.o:
	gcc -c prac03.c



prac02.c:
	gcc -c prac02.c
prac01.c:
	gcc -c prac01.c




run1:
	./prac01
run2:
	./prac02
run3:
	./prac03

clean:
	rm -f prac01.o prac01 prac02.o prac02 replace.o replace.h.gch prac03.o prac03



