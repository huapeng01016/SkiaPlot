#include "skiaplot.h"
#include <iostream>

int main() {
    std::cout << "Creating a simple plot..." << std::endl;
    
    // Create some sample data
    std::vector<double> x = {0, 1, 2, 3, 4, 5};
    std::vector<double> y = {0, 1, 4, 9, 16, 25};
    
    // Use quick plot utility
    if (SkiaPlot::Utils::quickPlot(x, y, "simple_plot.png", "Simple Plot: y = x²")) {
        std::cout << "Plot saved to simple_plot.png" << std::endl;
        return 0;
    } else {
        std::cerr << "Failed to create plot" << std::endl;
        return 1;
    }
}
