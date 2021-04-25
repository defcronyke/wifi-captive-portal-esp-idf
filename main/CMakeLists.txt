# wifi-captive-portal-esp-idf
#
# Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>
#
# This code is released under the license terms contained in the
# file named LICENSE, which is found in the top-level folder in
# this project. You must agree to follow those license terms,
# otherwise you aren't allowed to copy, distribute, or use any 
# part of this project in any way.

idf_component_register(
  SRCS
    "main.c"

    ${project_dir}/components/wifi-captive-portal-esp-idf-component/wifi-captive-portal-esp-idf.c
  
  REQUIRES
    "wifi-captive-portal-esp-idf-component"

  INCLUDE_DIRS
    ${project_dir}/components/wifi-captive-portal-esp-idf-component/include
)

set(
  COMPONENTS_DIRS,
    ${project_dir}/components
)

# add_compile_definitions(
# )
