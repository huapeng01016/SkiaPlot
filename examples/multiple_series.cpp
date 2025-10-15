#include "skiaplot.h"
#include <iostream>
#include <cmath>

int main() {
    std::cout << "Creating a multiple series plot..." << std::endl;
    
    // Create a plot object
    SkiaPlot::Plot plot(800, 600);
    
    // Configure the plot
    SkiaPlot::PlotConfig& config = plot.getConfig();
    config.title = "Trigonometric Functions";
    config.xLabel = "x (radians)";
    config.yLabel = "y";
    config.showGrid = true;
    config.showPoints = false;
    config.lineWidth = 2.5f;
    
    const int numPoints = 100;
    const double start = 0.0;
    const double end = 2.0 * M_PI;
    
    // Generate sine wave data
    SkiaPlot::DataSeries sineSeries("sin(x)");
    for (int i = 0; i < numPoints; ++i) {
        double x = start + i * (end - start) / (numPoints - 1);
        double y = std::sin(x);
        sineSeries.addPoint(x, y);
    }
    
    // Generate cosine wave data
    SkiaPlot::DataSeries cosineSeries("cos(x)");
    for (int i = 0; i < numPoints; ++i) {
        double x = start + i * (end - start) / (numPoints - 1);
        double y = std::cos(x);
        cosineSeries.addPoint(x, y);
    }
    
    // Generate tangent wave data (limited range)
    SkiaPlot::DataSeries tangentSeries("0.5*tan(x)");
    for (int i = 0; i < numPoints; ++i) {
        double x = start + i * (end - start) / (numPoints - 1);
        double y = 0.5 * std::tan(x);
        // Limit the range to avoid extreme values
        if (y > -5.0 && y < 5.0) {
            tangentSeries.addPoint(x, y);
        }
    }
    
    // Add all series to the plot
    plot.addSeries(sineSeries);
    plot.addSeries(cosineSeries);
    plot.addSeries(tangentSeries);
    
    // Save the plot
    if (plot.saveToFile("multiple_series.png")) {
        std::cout << "Plot saved to multiple_series.png" << std::endl;
        return 0;
    } else {
        std::cerr << "Failed to create plot" << std::endl;
        return 1;
    }
}
