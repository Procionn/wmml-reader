#
#  Copyright (C) 2025 Procion ByProcion@gmail.com
#
#  This file is part of WMML reader.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#  See the LICENSE file for more details.
#
cmake_minimum_required(VERSION 3.16)

project(wmml-reader VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(
  wmml
  GIT_REPOSITORY https://github.com/Procionn/wmml2
  GIT_TAG 5367decfaa97cf0ba3de9bde0e5bd15b7d6df211
)
FetchContent_MakeAvailable(wmml)

add_executable(reader
    main.cpp
)
target_link_libraries(reader PRIVATE wmml_static)
