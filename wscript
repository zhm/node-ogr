import os
import sys
from os import unlink, symlink, popen, uname, environ

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")

  linkflags = popen("gdal-config --libs").readline().strip().split(' ')
  #deps = popen("gdal-config --dep-libs").readline().strip().split(' ')

  conf.env.append_value("LINKFLAGS", linkflags)
  #conf.env.append_value("LINKFLAGS", deps)

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.cxxflags = ["-g", "-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE", "-Wall", "-Qunused-arguments"]
  obj.target = "ogr"
  obj.source = [
    "src/ogr_driver.cpp",
    "src/ogr_datasource.cpp",
    "src/ogr_layer.cpp",
    "src/ogr_feature.cpp",
    "src/ogr_feature_defn.cpp",
    "src/ogr_field_defn.cpp",
    "src/ogr_geometry.cpp",
    "src/node_ogr.cpp"
  ]
