#!/usr/bin/env coffee

ogr  = require '../'
fs   = require 'fs'
path = require 'path'
_    = require 'underscore'

#ogr.quiet()

describe 'DataSource', ->
  valid_file = './test/support/valid_shapefile.shp'
  ds = null

  beforeEach ->
    ds = ogr.open(valid_file)

  it "should be an instance of Datasource", ->
    ds.should.be.an.instanceof ogr.Datasource

  it "should be able to get its name", ->
    ds.getName().should.eql("./test/support/valid_shapefile.shp")

  it "should be able to get the layer count", ->
    ds.getLayerCount().should.eql(1)

  it "should be able to get a layer by name", ->
    ds.getLayerByName("valid_shapefile").should.be.an.instanceof ogr.Layer

  it "should be able to get a layer by index", ->
    ds.getLayer(0).should.be.an.instanceof ogr.Layer

  it "should be able to sync changes to disk", ->
    ds.syncToDisk()

  it "should be able to execute SQL statements", ->
    layer = ds.executeSQL("SELECT * FROM valid_shapefile")
    layer.should.be.an.instanceof ogr.Layer
    ds.releaseResultSet(layer)

  it "should be able test capabilities", ->
    ds.testCapability("ODsCCreateLayer").should.be.false
    ds.testCapability("ODsCDeleteLayer:").should.be.false

  it "should be able to copy a layer", ->
    layer = ds.getLayer(0)
    layer.should.be.an.instanceof ogr.Layer

    if path.existsSync("./test/support/test_output.json")
      fs.unlinkSync("./test/support/test_output.json")

    json_driver = ogr.getDriverByName('GeoJSON')
    json_ds = json_driver.createDataSource('./test/support/test_output.json')
    json_ds.should.be.an.instanceof ogr.Datasource
    json_layer = json_ds.copyLayer(layer, "json_layer")
    json_layer.should.be.an.instanceof ogr.Layer

    fs.unlinkSync("./test/support/test_output.json")

  it "should be able to create a new layer", ->
    #layer = ds.createLayer("test_layer")
    #layer.should.be.an.instanceof ogr.Layer
