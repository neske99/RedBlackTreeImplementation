run:1.cpp
	g++ -o $@ -g -Wall -Wextra $<

clean:
	rm -f *.dot run *.pdf

topdf:
	for number in *.dot ; do \
        dot -Tpdf $$number>$$number.pdf ; \
    done
	