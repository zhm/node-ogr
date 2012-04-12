#!/usr/bin/env coffee

ogr = require 'ogr'
_   = require 'underscore'

#ogr.quiet()

describe 'FeatureDefn', ->
  valid_file = './test/support/valid_shapefile.shp'
  ds = ogr.open(valid_file)
  layer =  ds.getLayer(0)
  defn  = layer.getLayerDefn()

  it "should be an instance of FeatureDefn", ->
    defn.should.be.an.instanceof ogr.FeatureDefn

  it "should be able get its name", ->
    defn.getName().should.eql("valid_shapefile")

  it "should be able to get its field count", ->
    defn.getFieldCount().should.eql(18)

  it "should be able to get a field index", ->
    defn.getFieldIndex("a_field_that_doesnt_exist").should.eql(-1)
    defn.getFieldIndex("source").should.eql(14)

  it "should be able to get its geometry type", ->
    defn.getGeomType().should.eql(ogr.wkbPoint)

  it "should be able to set its geometry type", ->
    defn.setGeomType(ogr.wkbPoint)

    (-> defn.setGeomType("an invalid geometry type parameter")).should.throw()

  it "should be able to clone itself", ->
    defn.clone().should.be.an.instanceof(ogr.FeatureDefn).and.not.equal(defn)

  it "should be able to determine if the geometry field is ignored", ->
    defn.isGeometryIgnored().should.be.false

  it "should be able to set the ignore geometry indicator", ->
    defn.setGeometryIgnored(true)
    defn.isGeometryIgnored().should.be.true

    defn.setGeometryIgnored(false)
    defn.isGeometryIgnored().should.be.false

    (-> defn.setGeometryIgnored("an invalid parameter")).should.throw()

  it "should be able to determine if the style is ignored", ->
    defn.isStyleIgnored().should.be.false

  it "should be able to set the ignore style indicator", ->
    defn.setStyleIgnored(true)
    defn.isStyleIgnored().should.be.true

    defn.setStyleIgnored(false)
    defn.isStyleIgnored().should.be.false

    (-> defn.setStyleIgnored("an invalid parameter")).should.throw()

  it "should be able to reorder the fields", ->
    defn.reorderFieldDefns([0, 2, 1])

    (-> defn.reorderFieldDefns("a string parameter")).should.throw()
    (-> defn.reorderFieldDefns({})).should.throw()


  it "should be able to add a new field", ->
    new_field = new ogr.FieldDefn("new_field_test", ogr.OFTString)
    defn.addFieldDefn(new_field)
    new_field = null

