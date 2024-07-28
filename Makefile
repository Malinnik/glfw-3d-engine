run:
	cmake -B ./build -S .
	cd ./build && make && ./3d-engine

build: cmake -B ./build -S .
