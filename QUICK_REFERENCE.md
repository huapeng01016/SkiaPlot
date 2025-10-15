# SkiaPlot Quick Reference

## Installation

```bash
# 1. Build Skia (one-time setup)
git clone https://skia.googlesource.com/skia.git
cd skia
python3 tools/git-sync-deps
bin/gn gen out/Release --args='is_official_build=true'
ninja -C out/Release

# 2. Set environment variable
export SKIA_DIR=/path/to/skia

# 3. Build SkiaPlot
git clone https://github.com/huapeng01016/SkiaPlot.git
cd SkiaPlot
./build.sh
```

## Basic Usage

### Quick Plot
```cpp
#include "skiaplot.h"

std::vector<double> x = {0, 1, 2, 3, 4};
std::vector<double> y = {0, 1, 4, 9, 16};
SkiaPlot::Utils::quickPlot(x, y, "plot.png", "Title");
```

### Standard Plot
```cpp
SkiaPlot::Plot plot(800, 600);

SkiaPlot::DataSeries series("My Data");
series.addPoint(0, 0);
series.addPoint(1, 1);
series.addPoint(2, 4);

plot.addSeries(series);
plot.saveToFile("output.png");
```

### Customized Plot
```cpp
SkiaPlot::Plot plot;

// Configure appearance
plot.getConfig().title = "My Plot";
plot.getConfig().xLabel = "X Axis";
plot.getConfig().yLabel = "Y Axis";
plot.getConfig().lineWidth = 3.0f;
plot.getConfig().showGrid = true;
plot.getConfig().showPoints = false;

// Add data and save
plot.addSeries(series);
plot.saveToFile("custom.png");
```

## Common Patterns

### Multiple Series
```cpp
SkiaPlot::Plot plot;

SkiaPlot::DataSeries series1("Series 1");
SkiaPlot::DataSeries series2("Series 2");

// Add data to both series...

plot.addSeries(series1);
plot.addSeries(series2);
plot.saveToFile("multi.png");
```

### Generate Data
```cpp
// Linear space
auto points = SkiaPlot::Utils::linspace(0, 10, 100);

// Function plot
SkiaPlot::DataSeries series;
for (double x = 0; x < 10; x += 0.1) {
    series.addPoint(x, sin(x));
}
```

### Configuration Options
```cpp
SkiaPlot::PlotConfig config;
config.width = 1024;              // Canvas width
config.height = 768;              // Canvas height
config.title = "Title";           // Plot title
config.xLabel = "X";              // X-axis label
config.yLabel = "Y";              // Y-axis label
config.lineWidth = 2.0f;          // Line thickness
config.pointRadius = 4.0f;        // Point size
config.showGrid = true;           // Show grid
config.showPoints = true;         // Show points
config.backgroundColor = 0xFFFFFFFF;  // White (ARGB)
config.lineColor = 0xFF0000FF;        // Blue (ARGB)

plot.setConfig(config);
```

## Color Format

Colors use ARGB format: `0xAARRGGBB`

Common colors:
- White:  `0xFFFFFFFF`
- Black:  `0xFF000000`
- Red:    `0xFFFF0000`
- Green:  `0xFF00FF00`
- Blue:   `0xFF0000FF`
- Yellow: `0xFFFFFF00`
- Gray:   `0xFF808080`

## API Classes

### Point
```cpp
SkiaPlot::Point p(1.0, 2.0);  // x=1.0, y=2.0
```

### DataSeries
```cpp
SkiaPlot::DataSeries series("Name");
series.addPoint(x, y);
series.addPoints(vector<Point>);
series.setPoints(vector<Point>);
series.getName();
series.setName("New Name");
```

### Plot
```cpp
SkiaPlot::Plot plot(width, height);
plot.setConfig(config);
plot.getConfig();
plot.addSeries(series);
plot.clearSeries();
plot.render();
plot.saveToFile("file.png");
plot.getCanvas();  // Advanced: direct Skia access
```

## Build Commands

### CMake
```bash
mkdir build && cd build
cmake ..
make
./examples/simple_plot
```

### Make (Unix/macOS only)
```bash
make all
./build/simple_plot
```

### Build Script
```bash
./build.sh
cd build/examples
./simple_plot
```

## Troubleshooting

**Problem**: CMake can't find Skia  
**Solution**: `export SKIA_DIR=/path/to/skia`

**Problem**: Linking errors  
**Solution**: Ensure Skia is built with same compiler

**Problem**: Empty plot  
**Solution**: Check that series has points before rendering

**Problem**: File not created  
**Solution**: Check write permissions and disk space

## Examples Location

All example programs are in the `examples/` directory:
- `simple_plot.cpp` - Basic usage
- `sine_wave.cpp` - Function plotting
- `multiple_series.cpp` - Multi-series plot

## Full Documentation

- `README.md` - Project overview
- `USAGE.md` - Complete API documentation
- `CONTRIBUTING.md` - Development guidelines

## License

MIT License - See LICENSE file
