.PHONY: build docs

build:
	cmake -B ./build -S . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="/MDd" -D CMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}scripts/buildsystems/vcpkg.cmake"

docs:
	cd ./docs/Doxygen && doxygen
	cd ./docs/Sphinx &&	make html

docs-win:
	make docs
	powershell -c ./docs/Sphinx/build/html/index.html

run:
	make build
	cd ./build && make && ./3d-engine

run-win:
	make build
	cmake --build ./build
	./build/Debug/3d-engine.exe

