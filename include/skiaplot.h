#ifndef SKIAPLOT_H
#define SKIAPLOT_H

#include <vector>
#include <string>
#include <memory>
#include <cmath>

// Forward declarations for Skia types
class SkCanvas;
class SkSurface;
class SkPaint;
class SkPath;

namespace SkiaPlot {

/**
 * @brief Represents a data point with x and y coordinates
 */
struct Point {
    double x;
    double y;
    
    Point() : x(0.0), y(0.0) {}
    Point(double x_, double y_) : x(x_), y(y_) {}
};

/**
 * @brief Represents a series of data points to be plotted
 */
class DataSeries {
public:
    DataSeries(const std::string& name = "Data");
    
    void addPoint(double x, double y);
    void addPoints(const std::vector<Point>& points);
    void setPoints(const std::vector<Point>& points);
    
    const std::vector<Point>& getPoints() const { return points_; }
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    // Get data range
    void getRange(double& xMin, double& xMax, double& yMin, double& yMax) const;
    
private:
    std::string name_;
    std::vector<Point> points_;
};

/**
 * @brief Configuration for plot appearance
 */
struct PlotConfig {
    // Canvas size
    int width = 800;
    int height = 600;
    
    // Margins
    int marginLeft = 60;
    int marginRight = 40;
    int marginTop = 40;
    int marginBottom = 60;
    
    // Colors (ARGB format)
    uint32_t backgroundColor = 0xFFFFFFFF;  // White
    uint32_t axisColor = 0xFF000000;        // Black
    uint32_t gridColor = 0xFFCCCCCC;        // Light gray
    uint32_t lineColor = 0xFF0000FF;        // Blue
    
    // Line properties
    float lineWidth = 2.0f;
    bool showGrid = true;
    bool showPoints = true;
    float pointRadius = 4.0f;
    
    // Labels
    std::string title;
    std::string xLabel;
    std::string yLabel;
    
    PlotConfig() = default;
};

/**
 * @brief Main plotting class that renders data using Skia
 */
class Plot {
public:
    Plot(int width = 800, int height = 600);
    ~Plot();
    
    // Configuration
    void setConfig(const PlotConfig& config);
    PlotConfig& getConfig() { return config_; }
    
    // Data management
    void addSeries(const DataSeries& series);
    void clearSeries();
    
    // Rendering
    bool render();
    bool saveToFile(const std::string& filename);
    
    // Get the canvas for custom drawing
    SkCanvas* getCanvas();
    
private:
    void setupCanvas();
    void drawBackground(SkCanvas* canvas);
    void drawGrid(SkCanvas* canvas);
    void drawAxes(SkCanvas* canvas);
    void drawSeries(SkCanvas* canvas, const DataSeries& series, uint32_t color);
    void drawLabels(SkCanvas* canvas);
    
    // Transform data coordinates to canvas coordinates
    void dataToCanvas(double x, double y, float& canvasX, float& canvasY) const;
    
    PlotConfig config_;
    std::vector<DataSeries> series_;
    std::unique_ptr<SkSurface> surface_;
    
    // Data range for scaling
    double xMin_, xMax_, yMin_, yMax_;
    bool rangeComputed_;
    
    void computeDataRange();
};

/**
 * @brief Helper functions for common plotting tasks
 */
namespace Utils {
    /**
     * @brief Generate linearly spaced points
     */
    std::vector<Point> linspace(double start, double end, int num);
    
    /**
     * @brief Create a simple line plot
     */
    bool quickPlot(const std::vector<double>& x, 
                   const std::vector<double>& y,
                   const std::string& filename,
                   const std::string& title = "");
}

} // namespace SkiaPlot

#endif // SKIAPLOT_H
