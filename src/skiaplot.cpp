#include "skiaplot.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontMetrics.h"
#include "include/core/SkData.h"
#include "include/core/SkStream.h"
#include "include/encode/SkPngEncoder.h"
#include "include/core/SkColor.h"
#include <algorithm>
#include <limits>
#include <sstream>
#include <iomanip>

namespace SkiaPlot {

// ============================================================================
// DataSeries Implementation
// ============================================================================

DataSeries::DataSeries(const std::string& name) : name_(name) {}

void DataSeries::addPoint(double x, double y) {
    points_.emplace_back(x, y);
}

void DataSeries::addPoints(const std::vector<Point>& points) {
    points_.insert(points_.end(), points.begin(), points.end());
}

void DataSeries::setPoints(const std::vector<Point>& points) {
    points_ = points;
}

void DataSeries::getRange(double& xMin, double& xMax, double& yMin, double& yMax) const {
    if (points_.empty()) {
        xMin = xMax = yMin = yMax = 0.0;
        return;
    }
    
    xMin = xMax = points_[0].x;
    yMin = yMax = points_[0].y;
    
    for (const auto& p : points_) {
        xMin = std::min(xMin, p.x);
        xMax = std::max(xMax, p.x);
        yMin = std::min(yMin, p.y);
        yMax = std::max(yMax, p.y);
    }
}

// ============================================================================
// Plot Implementation
// ============================================================================

Plot::Plot(int width, int height)
    : xMin_(0.0), xMax_(1.0), yMin_(0.0), yMax_(1.0), rangeComputed_(false) {
    config_.width = width;
    config_.height = height;
}

Plot::~Plot() = default;

void Plot::setConfig(const PlotConfig& config) {
    config_ = config;
    surface_.reset();  // Force recreation with new size
}

void Plot::addSeries(const DataSeries& series) {
    series_.push_back(series);
    rangeComputed_ = false;
}

void Plot::clearSeries() {
    series_.clear();
    rangeComputed_ = false;
}

void Plot::setupCanvas() {
    if (!surface_ || surface_->width() != config_.width || 
        surface_->height() != config_.height) {
        surface_ = SkSurface::MakeRasterN32Premul(config_.width, config_.height);
    }
}

void Plot::computeDataRange() {
    if (rangeComputed_ || series_.empty()) {
        return;
    }
    
    bool first = true;
    for (const auto& series : series_) {
        double sXMin, sXMax, sYMin, sYMax;
        series.getRange(sXMin, sXMax, sYMin, sYMax);
        
        if (series.getPoints().empty()) {
            continue;
        }
        
        if (first) {
            xMin_ = sXMin;
            xMax_ = sXMax;
            yMin_ = sYMin;
            yMax_ = sYMax;
            first = false;
        } else {
            xMin_ = std::min(xMin_, sXMin);
            xMax_ = std::max(xMax_, sXMax);
            yMin_ = std::min(yMin_, sYMin);
            yMax_ = std::max(yMax_, sYMax);
        }
    }
    
    // Add some padding to the range
    double xPadding = (xMax_ - xMin_) * 0.05;
    double yPadding = (yMax_ - yMin_) * 0.05;
    
    if (xPadding == 0.0) xPadding = 0.5;
    if (yPadding == 0.0) yPadding = 0.5;
    
    xMin_ -= xPadding;
    xMax_ += xPadding;
    yMin_ -= yPadding;
    yMax_ += yPadding;
    
    rangeComputed_ = true;
}

void Plot::dataToCanvas(double x, double y, float& canvasX, float& canvasY) const {
    int plotWidth = config_.width - config_.marginLeft - config_.marginRight;
    int plotHeight = config_.height - config_.marginTop - config_.marginBottom;
    
    canvasX = config_.marginLeft + 
              (x - xMin_) / (xMax_ - xMin_) * plotWidth;
    canvasY = config_.marginTop + plotHeight - 
              (y - yMin_) / (yMax_ - yMin_) * plotHeight;
}

void Plot::drawBackground(SkCanvas* canvas) {
    SkPaint paint;
    paint.setColor(config_.backgroundColor);
    canvas->drawRect(SkRect::MakeWH(config_.width, config_.height), paint);
}

void Plot::drawGrid(SkCanvas* canvas) {
    if (!config_.showGrid) {
        return;
    }
    
    SkPaint paint;
    paint.setColor(config_.gridColor);
    paint.setStrokeWidth(1.0f);
    paint.setStyle(SkPaint::kStroke_Style);
    
    int plotWidth = config_.width - config_.marginLeft - config_.marginRight;
    int plotHeight = config_.height - config_.marginTop - config_.marginBottom;
    
    // Vertical grid lines
    int numVerticalLines = 10;
    for (int i = 0; i <= numVerticalLines; ++i) {
        float x = config_.marginLeft + (i * plotWidth / numVerticalLines);
        canvas->drawLine(x, config_.marginTop, 
                        x, config_.height - config_.marginBottom, paint);
    }
    
    // Horizontal grid lines
    int numHorizontalLines = 10;
    for (int i = 0; i <= numHorizontalLines; ++i) {
        float y = config_.marginTop + (i * plotHeight / numHorizontalLines);
        canvas->drawLine(config_.marginLeft, y, 
                        config_.width - config_.marginRight, y, paint);
    }
}

void Plot::drawAxes(SkCanvas* canvas) {
    SkPaint paint;
    paint.setColor(config_.axisColor);
    paint.setStrokeWidth(2.0f);
    paint.setStyle(SkPaint::kStroke_Style);
    
    int plotWidth = config_.width - config_.marginLeft - config_.marginRight;
    int plotHeight = config_.height - config_.marginTop - config_.marginBottom;
    
    // X axis
    canvas->drawLine(config_.marginLeft, 
                    config_.height - config_.marginBottom,
                    config_.marginLeft + plotWidth,
                    config_.height - config_.marginBottom, paint);
    
    // Y axis
    canvas->drawLine(config_.marginLeft, config_.marginTop,
                    config_.marginLeft, 
                    config_.marginTop + plotHeight, paint);
    
    // Draw tick marks and labels
    SkFont font;
    font.setSize(12);
    paint.setStyle(SkPaint::kFill_Style);
    
    // X axis ticks
    int numXTicks = 5;
    for (int i = 0; i <= numXTicks; ++i) {
        float x = config_.marginLeft + (i * plotWidth / numXTicks);
        float y = config_.height - config_.marginBottom;
        
        // Tick mark
        canvas->drawLine(x, y, x, y + 5, paint);
        
        // Label
        double dataX = xMin_ + i * (xMax_ - xMin_) / numXTicks;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << dataX;
        std::string label = oss.str();
        
        SkRect bounds;
        font.measureText(label.c_str(), label.length(), SkTextEncoding::kUTF8, &bounds);
        canvas->drawSimpleText(label.c_str(), label.length(), SkTextEncoding::kUTF8,
                              x - bounds.width() / 2, y + 20, font, paint);
    }
    
    // Y axis ticks
    int numYTicks = 5;
    for (int i = 0; i <= numYTicks; ++i) {
        float x = config_.marginLeft;
        float y = config_.marginTop + plotHeight - (i * plotHeight / numYTicks);
        
        // Tick mark
        canvas->drawLine(x - 5, y, x, y, paint);
        
        // Label
        double dataY = yMin_ + i * (yMax_ - yMin_) / numYTicks;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << dataY;
        std::string label = oss.str();
        
        SkRect bounds;
        font.measureText(label.c_str(), label.length(), SkTextEncoding::kUTF8, &bounds);
        canvas->drawSimpleText(label.c_str(), label.length(), SkTextEncoding::kUTF8,
                              x - bounds.width() - 10, y + bounds.height() / 2, font, paint);
    }
}

void Plot::drawSeries(SkCanvas* canvas, const DataSeries& series, uint32_t color) {
    const auto& points = series.getPoints();
    if (points.empty()) {
        return;
    }
    
    SkPaint linePaint;
    linePaint.setColor(color);
    linePaint.setStrokeWidth(config_.lineWidth);
    linePaint.setStyle(SkPaint::kStroke_Style);
    linePaint.setAntiAlias(true);
    
    SkPath path;
    float canvasX, canvasY;
    
    // Start path at first point
    dataToCanvas(points[0].x, points[0].y, canvasX, canvasY);
    path.moveTo(canvasX, canvasY);
    
    // Draw lines to subsequent points
    for (size_t i = 1; i < points.size(); ++i) {
        dataToCanvas(points[i].x, points[i].y, canvasX, canvasY);
        path.lineTo(canvasX, canvasY);
    }
    
    canvas->drawPath(path, linePaint);
    
    // Draw points if enabled
    if (config_.showPoints) {
        SkPaint pointPaint;
        pointPaint.setColor(color);
        pointPaint.setStyle(SkPaint::kFill_Style);
        pointPaint.setAntiAlias(true);
        
        for (const auto& point : points) {
            dataToCanvas(point.x, point.y, canvasX, canvasY);
            canvas->drawCircle(canvasX, canvasY, config_.pointRadius, pointPaint);
        }
    }
}

void Plot::drawLabels(SkCanvas* canvas) {
    SkPaint paint;
    paint.setColor(config_.axisColor);
    
    SkFont font;
    
    // Draw title
    if (!config_.title.empty()) {
        font.setSize(18);
        SkRect bounds;
        font.measureText(config_.title.c_str(), config_.title.length(), 
                        SkTextEncoding::kUTF8, &bounds);
        canvas->drawSimpleText(config_.title.c_str(), config_.title.length(), 
                              SkTextEncoding::kUTF8,
                              (config_.width - bounds.width()) / 2, 25, font, paint);
    }
    
    // Draw X label
    if (!config_.xLabel.empty()) {
        font.setSize(14);
        SkRect bounds;
        font.measureText(config_.xLabel.c_str(), config_.xLabel.length(), 
                        SkTextEncoding::kUTF8, &bounds);
        canvas->drawSimpleText(config_.xLabel.c_str(), config_.xLabel.length(), 
                              SkTextEncoding::kUTF8,
                              (config_.width - bounds.width()) / 2, 
                              config_.height - 10, font, paint);
    }
    
    // Draw Y label (rotated)
    if (!config_.yLabel.empty()) {
        font.setSize(14);
        canvas->save();
        canvas->translate(15, config_.height / 2);
        canvas->rotate(-90);
        SkRect bounds;
        font.measureText(config_.yLabel.c_str(), config_.yLabel.length(), 
                        SkTextEncoding::kUTF8, &bounds);
        canvas->drawSimpleText(config_.yLabel.c_str(), config_.yLabel.length(), 
                              SkTextEncoding::kUTF8,
                              -bounds.width() / 2, 0, font, paint);
        canvas->restore();
    }
}

bool Plot::render() {
    setupCanvas();
    if (!surface_) {
        return false;
    }
    
    computeDataRange();
    
    SkCanvas* canvas = surface_->getCanvas();
    if (!canvas) {
        return false;
    }
    
    drawBackground(canvas);
    drawGrid(canvas);
    drawAxes(canvas);
    
    // Draw each series with different colors
    uint32_t colors[] = {
        0xFF0000FF,  // Blue
        0xFFFF0000,  // Red
        0xFF00AA00,  // Green
        0xFFFF8800,  // Orange
        0xFF8800FF,  // Purple
    };
    
    for (size_t i = 0; i < series_.size(); ++i) {
        uint32_t color = colors[i % 5];
        drawSeries(canvas, series_[i], color);
    }
    
    drawLabels(canvas);
    
    return true;
}

bool Plot::saveToFile(const std::string& filename) {
    if (!render()) {
        return false;
    }
    
    sk_sp<SkImage> image = surface_->makeImageSnapshot();
    if (!image) {
        return false;
    }
    
    sk_sp<SkData> data = SkPngEncoder::Encode(nullptr, image.get(), {});
    if (!data) {
        return false;
    }
    
    SkFILEWStream stream(filename.c_str());
    if (!stream.isValid()) {
        return false;
    }
    
    return stream.write(data->data(), data->size());
}

SkCanvas* Plot::getCanvas() {
    setupCanvas();
    return surface_ ? surface_->getCanvas() : nullptr;
}

// ============================================================================
// Utils Implementation
// ============================================================================

namespace Utils {

std::vector<Point> linspace(double start, double end, int num) {
    std::vector<Point> points;
    if (num <= 0) {
        return points;
    }
    
    if (num == 1) {
        points.emplace_back(start, 0.0);
        return points;
    }
    
    double step = (end - start) / (num - 1);
    for (int i = 0; i < num; ++i) {
        double x = start + i * step;
        points.emplace_back(x, 0.0);
    }
    
    return points;
}

bool quickPlot(const std::vector<double>& x, 
               const std::vector<double>& y,
               const std::string& filename,
               const std::string& title) {
    if (x.size() != y.size() || x.empty()) {
        return false;
    }
    
    Plot plot;
    DataSeries series("Data");
    
    for (size_t i = 0; i < x.size(); ++i) {
        series.addPoint(x[i], y[i]);
    }
    
    plot.addSeries(series);
    
    if (!title.empty()) {
        plot.getConfig().title = title;
    }
    
    return plot.saveToFile(filename);
}

} // namespace Utils

} // namespace SkiaPlot
