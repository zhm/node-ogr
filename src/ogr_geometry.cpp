
#include "ogr_common.hpp"
#include "ogr_geometry.hpp"

// boost
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>

Persistent<FunctionTemplate> Geometry::constructor;

void Geometry::Initialize(Handle<Object> target) {
  HandleScope scope;

  constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Geometry::New));
  constructor->InstanceTemplate()->SetInternalFieldCount(1);
  constructor->SetClassName(String::NewSymbol("Geometry"));

  NODE_SET_PROTOTYPE_METHOD(constructor, "toString", toString);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getDimension", getDimension);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getCoordinateDimension", getCoordinateDimension);
  NODE_SET_PROTOTYPE_METHOD(constructor, "isEmpty", isEmpty);
  NODE_SET_PROTOTYPE_METHOD(constructor, "isValid", isValid);
  NODE_SET_PROTOTYPE_METHOD(constructor, "isSimple", isSimple);
  NODE_SET_PROTOTYPE_METHOD(constructor, "isRing", isRing);
  NODE_SET_PROTOTYPE_METHOD(constructor, "clone", clone);
  NODE_SET_PROTOTYPE_METHOD(constructor, "empty", empty);
  NODE_SET_PROTOTYPE_METHOD(constructor, "wkbSize", wkbSize);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getGeometryType", getGeometryType);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getGeometryName", getGeometryName);
  NODE_SET_PROTOTYPE_METHOD(constructor, "exportToKML", exportToKML);
  NODE_SET_PROTOTYPE_METHOD(constructor, "exportToGML", exportToGML);
  NODE_SET_PROTOTYPE_METHOD(constructor, "exportToJSON", exportToJSON);
  NODE_SET_PROTOTYPE_METHOD(constructor, "exportToWKT", exportToWKT);
  NODE_SET_PROTOTYPE_METHOD(constructor, "closeRings", closeRings);
  NODE_SET_PROTOTYPE_METHOD(constructor, "intersects", intersects);
  NODE_SET_PROTOTYPE_METHOD(constructor, "equals", equals);
  NODE_SET_PROTOTYPE_METHOD(constructor, "disjoint", disjoint);
  NODE_SET_PROTOTYPE_METHOD(constructor, "touches", touches);
  NODE_SET_PROTOTYPE_METHOD(constructor, "crosses", crosses);
  NODE_SET_PROTOTYPE_METHOD(constructor, "within", within);
  NODE_SET_PROTOTYPE_METHOD(constructor, "contains", contains);
  NODE_SET_PROTOTYPE_METHOD(constructor, "overlaps", overlaps);
  NODE_SET_PROTOTYPE_METHOD(constructor, "boundary", boundary);
  NODE_SET_PROTOTYPE_METHOD(constructor, "distance", distance);
  NODE_SET_PROTOTYPE_METHOD(constructor, "convexHull", convexHull);
  NODE_SET_PROTOTYPE_METHOD(constructor, "buffer", buffer);
  NODE_SET_PROTOTYPE_METHOD(constructor, "intersection", intersection);
  NODE_SET_PROTOTYPE_METHOD(constructor, "union", unionGeometry);
  NODE_SET_PROTOTYPE_METHOD(constructor, "unionCascaded", unionCascaded);
  NODE_SET_PROTOTYPE_METHOD(constructor, "difference", difference);
  NODE_SET_PROTOTYPE_METHOD(constructor, "symDifference", symDifference);
  NODE_SET_PROTOTYPE_METHOD(constructor, "centroid", centroid);
  NODE_SET_PROTOTYPE_METHOD(constructor, "simplify", simplify);
  NODE_SET_PROTOTYPE_METHOD(constructor, "simplifyPreserveTopology", simplifyPreserveTopology);
  NODE_SET_PROTOTYPE_METHOD(constructor, "polygonize", polygonize);
  NODE_SET_PROTOTYPE_METHOD(constructor, "segmentize", segmentize);
  NODE_SET_PROTOTYPE_METHOD(constructor, "swapXY", swapXY);

  target->Set(String::NewSymbol("Geometry"), constructor->GetFunction());
}

Geometry::Geometry(OGRGeometry *layer)
: ObjectWrap(),
  this_(layer),
  owned_(true),
  size_(0)
{}

Geometry::Geometry()
: ObjectWrap(),
  this_(NULL),
  owned_(true),
  size_(0)
{
}

Geometry::~Geometry()
{
  if (owned_) {
    OGRGeometryFactory::destroyGeometry(this_);
    V8::AdjustAmountOfExternalAllocatedMemory(-size_);
  }
}

Handle<Value> Geometry::New(const Arguments& args)
{
  HandleScope scope;

  if (!args.IsConstructCall())
      return ThrowException(String::New("Cannot call constructor as function, you need to use 'new' keyword"));

  if (args[0]->IsExternal()) {
      Local<External> ext = Local<External>::Cast(args[0]);
      void* ptr = ext->Value();
      Geometry *f = static_cast<Geometry *>(ptr);
      f->Wrap(args.This());
      return args.This();
  }

  return args.This();
}

