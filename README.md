# Red Black Tree Implementation


# Uputstvo
1. `git clone https://github.com/neske99/RedBlackTreeImplementation.git`
2. Make ima naredne komande: main,test,topdf,cleanDotFiles,clean i exec
3. Make main pravi izvrsni main fajl koji je kompajlirani src/main.cpp
4. Make test pravi izvrsni test fajl koji je kompajlirani src/test.cpp
5. Make topdf konvertuje .dot iz dots direktorijuma i smesta ih u pfds/dots folder
6. Make cleanDotFiles brise .dot fajlove iz dots direktorijuma
7. Make clean brise .dot fajlove .pdf fajlove(iz odgovarajucih direktorijuma), main i test file
8. Make execMain kompajlira i izvrsava src/main.cpp (ako pravit makefile config u razvojnom okruzenju)
9. Make execTest kompajlira i izvrsava src/test.cpp (ako pravit makefile config u razvojnom okruzenju)

# main
1. main skenira standardni ulaz 
2. ako prva rec pocinje karakterom 'i' onda se druga rec konvertuje u int i insertu-je u stablo
3. ako prva rec pocinje karakterom 'e' onda se druga rec konvertuje u int i brise iz stabla
4. ako prva rec pocinje karakterom 'q' onda se izlazi iz petlje i program se zavrsava
5. za ostale ulaze program izbacuje undefined command
6. posle svakog insertovanja i brisanje stablo se eksportuje u .dot fajl 
7. posle ili tokom izvrsavanje main-a mozete pokrenuti make topdf da konvertujete .dot u .pdf fajlove

# Inspiracije
1. za brisanje: https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
2. za insertovanje: https://www.youtube.com/watch?v=5IBxA-bZZH8 i casovi vezbi
