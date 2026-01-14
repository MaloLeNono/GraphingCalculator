# Graphing Calculator

This is a pretty simple calculator, there are some more complex functions like `sec(x)` that haven't worked for me

## How to build

### 1. Requirements

If you're on Windows x64 and don't feel like building it yourself, feel free to grab precompiled binaries in the Releases tab.
For this to work, you will need to have `vcpkg` installed to manage the packages (SDL3). Make sure you take note of where your `vcpkg.cmake` file is.

### 2. Building.

1. In the `GraphingCalculator` directory, open your prefered terminal.
2. Make a new directory called `build` with `mkdir build`.
3. Go inside of that directory with `cd build`.
4. Type in `cmake -DCMAKE_TOOLCHAIN_FILE=path/to/your/vcpkg/scripts/buildsystems/vcpkg.cmake ..` and wait for it to do its thing.
5. When it's done, type `cmake --build . --config Release` and wait for it to be done.
6. Finally, type in `cmake --install . --prefix <installdir>`. Replace `<installdir>` with the directory you want the program to be installed in.
7. Follow the usage instructions below.

## Usage

In the same folder as your executable, open your terminal.
The arguments for the calculator are:

`<expression> <x scale> <y scale> <step>`

expression: The function you want to graph.

x scale: The scale at which the x axis will be drawn.

y scale: Same as x scale but with the y axis.

step: The x difference between every point calculated. The higher the step, the more jagged the function will look and vice versa.
