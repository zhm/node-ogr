var ogr = require('../lib/ogr');

var driver = ogr.getDriverByName('ESRI Shapefile');
var layer  = driver.open('../test/support/valid_shapefile.shp').getLayer(0);

console.log(layer.getNextFeature().getGeometry().exportToJSON());
