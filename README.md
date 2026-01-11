# Simple Graphing Caluclator in MonoGame

## Usage:

You set your function by going to the config folder and editing the Expression property of the expression.json file.

Expression: The function you'd like to graph
Thickness: How thick the function will appear
ScaleX: How far apart points are on the x axis
ScaleY: How far apart points are on the y axis
Step: The distance between every point, then a line will be drawn between every point to fill in the gaps. (Min: 0.1, Max: 2)
AxesOpacity: How opaque the background axes appear
AxesThickness: How thick the background axes appear
StepDelta: The amount that Step will be modified when scrolling your mouse wheel at runtime.

You can also click and drag to scale up and down the function at runtime and use your scrollwheel to modify the step.

## How To Start:

If you want to build yourself for safety reasons, you can always do 'dotnet build' inside the first GraphingCalculator directory.
If you dont care, you can go to GraphingCalculator/bin/Debug/net8.0-windows/ and run the GraphingCalculator.exe file.
