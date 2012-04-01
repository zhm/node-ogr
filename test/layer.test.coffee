#!/usr/bin/env coffee

ogr = require 'ogr'
_   = require 'underscore'

ogr.quiet()

describe 'Layer', ->
  valid_file = './test/support/valid_shapefile.shp'
  layer =  ogr.open(valid_file).getLayer()

  it "should be an instance of Layer", ->
    layer.should.be.an.instanceof ogr.Layer

  it "should be able to reset reading", ->
    -> layer.resetReading().should.not.throw()

  it "should be able to get the next feature", ->
    layer.getNextFeature().should.be.an.instanceof ogr.Feature

  it "should be able to retrieve the layer definition", ->
    layer.getLayerDefn().should.be.an.instanceof(ogr.FeatureDefn)

  it "should be able to retrieve a feature by its index", ->
    layer.getFeature(1).should.be.an.instanceof ogr.Feature

  it "should not be able to set a feature at an index on a read-only shapefile layer", ->
    feature = layer.getFeature(0)
    layer.setFeature(feature).should.not.eql(0)

  it "should not be able to create a new feature on a read-only shapefile layer", ->
    feature = layer.getFeature(0)
    feature.setFID(9999)
    layer.createFeature(feature).should.not.eql(0)

  it "should not be able to delete a feature on a read-only shapefile layer", ->
    layer.deleteFeature(0).should.not.eql(0)

  it "should be able to retrieve its name", ->
    layer.getName().should.eql("valid_shapefile")

  it "should be able retrieve the geometry type", ->
    layer.getGeomType().should.eql(1)

  it "should be able test the layer capabilities of a shapefile", ->
    capabilities = {
      'RandomRead': true
      'SequentialWrite': false
      'RandomWrite': false
      'FastSpatialFilter': false
      'FastFeatureCount': true
      'FastGetExtent': true
      'FastSetNextByIndex': true
      'CreateField': false
      'DeleteField': false
      'ReorderFields': false
      'AlterFieldDefn': false
      'DeleteFeature': false
      'StringsAsUTF8': true
      'Transactions': false
      'IgnoreFields': true
    }

    _.each capabilities, (supported, capability) ->
      layer.testCapability(capability).should.be[supported]


  it "should be able sync to disk", ->
    layer.syncToDisk()

  it "should be able to retrieve the FID column name", ->
    layer.getFIDColumn().should.eql("")

  it "should be able to retrieve the geometry column name", ->
    layer.getGeometryColumn().should.eql("")
