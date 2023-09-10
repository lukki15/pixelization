all: prepare

prepare:
	rm -rf build
	mkdir build

prepare_vcpkg:
	./external/vcpkg/bootstrap-vcpkg.sh
	./external/vcpkg/vcpkg install

conan_d:
	rm -rf build
	mkdir build
	cd build && conan install .. -s build_type=Debug --output-folder=. --build missing -s compiler.cppstd=17

conan_r:
	rm -rf build
	mkdir build
	cd build && conan install .. -s build_type=Release --output-folder=. --build missing -s compiler.cppstd=17
