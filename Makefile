run:src/test.cpp lib/RBTree.cpp
	g++ -o $@ -g -Wall -Wextra $^

.PHONY:clean topdf cleanDotFiles exec

clean:
	rm -f dots/*.dot pdfs/dots/*.pdf run

cleanDotFiles:
	rm -f dots/*.dot

topdf:
	for f in dots/*.dot; do \
		dot -Tpdf $$f >pdfs/$$f.pdf; \
	done

exec:run
	./run
	make topdf
	make cleanDotFiles