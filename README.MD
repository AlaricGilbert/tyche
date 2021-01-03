# Tyche Web Framework
__Tyche: a web service framework written in C++__

## Project Structure
```
|
|---svc (A sample bbs service)
|    |
|    |---include
|    |
|    `---src
|
|---tyche (Framework codes)
|    |
|    |---include
|    |
|    `---src
...
```
## Build instructions

### macOS build instructions
* Install command-line tools:
```shell script
$ xcode-select --install
```
* Install CMake:
```shell script
$ brew install cmake
```
* Clone the repo:
```shell script
$ git clone https://github.com/AlaricGilbert/tyche
```
* Go into the directory, run the compile script:
```shell script
$ cd tyche
$ ./build.sh
```

### Linux build instructions
* Install tools (Example code in Ubuntu, GCC toolchain)
```shell script
$ apt-get update
$ apt-get insatll gcc g++ cmake
```
* Clone the repo:
```shell script
$ git clone https://github.com/AlaricGilbert/tyche
```
* Go into the directory, run the compile script:
```shell script
$ cd tyche
$ ./build.sh
```
* Then you can find results of compilations under `artifacts` folder.

### Windows build instructions
* Goto CMake website, install CMake and add CMake into `PATH`

* Clone the repo:
```powershell
PS git clone https://github.com/AlaricGilbert/tyche
```
#### Windows with MSVC tool chain
**We assume that MSVC was installed correctly in this context**

* Go into the directory, run the compile script (**USE POWERSHELL**):

```powershell
PS cd tyche
PS build.ps1
```
*You may need to modify bypass execution settings for your Powershell*

#### Windows with MinGW in PATH
**We assume that MinGW (win32) was installed correctly in this context and added into `PATH`**
* Go into the directory, run the compile script:
```powershell
PS cd tyche
PS mkdir build
PS cd build
PS cmake .. -G"MinGW Makefiles"
PS make
```

## License: The MIT License
```
Copyright (c) Alaric Gilbert

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```