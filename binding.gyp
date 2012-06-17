{
  "variables": {
    "node_ogr_sources": [
      "src/node_ogr.cpp",
      "src/ogr_datasource.cpp",
      "src/ogr_driver.cpp",
      "src/ogr_feature.cpp",
      "src/ogr_feature_defn.cpp",
      "src/ogr_field_defn.cpp",
      "src/ogr_geometry.cpp",
      "src/ogr_layer.cpp"
    ]
  },
  "targets": [
    {
      "target_name": "ogr",
      "product_name": "ogr",
      "type": "loadable_module",
      "product_prefix": "",
      "product_extension":"node",
      "sources": [
        "<@(node_ogr_sources)"
      ],
      "defines": [
        "PLATFORM='<(OS)'",
        "_LARGEFILE_SOURCE",
        "_FILE_OFFSET_BITS=64"
      ],
      "libraries": [
        "<!@(gdal-config --libs)"
      ],
      "cflags": [
        "<!@(gdal-config --cflags)"
      ]
    }
  ]
}
