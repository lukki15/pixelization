all: prepare

prepare:
	rm -rf build
	mkdir build

prepare_vcpkg:
	./external/vcpkg/bootstrap-vcpkg.sh
	./external/vcpkg/vcpkg install
