# SkiaPlot

A C++ plotting library built on top of Google's Skia graphics library. SkiaPlot provides a simple and intuitive interface for creating high-quality 2D plots and charts.

## Features

- **Simple API**: Easy-to-use interface for creating plots
- **High-Quality Output**: Leverages Skia's powerful rendering capabilities
- **Multiple Data Series**: Plot multiple datasets on the same chart
- **Customizable**: Configure colors, line styles, labels, and more
- **PNG Export**: Save plots as PNG images
- **Cross-Platform**: Works on Linux, macOS, and Windows

## Prerequisites

### Skia Library

SkiaPlot requires the Skia graphics library. You need to download and build Skia before building this project.

#### Building Skia

1. **Install depot_tools**:
   ```bash
   git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
   export PATH="${PWD}/depot_tools:${PATH}"
   ```

2. **Download Skia**:
   ```bash
   git clone https://skia.googlesource.com/skia.git
   cd skia
   python3 tools/git-sync-deps
   ```

3. **Build Skia**:
   ```bash
   # For Linux/macOS
   bin/gn gen out/Release --args='is_official_build=true'
   ninja -C out/Release
   
   # For Debug build
   bin/gn gen out/Debug
   ninja -C out/Debug
   ```

For more details, see the [official Skia build documentation](https://skia.org/docs/user/build/).

### System Dependencies

**Ubuntu/Debian**:
```bash
sudo apt-get install build-essential cmake libfontconfig1-dev libfreetype6-dev libgl1-mesa-dev
```

**macOS**:
```bash
brew install cmake
```

## Building SkiaPlot

1. **Clone this repository**:
   ```bash
   git clone https://github.com/huapeng01016/SkiaPlot.git
   cd SkiaPlot
   ```

2. **Set Skia path**:
   ```bash
   export SKIA_DIR=/path/to/your/skia
   ```

3. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

## Usage

### Quick Start

The simplest way to create a plot:

```cpp
#include "skiaplot.h"

int main() {
    std::vector<double> x = {0, 1, 2, 3, 4, 5};
    std::vector<double> y = {0, 1, 4, 9, 16, 25};
    
    SkiaPlot::Utils::quickPlot(x, y, "output.png", "y = x²");
    return 0;
}
```

### Advanced Usage

For more control over the plot:

```cpp
#include "skiaplot.h"
#include <cmath>

int main() {
    // Create a plot
    SkiaPlot::Plot plot(800, 600);
    
    // Configure appearance
    SkiaPlot::PlotConfig& config = plot.getConfig();
    config.title = "Sine Wave";
    config.xLabel = "x (radians)";
    config.yLabel = "sin(x)";
    config.showGrid = true;
    config.lineWidth = 3.0f;
    
    // Create data series
    SkiaPlot::DataSeries series("sin(x)");
    for (int i = 0; i < 100; ++i) {
        double x = i * 2.0 * M_PI / 99.0;
        double y = std::sin(x);
        series.addPoint(x, y);
    }
    
    // Add series and save
    plot.addSeries(series);
    plot.saveToFile("sine.png");
    
    return 0;
}
```

### Multiple Series

Plot multiple datasets on the same chart:

```cpp
SkiaPlot::Plot plot;

SkiaPlot::DataSeries series1("sin(x)");
SkiaPlot::DataSeries series2("cos(x)");

// Add data to series...

plot.addSeries(series1);
plot.addSeries(series2);
plot.saveToFile("multi.png");
```

## Examples

The `examples/` directory contains several example programs:

- **simple_plot**: Basic plotting example
- **sine_wave**: Plotting a sine wave with customization
- **multiple_series**: Plotting multiple datasets

Build and run the examples:

```bash
cd build
./examples/simple_plot
./examples/sine_wave
./examples/multiple_series
```

## API Reference

### Classes

#### `SkiaPlot::DataSeries`

Represents a series of data points.

**Methods**:
- `addPoint(double x, double y)`: Add a single point
- `addPoints(const std::vector<Point>& points)`: Add multiple points
- `setName(const std::string& name)`: Set the series name

#### `SkiaPlot::Plot`

Main plotting class.

**Methods**:
- `Plot(int width = 800, int height = 600)`: Constructor
- `addSeries(const DataSeries& series)`: Add a data series
- `clearSeries()`: Remove all series
- `setConfig(const PlotConfig& config)`: Set plot configuration
- `render()`: Render the plot
- `saveToFile(const std::string& filename)`: Save plot as PNG

#### `SkiaPlot::PlotConfig`

Configuration for plot appearance.

**Properties**:
- `width`, `height`: Canvas dimensions
- `title`, `xLabel`, `yLabel`: Text labels
- `backgroundColor`, `axisColor`, `gridColor`, `lineColor`: Colors (ARGB)
- `lineWidth`: Line thickness
- `showGrid`, `showPoints`: Display options

### Utility Functions

- `SkiaPlot::Utils::linspace(start, end, num)`: Generate evenly spaced points
- `SkiaPlot::Utils::quickPlot(x, y, filename, title)`: Quick plotting function

## License

This project is provided as-is for educational and research purposes.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Acknowledgments

- Built on [Google Skia](https://skia.org/) graphics library
- Inspired by matplotlib and other plotting libraries
