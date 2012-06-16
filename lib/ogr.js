try {
  // node 0.6+
  module.exports = require('../build/Release/ogr');
} catch (e) {
  // node 0.4.x
  module.exports = require('../build/default/ogr');
}
