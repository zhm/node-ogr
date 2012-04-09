#!/usr/bin/env coffee

ogr = require 'ogr'
_   = require 'underscore'

ogr.quiet()

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
    ds.executeSQL("SELECT * FROM valid_shapefile").should.be.an.instanceof ogr.Layer

#     static Handle<Value> getName(const Arguments &args);
    #static Handle<Value> getLayerCount(const Arguments &args);
    #static Handle<Value> getLayerByName(const Arguments &args);
    #static Handle<Value> getLayer(const Arguments &args);
    #static Handle<Value> deleteLayer(const Arguments &args);
    #static Handle<Value> testCapability(const Arguments &args);
    #static Handle<Value> executeSQL(const Arguments &args);
    #static Handle<Value> syncToDisk(const Arguments &args);
    #static Handle<Value> createLayer(const Arguments &args);
    #static Handle<Value> copyLayer(const Arguments &args);
