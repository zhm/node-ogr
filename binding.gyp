{
  'variables': {
    'node_ogr_sources': [
      "src/node_ogr.cpp",
      "src/ogr_datasource.cpp",
      "src/ogr_driver.cpp",
      "src/ogr_feature.cpp",
      "src/ogr_feature_defn.cpp",
      "src/ogr_field_defn.cpp",
      "src/ogr_geometry.cpp",
      "src/ogr_layer.cpp"
    ],
    'node_root': '/opt/node-v6.1',
    'node_root_win': 'c:\\node',
    'deps_root_win': 'c:\\dev2'
  },
  'targets': [
    {
      'target_name': 'ogr',
      'product_name': 'ogr',
      'type': 'loadable_module',
      'product_prefix': '',
      'product_extension':'node',
      'sources': [
        '<@(node_ogr_sources)',
      ],
      'defines': [
        'PLATFORM="<(OS)"',
      ],
      'conditions': [
        [ 'OS=="mac"', {
          'libraries': [
            '<!@(gdal-config --libs)'
          ],
          'include_dirs': [
          ],
          'defines': [
            '_LARGEFILE_SOURCE',
            '_FILE_OFFSET_BITS=64',
          ],
        }],
      ],
    },
  ],
}
