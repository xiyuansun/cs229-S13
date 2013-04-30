build/sim-gui: build/Makefile 
	cd build; make -f Makefile

sim-gui.pro: sim-gui/sim-gui.cc sim-gui/widgets/*.h lib/io/*.h lib/*.h
	cd sim-gui; qmake-qt4 -project -o sim-gui.pro

build/Makefile: sim-gui.pro
	cd build/; qmake-qt4 ../sim-gui/sim-gui.pro -o Makefile
