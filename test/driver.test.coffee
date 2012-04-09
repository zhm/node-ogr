#!/usr/bin/env coffee

ogr = require 'ogr'
_   = require 'underscore'

#ogr.quiet()

describe 'Driver', ->
  driver = null
  valid_file = './test/support/valid_shapefile.shp'

  beforeEach ->
    driver = ogr.getDriverByName("ESRI Shapefile")

  #it "should be an instance of Driver", ->
    #driver.should.be.an.instanceof ogr.Driver

  #it "should be able get its name", ->
    #driver.getName().should.eql("ESRI Shapefile")

  it "should be able to open a data source", ->
    ds = driver.open(valid_file)
    ds.should.be.an.instanceof ogr.Datasource

  #it "should be able to test for a capability", ->
    #driver.testCapability(ogr.CreateDataSourceOption).should.be.true
    #driver.testCapability(ogr.DeleteDataSourceOption).should.be.true

    #driver.testCapability("an invalid capability string").should.be.false
    #(-> driver.testCapability(1)).should.throw()
    #(-> driver.testCapability(['an array instead of a string'])).should.throw

  #it "should be able to create a new data source", ->
    #driver.createDataSource("test_output.shp").should.be.an.instanceof ogr.Datasource
