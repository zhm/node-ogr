var ogr = require('ogr');
var _   = require('underscore');
var fs  = require('fs');

var database_name = 'osm_florida_all';
var table_name    = 'planet_osm_polygon';

var driver     = ogr.getDriverByName('PostgreSQL');
var datasource = driver.open('PG:dbname=' + database_name + ' host=localhost port=5432');
var layer      = datasource.getLayerByName(table_name);

var centroids = [];

while (feat = layer.getNextFeature()) {
  geom = feat.getGeometry();

  centroids.push({
    id: feat.getFID(),
    name: feat.getFieldAsString(feat.getFieldIndex('name')),
    area: geom.getArea(),
    centroid: JSON.parse(geom.centroid().exportToJSON())
  });
}

fs.writeFileSync('centroids.txt', ['id,name,area,longitude,latitude'].concat(_.map(centroids, function(o) {
  return [o.id, o.name, o.area, o.centroid.coordinates[0], o.centroid.coordinates[1]].join(',');
})).join('\n'), 'utf8');
