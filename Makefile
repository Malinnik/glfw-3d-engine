run:
	cmake -B ./build -S .
	cd ./build && make && ./3d-engine

run-windows:
	cmake -B ./build -S .
	cmake --build ./build
	./build/debug/3d-engine.exe

build:
	cmake -B ./build -S .
