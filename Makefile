run:1.cpp
	g++ -o $@ -g -Wall -Wextra $<

clean:
	rm -f dots/*.dot pdfs/dots/*.pdf

topdf:
	for number in dots/*.dot ; do \
        dot -Tpdf $$number> pdfs/$$number.pdf ; \
    done
	