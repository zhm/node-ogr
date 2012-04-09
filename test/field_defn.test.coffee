#!/usr/bin/env coffee

ogr = require 'ogr'
_   = require 'underscore'

#ogr.quiet()

describe 'FieldDefn', ->
  valid_file = './test/support/valid_shapefile.shp'
  layer =  ogr.open(valid_file).getLayer(0)
  defn  = layer.getLayerDefn().getFieldDefn(0)

  it "should be an instance of FieldDefn", ->
    defn.should.be.an.instanceof ogr.FieldDefn

  it "should be able to get the name of the field", ->
    defn.getName().should.eql("ogc_fid")

  it "should be able to set the name of the field", ->
    defn.setName("new_name")
    defn.getName().should.eql("new_name")

  it "should be able to get the type of the field", ->
    defn.getType().should.eql(0)

  it "should be able to set the type of the field", ->
    defn.setType(2)

  it "should be able to get the justification", ->
    defn.getJustify().should.eql(0)

  it "should be able to set the justification", ->
    defn.setJustify(1)

    (-> defn.setJustify("a non-integer parameter")).should.throw()

  it "should be able to get the field width", ->
    defn.getWidth().should.eql(10)

  it "should be able to set the field width", ->
    defn.setWidth(12)

    (-> defn.setWidth("a non-integer value")).should.throw()

  it "should be able to get the precision of the field", ->
    defn.getPrecision().should.eql(0)

  it "should be able to set the precision of the field", ->
    defn.setPrecision(10)

    (-> defn.setPrecision("a non-integer value")).should.throw()

  it "should be able to determine if it's ignored", ->
    defn.isIgnored().should.be.false
    defn.setIgnored(true)
    defn.isIgnored().should.be.true

  it "should be able to set the ignored flag", ->
    defn.setIgnored(true)
    defn.isIgnored().should.be.true

    defn.setIgnored(false)
    defn.isIgnored().should.be.false
