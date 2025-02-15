# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(wound_theropy)

#target_sources(app PRIVATE src/spi.c)
#target_sources(app PRIVATE src/gpio.c)
target_sources(app PRIVATE src/i2c.c)
#target_sources(app PRIVATE src/bluetooth.c)
#target_sources(app PRIVATE src/uart.c)
