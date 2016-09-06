SFML = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

main: main.o ShipPlayer.o Bullet.o ManagerPlayer.o Allien.o ManageAlliens.o Welcome.o
	clang++ main.o ShipPlayer.o Bullet.o ManagerPlayer.o Allien.o ManageAlliens.o Welcome.o -o main $(SFML)

main.o: main.cpp ShipPlayer.h Bullet.h ManagerPlayer.h Welcome.h Allien.h ManageAlliens.h
	clang++ -c main.cpp

Welcome.o: Welcome.cpp Welcome.h
	clang++ -c Welcome.cpp

ShipPlayer.o: ShipPlayer.cpp ShipPlayer.h
	clang++ -c ShipPlayer.cpp

Bullet.o: Bullet.cpp Bullet.h
	clang++ -c Bullet.cpp

Allien.o: Allien.cpp Allien.h
	clang++ -c Allien.cpp

ManagerPlayer.o: ManagerPlayer.cpp ManagerPlayer.h Bullet.h ShipPlayer.h
	clang++ -c ManagerPlayer.cpp

ManageAlliens.o: ManageAlliens.cpp ManageAlliens.h Bullet.h ShipPlayer.h Allien.h
	clang++ -c ManageAlliens.cpp


clean:
	rm -f main *.o
