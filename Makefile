.PHONY: build

build:
	cmake -B ./build -S . -D CMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}scripts/buildsystems/vcpkg.cmake"

run:
	make build
	cd ./build && make && ./3d-engine

run-windows:
	make build
	cmake --build ./build
	./build/debug/3d-engine.exe

