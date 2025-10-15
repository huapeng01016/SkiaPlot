# SkiaPlot Usage Guide

This guide provides detailed information on how to use the SkiaPlot library.

## Table of Contents

1. [Getting Started](#getting-started)
2. [Core Concepts](#core-concepts)
3. [API Reference](#api-reference)
4. [Examples](#examples)
5. [Advanced Usage](#advanced-usage)

## Getting Started

### Basic Plot

The simplest way to create a plot is using the `quickPlot` utility function:

```cpp
#include "skiaplot.h"

int main() {
    std::vector<double> x = {0, 1, 2, 3, 4};
    std::vector<double> y = {0, 1, 4, 9, 16};
    
    SkiaPlot::Utils::quickPlot(x, y, "output.png", "My Plot");
    return 0;
}
```

## Core Concepts

### Data Points and Series

SkiaPlot works with **data points** organized into **data series**:

- **Point**: A single (x, y) coordinate
- **DataSeries**: A collection of points with a name

```cpp
SkiaPlot::DataSeries series("My Data");
series.addPoint(1.0, 2.0);
series.addPoint(2.0, 4.0);
series.addPoint(3.0, 6.0);
```

### Plot Object

The `Plot` class manages the rendering and configuration:

```cpp
SkiaPlot::Plot plot(800, 600);  // width x height
plot.addSeries(series);
plot.saveToFile("output.png");
```

### Configuration

Customize your plot using `PlotConfig`:

```cpp
SkiaPlot::PlotConfig& config = plot.getConfig();
config.title = "My Plot Title";
config.xLabel = "X Axis";
config.yLabel = "Y Axis";
config.lineWidth = 3.0f;
config.showGrid = true;
```

## API Reference

### Point Structure

```cpp
struct Point {
    double x;
    double y;
    
    Point();
    Point(double x, double y);
};
```

### DataSeries Class

**Constructor:**
```cpp
DataSeries(const std::string& name = "Data");
```

**Methods:**
```cpp
void addPoint(double x, double y);
void addPoints(const std::vector<Point>& points);
void setPoints(const std::vector<Point>& points);
const std::vector<Point>& getPoints() const;
const std::string& getName() const;
void setName(const std::string& name);
void getRange(double& xMin, double& xMax, double& yMin, double& yMax) const;
```

**Example:**
```cpp
SkiaPlot::DataSeries series("Temperature");
series.addPoint(0, 20.5);
series.addPoint(1, 21.0);
series.addPoint(2, 22.3);

std::cout << "Series name: " << series.getName() << std::endl;
```

### PlotConfig Structure

**Properties:**

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `width` | int | 800 | Canvas width in pixels |
| `height` | int | 600 | Canvas height in pixels |
| `marginLeft` | int | 60 | Left margin |
| `marginRight` | int | 40 | Right margin |
| `marginTop` | int | 40 | Top margin |
| `marginBottom` | int | 60 | Bottom margin |
| `backgroundColor` | uint32_t | 0xFFFFFFFF | Background color (ARGB) |
| `axisColor` | uint32_t | 0xFF000000 | Axis color (ARGB) |
| `gridColor` | uint32_t | 0xFFCCCCCC | Grid color (ARGB) |
| `lineColor` | uint32_t | 0xFF0000FF | Line color (ARGB) |
| `lineWidth` | float | 2.0f | Line thickness |
| `showGrid` | bool | true | Show/hide grid |
| `showPoints` | bool | true | Show/hide data points |
| `pointRadius` | float | 4.0f | Point radius |
| `title` | string | "" | Plot title |
| `xLabel` | string | "" | X-axis label |
| `yLabel` | string | "" | Y-axis label |

**Color Format:**

Colors are in ARGB format (32-bit unsigned integer):
- `0xAARRGGBB` where AA=alpha, RR=red, GG=green, BB=blue
- Example: `0xFFFF0000` is opaque red

**Example:**
```cpp
SkiaPlot::PlotConfig config;
config.width = 1024;
config.height = 768;
config.title = "Temperature Over Time";
config.xLabel = "Time (hours)";
config.yLabel = "Temperature (°C)";
config.backgroundColor = 0xFFF5F5F5;  // Light gray
config.lineColor = 0xFFFF0000;        // Red
config.lineWidth = 3.0f;
config.showGrid = true;
config.showPoints = false;

plot.setConfig(config);
```

### Plot Class

**Constructor:**
```cpp
Plot(int width = 800, int height = 600);
```

**Methods:**
```cpp
void setConfig(const PlotConfig& config);
PlotConfig& getConfig();
void addSeries(const DataSeries& series);
void clearSeries();
bool render();
bool saveToFile(const std::string& filename);
SkCanvas* getCanvas();
```

**Example:**
```cpp
SkiaPlot::Plot plot(1024, 768);

// Configure
plot.getConfig().title = "My Plot";
plot.getConfig().lineWidth = 2.5f;

// Add data
SkiaPlot::DataSeries series;
series.addPoint(0, 0);
series.addPoint(1, 1);
plot.addSeries(series);

// Save
if (plot.saveToFile("output.png")) {
    std::cout << "Success!" << std::endl;
}
```

### Utility Functions

**linspace:**
```cpp
std::vector<Point> linspace(double start, double end, int num);
```

Creates evenly spaced points from `start` to `end`.

**Example:**
```cpp
auto points = SkiaPlot::Utils::linspace(0.0, 10.0, 100);
// Creates 100 points from x=0 to x=10, y=0
```

**quickPlot:**
```cpp
bool quickPlot(const std::vector<double>& x, 
               const std::vector<double>& y,
               const std::string& filename,
               const std::string& title = "");
```

Quick plotting function for simple use cases.

**Example:**
```cpp
std::vector<double> x = {0, 1, 2, 3, 4};
std::vector<double> y = {0, 2, 4, 6, 8};
SkiaPlot::Utils::quickPlot(x, y, "line.png", "Linear Function");
```

## Examples

### Example 1: Simple Line Plot

```cpp
#include "skiaplot.h"

int main() {
    SkiaPlot::Plot plot(800, 600);
    
    SkiaPlot::DataSeries series("Linear");
    for (int i = 0; i <= 10; ++i) {
        series.addPoint(i, 2 * i + 1);
    }
    
    plot.addSeries(series);
    plot.getConfig().title = "y = 2x + 1";
    plot.saveToFile("linear.png");
    
    return 0;
}
```

### Example 2: Sine and Cosine

```cpp
#include "skiaplot.h"
#include <cmath>

int main() {
    SkiaPlot::Plot plot(800, 600);
    plot.getConfig().title = "Trigonometric Functions";
    plot.getConfig().xLabel = "Angle (radians)";
    plot.getConfig().yLabel = "Value";
    plot.getConfig().showPoints = false;
    
    // Sine series
    SkiaPlot::DataSeries sineSeries("sin(x)");
    for (int i = 0; i <= 100; ++i) {
        double x = i * 2 * M_PI / 100;
        sineSeries.addPoint(x, std::sin(x));
    }
    
    // Cosine series
    SkiaPlot::DataSeries cosSeries("cos(x)");
    for (int i = 0; i <= 100; ++i) {
        double x = i * 2 * M_PI / 100;
        cosSeries.addPoint(x, std::cos(x));
    }
    
    plot.addSeries(sineSeries);
    plot.addSeries(cosSeries);
    plot.saveToFile("trig.png");
    
    return 0;
}
```

### Example 3: Custom Styling

```cpp
#include "skiaplot.h"

int main() {
    SkiaPlot::Plot plot(1024, 768);
    
    // Custom configuration
    SkiaPlot::PlotConfig& config = plot.getConfig();
    config.title = "Styled Plot";
    config.xLabel = "X Values";
    config.yLabel = "Y Values";
    config.backgroundColor = 0xFF1A1A1A;  // Dark background
    config.axisColor = 0xFFFFFFFF;        // White axes
    config.gridColor = 0xFF333333;        // Dark gray grid
    config.lineWidth = 3.0f;
    config.pointRadius = 6.0f;
    config.showGrid = true;
    config.showPoints = true;
    
    // Add data
    SkiaPlot::DataSeries series("Data");
    series.addPoint(0, 0);
    series.addPoint(1, 3);
    series.addPoint(2, 1);
    series.addPoint(3, 4);
    series.addPoint(4, 2);
    
    plot.addSeries(series);
    plot.saveToFile("styled.png");
    
    return 0;
}
```

### Example 4: Scatter Plot

```cpp
#include "skiaplot.h"
#include <random>

int main() {
    SkiaPlot::Plot plot(800, 600);
    
    // Configure for scatter plot
    plot.getConfig().title = "Scatter Plot";
    plot.getConfig().showPoints = true;
    plot.getConfig().pointRadius = 5.0f;
    plot.getConfig().lineWidth = 0.5f;  // Thin lines between points
    
    // Generate random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(0.0, 1.0);
    
    SkiaPlot::DataSeries series("Random Data");
    for (int i = 0; i < 50; ++i) {
        series.addPoint(dist(gen), dist(gen));
    }
    
    plot.addSeries(series);
    plot.saveToFile("scatter.png");
    
    return 0;
}
```

## Advanced Usage

### Direct Canvas Access

For advanced users who need more control, you can access the underlying Skia canvas:

```cpp
SkiaPlot::Plot plot;
plot.render();  // Render the basic plot

SkCanvas* canvas = plot.getCanvas();
if (canvas) {
    // Use Skia API directly
    SkPaint paint;
    paint.setColor(SK_ColorRED);
    canvas->drawCircle(100, 100, 50, paint);
}

plot.saveToFile("custom.png");
```

### Multiple Plots

Create different plots in the same program:

```cpp
// Plot 1
SkiaPlot::Plot plot1(800, 600);
plot1.addSeries(series1);
plot1.saveToFile("plot1.png");

// Plot 2
SkiaPlot::Plot plot2(800, 600);
plot2.addSeries(series2);
plot2.saveToFile("plot2.png");
```

### Reusing Configuration

```cpp
SkiaPlot::PlotConfig commonConfig;
commonConfig.width = 1024;
commonConfig.height = 768;
commonConfig.title = "Common Style";

SkiaPlot::Plot plot1, plot2;
plot1.setConfig(commonConfig);
plot2.setConfig(commonConfig);
```

### Data Preprocessing

```cpp
// Smooth data using moving average
std::vector<SkiaPlot::Point> smoothData(
    const std::vector<SkiaPlot::Point>& data, 
    int windowSize) {
    
    std::vector<SkiaPlot::Point> result;
    for (size_t i = 0; i < data.size(); ++i) {
        double sum = 0;
        int count = 0;
        
        for (int j = -windowSize/2; j <= windowSize/2; ++j) {
            int idx = i + j;
            if (idx >= 0 && idx < data.size()) {
                sum += data[idx].y;
                count++;
            }
        }
        
        result.emplace_back(data[i].x, sum / count);
    }
    
    return result;
}
```

## Tips and Best Practices

1. **Performance**: For large datasets (>10,000 points), consider downsampling or using line plots without points (`showPoints = false`)

2. **Color Selection**: Use contrasting colors for multiple series. The library automatically assigns different colors to each series.

3. **Margins**: Adjust margins if labels are cut off:
   ```cpp
   config.marginLeft = 80;   // For long Y-axis labels
   config.marginBottom = 80; // For long X-axis labels
   ```

4. **Data Range**: The library automatically computes the data range with 5% padding. Data points outside the visible range will be clipped.

5. **File Format**: Currently only PNG format is supported. The filename should end with `.png`.

## Troubleshooting

### Build Issues

**Problem**: CMake can't find Skia
```
Solution: Set SKIA_DIR environment variable:
export SKIA_DIR=/path/to/skia
```

**Problem**: Linking errors
```
Solution: Make sure Skia is built with the same compiler and standard library
```

### Runtime Issues

**Problem**: Plot file not created
```
Solution: Check write permissions and disk space
```

**Problem**: Empty plot
```
Solution: Verify data series contain points before rendering
```

## Further Resources

- [Skia Documentation](https://skia.org/docs/)
- [SkiaPlot GitHub Repository](https://github.com/huapeng01016/SkiaPlot)
- Example programs in the `examples/` directory
