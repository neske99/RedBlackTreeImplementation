run:1.cpp
	g++ -o $@ -g -Wall -Wextra $<

clean:
	rm -f *.dot run