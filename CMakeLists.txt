# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

set(BOARD nrf5340dk_nrf5340_cpuapp)
set(ZEPHYR_TOOLCHAIN_VARIANT gnuarmemb)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)

target_sources(app PRIVATE src/main.cpp)
