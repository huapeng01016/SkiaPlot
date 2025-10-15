#include "skiaplot.h"
#include <iostream>
#include <cmath>

int main() {
    std::cout << "Creating a sine wave plot..." << std::endl;
    
    // Create a plot object
    SkiaPlot::Plot plot(800, 600);
    
    // Configure the plot
    SkiaPlot::PlotConfig& config = plot.getConfig();
    config.title = "Sine Wave";
    config.xLabel = "x (radians)";
    config.yLabel = "sin(x)";
    config.showGrid = true;
    config.showPoints = false;
    config.lineWidth = 3.0f;
    
    // Generate sine wave data
    SkiaPlot::DataSeries series("sin(x)");
    const int numPoints = 100;
    const double start = 0.0;
    const double end = 2.0 * M_PI;
    
    for (int i = 0; i < numPoints; ++i) {
        double x = start + i * (end - start) / (numPoints - 1);
        double y = std::sin(x);
        series.addPoint(x, y);
    }
    
    plot.addSeries(series);
    
    // Save the plot
    if (plot.saveToFile("sine_wave.png")) {
        std::cout << "Plot saved to sine_wave.png" << std::endl;
        return 0;
    } else {
        std::cerr << "Failed to create plot" << std::endl;
        return 1;
    }
}
