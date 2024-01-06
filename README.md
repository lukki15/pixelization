# pixelization
Convert images into pixel art

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![C++](https://img.shields.io/badge/Linux-17-blue)

[![Main CI](https://github.com/lukki15/pixelization/actions/workflows/build-target-main.yaml/badge.svg)](https://github.com/lukki15/pixelization/actions/workflows/build-target-main.yaml)
[![Unit-Test CI](https://github.com/lukki15/pixelization/actions/workflows/build-target-unit-test.yaml/badge.svg)](https://github.com/lukki15/pixelization/actions/workflows/build-target-unit-test.yaml)

[![CodeCov CI](https://github.com/lukki15/pixelization/actions/workflows/code-cov.yml/badge.svg)](https://github.com/lukki15/pixelization/actions/workflows/code-cov.yml)
[![codecov](https://codecov.io/gh/lukki15/pixelization/graph/badge.svg?token=3HLOXGNKR2)](https://codecov.io/gh/lukki15/pixelization)

[![pre-commit](https://github.com/lukki15/pixelization/actions/workflows/pre-commit.yml/badge.svg)](https://github.com/lukki15/pixelization/actions/workflows/pre-commit.yml)
[![Documentation](https://github.com/lukki15/pixelization/actions/workflows/documentation.yml/badge.svg)](https://github.com/lukki15/pixelization/actions/workflows/documentation.yml)
[![CodeQL](https://github.com/lukki15/pixelization/actions/workflows/codeql.yml/badge.svg)](https://github.com/lukki15/pixelization/actions/workflows/codeql.yml)

## setup

```bash
$ make prepare
$ make prepare_vcpkg
```

see GitHub [build target action](.github/workflows/build-target-main.yaml)

## build

```bash
$ cmake -H. -Bbuild -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Release"
$ cmake --build build --config Release --target main -j
```

## execution

```
$ ./build/app/main --help
Pixelization v0.1.0
Usage:
  Pixelization [OPTION...]

      --help              Print usage
  -i, --input arg         Input image file
  -o, --output arg        Output image file
  -w, --width arg         Output image width
  -h, --height arg        Output image height
      --nearest_neighbor  Resize with nearest-neighbor
      --bi_linear         Resize with bi-linear
      --bi_cubic          Resize with bi-cubic
```

## example

### original
![original](assets/example.jpg) 

[image source](https://www.pexels.com/photo/smiling-woman-with-red-hair-1987301/)

### nearest neighbor
```
$ ./build/app/main -i assets/example.jpg -o assets/exampleNearestNeighbor.jpg -w 320 -h 480 --nearest_neighbor
```
![nearest neighbor](assets/exampleNearestNeighbor.jpg)

### bi linear
```
./build/app/main -i assets/example.jpg -o assets/exampleBiLinear.jpg -w 320 -h 480 --bi_linear
```
![bi linear](assets/exampleBiLinear.jpg)

### bi cubic
```
./build/app/main -i assets/example.jpg -o assets/exampleBiCubic.jpg -w 320 -h 480 --bi_cubic
```
![bi cubic](assets/exampleBiCubic.jpg)
