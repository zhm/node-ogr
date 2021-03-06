#!/usr/bin/env coffee

ogr = require '../'

describe 'Feature', ->
  valid_file = './test/support/valid_shapefile.shp'
  ds = ogr.open(valid_file)
  layer =  ds.getLayer(0)
  feature = layer.getFeature(0)
  feature_other = layer.getFeature(1)

  it "should be an instance of Feature", ->
    feature.should.be.an.instanceof ogr.Feature

  it "should be able to access its geometry", ->
    feature.getGeometry().should.be.an.instanceof ogr.Geometry

  it "should be able to clone itself", ->
    feature.clone().should.be.an.instanceof ogr.Feature

  it "should be able to set its geometry", ->
    feature2 = layer.getFeature(1)
    feature.clone().setGeometry(feature2.getGeometry()).should.eql(0)
    feature.clone().setGeometryDirectly(feature2.stealGeometry()).should.eql(0)

    #TODO: figure out ownership semantics of stealGeometry/setGeometryDirectly
    #it "should be able to steal another feature's geometry", ->
    #feature2 = layer.getFeature(5)
    #stolen_geom = feature2.stealGeometry()
    #feature.clone().setGeometryDirectly(stolen_geom).should.eql(0)
    #feature.clone().setGeometryDirectly(stolen_geom.clone()).should.eql(0)
    #feature.clone().setGeometry(stolen_geom.clone()).should.eql(0)
    #feature.clone().setGeometry(stolen_geom).should.eql(0)

  it "should equal its clone", ->
    feature.clone().equal(feature).should.be.true

  it "should not equal a different feature", ->
    a_different_feature = layer.getFeature(1)
    feature.equal(a_different_feature).should.be.false

  it "should return the field count", ->
    feature.getFieldCount().should.eql(18)

  it "should return the field index for a field by name", ->
    feature.getFieldIndex("symbol").should.eql(17)

  it "should be able to determine if a given field is set", ->
    feature.isFieldSet(0).should.be.true
    feature.isFieldSet(16).should.be.false

  it "should be able to unset a given field", ->
    (-> feature.unsetField(1)).should.not.throw()

  it "should be able to retrieve a field as an integer", ->
    feature.getFieldAsInteger(feature.getFieldIndex("district")).should.eql(8)

  it "should be able to retrieve a field as an double", ->
    feature.getFieldAsDouble(feature.getFieldIndex("district")).should.eql(8.00)

  it "should be able to retrieve a field as an string", ->
    feature.getFieldAsString(feature.getFieldIndex("source")).should.eql("8th District")

  it "should be able to set a field", ->
    field_index = feature.getFieldIndex("source")
    a_new_value = "A new value"

    feature.setField(field_index, a_new_value)
    feature.getFieldAsString(field_index).should.eql(a_new_value)

    field_index = feature.getFieldIndex("district")
    a_new_value = 9

    feature.setField(field_index, a_new_value)
    feature.getFieldAsInteger(field_index).should.eql(a_new_value)

  it "should be able to retrieve its feature id", ->
    feature.getFID().should.eql(0)

  it "should be able to set its feature id", ->
    feature.setFID(1000)
    feature.getFID().should.eql(1000)

  # it "should be able to be set from another feature", ->
  #   feature.setFrom(feature_other).should.eql ogr.Success
  #   feature.setFrom(feature_other, true).should.eql ogr.Success
  #   feature.setFrom(feature_other, false).should.eql ogr.Success
  #   feature.setFrom(feature_other, [0..16], false).should.eql ogr.Success
  #   feature.setFrom(feature_other, [0..16], true).should.eql ogr.Success
  #   feature.setFrom(feature_other, [0, 1], true).should.eql ogr.Failure

  #   (-> feature.setFrom()).should.throw()
  #   (-> feature.setFrom({})).should.throw()
  #   (-> feature.setFrom(feature_other, '2nd parameter must be a boolean or an array of integers')).should.throw()
  #   (-> feature.setFrom(feature_other, {})).should.throw()
  #   (-> feature.setFrom(feature_other, ['an array but not of integers'])).should.throw()
  #   (-> feature.setFrom(feature_other, ['an array but not of integers'], true)).should.throw()
  #   (-> feature.setFrom(feature_other, [0..16], 'an integer array but not boolean 3rd parameter')).should.throw()
