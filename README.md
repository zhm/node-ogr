# node-ogr

[OGR](http://www.gdal.org/ogr) bindings for [node](http://nodejs.org).

```js
var ogr = require('ogr');

var driver = ogr.getDriverByName('ESRI Shapefile');
var layer  = driver.open('../test/support/valid_shapefile.shp').getLayer(0);

console.log(layer.getNextFeature().getGeometry().exportToJSON());
```

## Requirements
* node >= 0.6.0
* GDAL 1.9.x

## Installation

First install GDAL http://trac.osgeo.org/gdal/wiki/DownloadSource

As of right now you can only install it locally since it's not on npm yet.

    git clone git://github.com/zhm/node-ogr.git
    cd node-ogr
    make configure
    make

Then you can run the tests

    make test

## Examples

There are examples in `/examples` that show some basic usage. One connects to PostGIS and outputs centroids to a csv.
The best documentation for right now is the unit tests in `/test` and the [OGR API](http://www.gdal.org/ogr/ogr__api_8h.html) reference.

When running locally, you might need to specify `$NODE_PATH` so node knows where to find the binding. See the `run` script in
the `/examples` directory for more information. The other option is to explicitly pass the path of `ogr.node` to `require`.

## Notes

Don't use this in server code. All of the methods are synchronous for now until I determine which methods should make
use of the node threadpool. The API *will* change when async methods are added because they will become the default.

## License

BSD.

