# Building

## Dependencies

- **Qt > 6.5.0** _is mandatory_ for MultiEffect support.
- **Google Test** is optional only to build and run tests ![Google Test GitHub](https://github.com/google/googletest).


## Building 

Get the latest QuickQanava sources:

```sh
git clone https://github.com/cneben/QuickQanava
cd QuickQanava
```

Or install as a Git submodule:

```sh
$ git submodule add https://github.com/cneben/QuickQanava
$ git submodule update
```

QuickQanava is configured with CMake >= 3.5.0.

1. Open _CMakeLists.txt_ in QtCreator.

2. In 'Projects' panel, set `QUICK_QANAVA_BUILD_SAMPLES` option to `ON` in CMake configuration panel.

3. Select a kit, build and launch samples.

Or manually in command line using CMake:

```sh
$ cd QuickQanava
$ mkdir build
$ cd build

# IF QT_DIR IS CONFIGURED
$ cmake -DCMAKE_BUILD_TYPE=Release -DQUICK_QANAVA_BUILD_SAMPLES=OFF ..

# IF QT DIR IS NOT CONFIGURED, CONFIGURE KIT MANUALLY
$ cmake -DCMAKE_PREFIX_PATH="/home/xxx/Qt/6.6.1/gcc_64" -DQUICK_QANAVA_BUILD_SAMPLES=OFF -DBUILD_STATIC_QRC=TRUE ../QuickQanava/

$ cmake --build .

# Then run the samples in ./samples (with QUICK_QANAVA_BUILD_SAMPLES=ON)

# DO NOT make install, QuickQanava is not designed for system installation, only for submodule inclusion
```

Detailed instructions:  [Installation](http://cneben.github.io/QuickQanava/installation/index.html)
