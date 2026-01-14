# Graphing Calculator

This is a pretty simple calculator, there are some more complex functions like `sec(x)` that haven't worked for me

## How to build

### 1. Requirements

If you don't feel like building this yourself, you can grab precompiled binaries in the Releases tab.
You will need to have a copy of SDL3 installed at `C:/SDL`. This program looks for SDL CMake files at `C:/SDL/cmake` and for `SDL3.dll` at `C:/SDL/bin`. I know this isn't great but I basically never used CMake so this will have to do.

### 2. Building.

1. In the `GraphingCalculator` directory, open your prefered terminal.
2. Make a new directory called `build` with `mkdir build`.
3. Go inside of that directory with `cd build`.
4. Type in `cmake ..` and wait for it to do its thing.
5. When it's done, type `cmake --build . --config Release` and wait for it to be done.
6. Finally, type in `cmake --install . --prefix <installdir>`. Replace `<installdir>` with the directory you want the program to be installed in.
7. Launch the `.exe`!

## Usage

In the same folder as your executable, open your terminal.
The arguments for the calculator are:

`<expression> <x scale> <y scale> <step>`

expression: The function you want to graph.

x scale: The scale at which the x axis will be drawn.

y scale: Same as x scale but with the y axis.

step: The x difference between every point calculated. The higher the step, the more jagged the function will look and vice versa.
