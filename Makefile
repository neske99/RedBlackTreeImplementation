main:main.o rbtree.o
	g++ -o $@ -g -Wall -Wextra $^

test:test.o catch.o rbtree.o
	g++ -o $@ -g -Wall -Wextra $^

rbtree.o:lib/RBTree.cpp
	g++ -c -o $@ $<
main.o:src/main.cpp
	g++ -c -o $@ $< -Wall -Wextra -g
test.o:src/test.cpp
	g++ -c -o $@ $< -g -Wall -Wextra
catch.o:lib/catch.cpp
	g++ -c -o $@ $<

.PHONY:clean topdf cleanDotFiles exec

clean:
	rm -f dots/*.dot pdfs/dots/*.pdf main test test.o rbtree.o main.o

cleanDotFiles:
	rm -f dots/*.dot

topdf:
	for f in dots/*.dot; do \
		dot -Tpdf $$f >pdfs/$$f.pdf; \
	done

execMain:main
	./main
	make topdf
	make cleanDotFiles
execTest:test
	./test