Handle<Value> Geometry::New(OGRGeometry *geom) {
  v8::HandleScope scope;
  Geometry *wrapped = new Geometry(geom);
  wrapped->size_ = geom->WkbSize();
  V8::AdjustAmountOfExternalAllocatedMemory(wrapped->size_);

  v8::Handle<v8::Value> ext = v8::External::New(wrapped);
  v8::Handle<v8::Object> obj = Geometry::constructor->GetFunction()->NewInstance(1, &ext);

  return scope.Close(obj);
}

Handle<Value> Geometry::New(OGRGeometry *geom, bool owned) {
  v8::HandleScope scope;
  Geometry *wrapped = new Geometry(geom);
  wrapped->owned_ = owned;
  v8::Handle<v8::Value> ext = v8::External::New(wrapped);
  v8::Handle<v8::Object> obj = Geometry::constructor->GetFunction()->NewInstance(1, &ext);
  return scope.Close(obj);
}

Handle<Value> Geometry::toString(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(String::New(geom->this_->exportToJson()));
}


NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, getDimension, Integer, getDimension);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, getCoordinateDimension, Integer, getCoordinateDimension);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, isEmpty, Boolean, IsEmpty);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, isValid, Boolean, IsValid);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, isSimple, Boolean, IsSimple);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, isRing, Boolean, IsRing);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, clone, Geometry, clone);
NODE_WRAPPED_METHOD(Geometry, empty, empty);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, wkbSize, Integer, WkbSize);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, getGeometryType, Integer, getGeometryType);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, getGeometryName, String, getGeometryName);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, exportToKML, String, exportToKML);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, exportToGML, String, exportToGML);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, exportToJSON, String, exportToJson);
NODE_WRAPPED_METHOD(Geometry, closeRings, closeRings);
NODE_WRAPPED_METHOD_WITH_1_DOUBLE_PARAM(Geometry, segmentize, segmentize, "segment length");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, intersects, Boolean, Intersects, Geometry, "geometry to compare");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, equals, Boolean, Equals, Geometry, "geometry to compare");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, disjoint, Boolean, Disjoint, Geometry, "geometry to compare");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, touches, Boolean, Touches, Geometry, "geometry to compare");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, crosses, Boolean, Crosses, Geometry, "geometry to compare");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, within, Boolean, Within, Geometry, "geometry to compare");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, contains, Boolean, Contains, Geometry, "geometry to compare");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, overlaps, Boolean, Overlaps, Geometry, "geometry to compare");
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, boundary, Geometry, Boundary);
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, distance, Number, Distance, Geometry, "geometry to use for distance calculation");
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, convexHull, Geometry, ConvexHull);
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, intersection, Geometry, Intersection, Geometry, "geometry to use for intersection");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, unionGeometry, Geometry, Union, Geometry, "geometry to use for union");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, difference, Geometry, Difference, Geometry, "geometry to use for difference");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Geometry, symDifference, Geometry, SymDifference, Geometry, "geometry to use for sym difference");
NODE_WRAPPED_METHOD_WITH_RESULT_1_DOUBLE_PARAM(Geometry, simplify, Geometry, Simplify, "tolerance");
NODE_WRAPPED_METHOD_WITH_RESULT_1_DOUBLE_PARAM(Geometry, simplifyPreserveTopology, Geometry, SimplifyPreserveTopology, "tolerance");
NODE_WRAPPED_METHOD(Geometry, swapXY, swapXY);

//manually wrap this method because we don't have macros for multiple params
Handle<Value> Geometry::buffer(const Arguments& args)
{
  HandleScope scope;

  double distance;
  int number_of_segments = 30;

  NODE_ARG_DOUBLE(0, "distance", distance);
  NODE_ARG_INT_OPT(1, "number of segments", number_of_segments);

  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());

  return scope.Close(Geometry::New(geom->this_->Buffer(distance, number_of_segments)));
}



//manually wrap this method because we don't have macros for multiple params
Handle<Value> Geometry::exportToWKT(const Arguments& args)
{
  HandleScope scope;

  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());

  char *text = NULL;
  geom->this_->exportToWkt(&text);

  if (text) {
    return scope.Close(String::New(text));
  }

  return Undefined();
}


Handle<Value> Geometry::unionCascaded(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());

  if (geom->this_->getGeometryType() != wkbMultiPolygon) {
    return NODE_THROW("Geometry must be a MultiPolygon for a cascaded union.");
  }

  return scope.Close(Geometry::New(geom->this_->UnionCascaded()));
}


// The Centroid method wants the caller to create the point to fill in. Instead
// of requiring the caller to create the point geometry to fill in, we new up an
// OGRPoint and put the result into it and return that.
Handle<Value> Geometry::centroid(const Arguments& args)
{
  HandleScope scope;
  OGRPoint *point = new OGRPoint();

  ObjectWrap::Unwrap<Geometry>(args.This())->this_->Centroid(point);

  return scope.Close(Geometry::New(point));
}


Handle<Value> Geometry::polygonize(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());

  if (geom->this_->getGeometryType() != wkbMultiLineString) {
    return NODE_THROW("Geometry must be a wkbMultiLineString to polygonize.");
  }

  OGRGeometry *polygonized = geom->this_->Polygonize();

  if (polygonized != NULL) {
    return scope.Close(Geometry::New(polygonized));
  } else {
    return Null();
  }
}
