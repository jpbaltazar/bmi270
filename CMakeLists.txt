cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app)

# special include just to get the header
zephyr_include_directories(${ZEPHYR_BASE}/drivers/sensor/bosch/bmi270)

target_sources(app PRIVATE src/bmi_wrapper.c)
target_sources(app PRIVATE src/main.c)
