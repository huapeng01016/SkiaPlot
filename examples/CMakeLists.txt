# Example programs

# Simple plot example
add_executable(simple_plot simple_plot.cpp ../src/skiaplot.cpp)
target_link_libraries(simple_plot PRIVATE skiaplot)

# Sine wave example
add_executable(sine_wave sine_wave.cpp ../src/skiaplot.cpp)
target_link_libraries(sine_wave PRIVATE skiaplot)

# Multiple series example
add_executable(multiple_series multiple_series.cpp ../src/skiaplot.cpp)
target_link_libraries(multiple_series PRIVATE skiaplot)
