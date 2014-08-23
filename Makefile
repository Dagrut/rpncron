all: make

make:
	+make -C build/

cmake:
	mkdir -p build/
	cd build && cmake ..

clean:
	+make -C build/ clean

mrproper:
	rm -rf build/

rebuild:
	rm -rf build/
	mkdir -p build/
	cd build && cmake ..
	+make -C build/

