# Contributing to AGH Presenter
If you want to contribute to AGH Presenter or compile this project on your computer, you should read this page.

## Gettings started
### Branches
The official branch is master. Merges to master are realized via pull requests.

### Getting source code
```
git clone --recursive https://github.com/Vectrom/aghPresenter.git
```

### Prerequisities
- [CMake](https://cmake.org/) 3.15.5 or higher
- [Python](https://www.python.org/) 3.9.1 or higher
- [Conan](https://conan.io/) 1.32.1 or higher
- **Windows**: Microsoft Visual Studio 16 2019
- **macOS**: Xcode 11 or higher

On Linux additional packages (used by Qt) can be required:
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install -y libgl1-mesa-dev xorg-dev libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev xkb-data libxcb-util-dev
```

### Build 
**Clean previous build**:
```
cmake -E remove_directory build
```

**Make a build directory**:
```
cmake -E make_directory build
cd build
```
**Generate solution (in the build directory)**:
- on Windows:
```
cmake .. -G "Visual Studio 16 2019"
```
- on macOS:
```
cmake .. -G "Xcode"
```
- on Linux systems:
```
cmake .. -DCMAKE_BUILD_TYPE=Release
```

**You can open your .sln file on Windows, .xcodeproj file on macOS, Make on Linux or compile using CMake**:
```
cmake --build . --config Release
```

### Tests
AGH Presenter needs to be tested manually. Only basic smoke tests are provided. You can run using ctest:
```
ctest --parallel 8 -VV -C Release -T test --no-compress-output --timeout 60
```

### Deployment
Packages are created by GitHub Actions and they are availaible on build subpages. If you want to create .zip file on your local machine, it is possible using cpack:
```
cpack
```