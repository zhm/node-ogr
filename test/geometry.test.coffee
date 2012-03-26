#!/usr/bin/env coffee

ogr = require 'ogr'
_   = require 'underscore'

#ogr.quiet()

describe 'Geometry', ->
  valid_file = './test/support/valid_shapefile.shp'
  layer =  ogr.open(valid_file).getLayer()
  geom = layer.getNextFeature().getGeometry()
  geom_other = layer.getNextFeature().getGeometry()

  beforeEach ->
    layer.resetReading()
    geom = layer.getNextFeature().getGeometry()
    geom_other = layer.getNextFeature().getGeometry()

  it "should be an instance of Geometry", ->
    geom.should.be.an.instanceof ogr.Geometry

  it "should be able to get its dimension", ->
    geom.getDimension().should.eql(0)

  it "should be able to get its coordinate dimension", ->
    geom.getCoordinateDimension().should.eql(2)

  it "should be able to determine if it's empty", ->
    geom.isEmpty().should.be.false
    geom.empty()
    geom.isEmpty().should.be.true

  it "should be able to determine if it's valid", ->
    geom.isValid().should.be.true

  it "should be able to determine if it's simple", ->
    geom.isSimple().should.be.true

  it "should be able to determine if it's a ring", ->
    geom.isRing().should.be.false

  it "should be able to empty the geometry", ->
    geom.empty()
    geom.isEmpty().should.be.true

  it "should be able to get the WKB size", ->
    geom.wkbSize().should.eql(21)

  it "should be able to export to GML", ->
    geom.exportToGML().should.eql("<gml:Point><gml:coordinates>-93.822760566629469,29.644603855661842</gml:coordinates></gml:Point>")

  it "should be able to export to KML", ->
    geom.exportToKML().should.eql("<Point><coordinates>-93.822760566629469,29.644603855661842</coordinates></Point>")

  it "should be able to export to JSON", ->
    geom.exportToJSON().should.eql('{ "type": "Point", "coordinates": [ -93.822760566629469, 29.644603855661842 ] }')

  it "should be able to get the geometry type", ->
    geom.getGeometryType().should.eql(ogr.wkbPoint)

  it "should be able perform spatial comparisons", ->
    comparisons =
      'intersects' : false
      'equals'     : false
      'disjoint'   : true
      'touches'    : false
      'crosses'    : false
      'within'     : false
      'contains'   : false
      'overlaps'   : false

    _.each comparisons, (correct_result, comparison) ->
      geom[comparison](geom_other).should.eql(correct_result)
      geom_other[comparison](geom).should.eql(correct_result)

  it "should be able to compute the convex hull", ->
    geom.convexHull().should.be.an.instanceof ogr.Geometry

  it "should be able to compute a buffer", ->
    geom.buffer(13.37).should.be.an.instanceof ogr.Geometry
    geom.buffer(13.37, 10).should.be.an.instanceof ogr.Geometry

    (-> geom.buffer()).should.throw()
    (-> geom.buffer(1, 13.1)).should.throw()
    (-> geom.buffer("a string parameter")).should.throw()
    (-> geom.buffer({})).should.throw()

  it "should be able to perform an intersection", ->
    geom.intersection(geom_other).should.be.an.instanceof ogr.Geometry

    (-> geom.intersection()).should.throw()
    (-> geom.intersection(1, 13.1)).should.throw()
    (-> geom.intersection("a string parameter")).should.throw()
    (-> geom.intersection({})).should.throw()

  it "should be able to perform a union", ->
    geom.union(geom_other).should.be.an.instanceof ogr.Geometry

    (-> geom.union()).should.throw()
    (-> geom.union(1, 13.1)).should.throw()
    (-> geom.union("a string parameter")).should.throw()
    (-> geom.union({})).should.throw()

  it "should be able to perform a cascaded union", ->
    #geom.unionCascaded(geom_other).should.be.an.instanceof ogr.Geometry

    (-> geom.unionCascaded()).should.throw()
    (-> geom.unionCascaded(1, 13.1)).should.throw()
    (-> geom.unionCascaded("a string parameter")).should.throw()
    (-> geom.unionCascaded({})).should.throw()

  it "should be able to perform a difference", ->
    geom.difference(geom_other).should.be.an.instanceof ogr.Geometry

    (-> geom.difference()).should.throw()
    (-> geom.difference(1, 13.1)).should.throw()
    (-> geom.difference("a string parameter")).should.throw()
    (-> geom.difference({})).should.throw()

  it "should be able to perform a sym difference", ->
    geom.symDifference(geom_other).should.be.an.instanceof ogr.Geometry

    (-> geom.symDifference()).should.throw()
    (-> geom.symDifference(1, 13.1)).should.throw()
    (-> geom.symDifference("a string parameter")).should.throw()
    (-> geom.symDifference({})).should.throw()

  it "should be able to be simplified", ->
    geom.simplify(1.5).should.be.an.instanceof ogr.Geometry

    (-> geom.simplify()).should.throw()
    (-> geom.simplify("a string parameter")).should.throw()
    (-> geom.simplify({})).should.throw()

  it "should be able to be simplified preserving topology", ->
    geom.simplifyPreserveTopology(1.5).should.be.an.instanceof ogr.Geometry

    (-> geom.simplifyPreserveTopology()).should.throw()
    (-> geom.simplifyPreserveTopology("a string parameter")).should.throw()
    (-> geom.simplifyPreserveTopology({})).should.throw()

  it "should be able to be cloned", ->
    geom.clone().should.be.an.instanceof(ogr.Geometry).and.should.not.equal(geom)
    geom.clone().equals(geom).should.be.true

  it "should be equal to the same geometry", ->
    geom.clone().equals(geom).should.be.true
