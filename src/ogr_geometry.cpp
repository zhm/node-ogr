
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
  //NODE_SET_PROTOTYPE_METHOD(constructor, "centroid", centroid);
  NODE_SET_PROTOTYPE_METHOD(constructor, "simplify", simplify);
  NODE_SET_PROTOTYPE_METHOD(constructor, "simplifyPreserveTopology", simplifyPreserveTopology);
  NODE_SET_PROTOTYPE_METHOD(constructor, "polygonize", polygonize);
  NODE_SET_PROTOTYPE_METHOD(constructor, "swapXY", swapXY);

  target->Set(String::NewSymbol("Geometry"), constructor->GetFunction());
}

Geometry::Geometry(OGRGeometry *layer)
: ObjectWrap(),
  this_(layer)
{}

Geometry::Geometry()
: ObjectWrap(),
  this_(0)
{
}

Geometry::~Geometry()
{
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
  return ClosedPtr<Geometry, OGRGeometry>::Closed(geom);
}

Handle<Value> Geometry::toString(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(String::New(geom->this_->exportToJson()));
}


Handle<Value> Geometry::getDimension(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Integer::New(geom->this_->getDimension()));
}


Handle<Value> Geometry::getCoordinateDimension(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Integer::New(geom->this_->getCoordinateDimension()));
}


NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, isEmpty, Boolean, IsEmpty);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, isValid, Boolean, IsValid);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, isSimple, Boolean, IsSimple);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, isRing, Boolean, IsRing);
NODE_WRAPPED_METHOD_WITH_RESULT(Geometry, clone, Geometry, clone);



Handle<Value> Geometry::empty(const Arguments& args)
{
  HandleScope scope;

  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());

  geom->this_->empty();

  return Undefined();
}


Handle<Value> Geometry::wkbSize(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Integer::New(geom->this_->WkbSize()));
}


Handle<Value> Geometry::getGeometryType(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Integer::New(geom->this_->getGeometryType()));
}


Handle<Value> Geometry::getGeometryName(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(String::New(geom->this_->getGeometryName()));
}


Handle<Value> Geometry::exportToKML(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(String::New(geom->this_->exportToKML()));
}


Handle<Value> Geometry::exportToGML(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(String::New(geom->this_->exportToGML()));
}


Handle<Value> Geometry::exportToJSON(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(String::New(geom->this_->exportToJson()));
}


Handle<Value> Geometry::closeRings(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  geom->this_->closeRings();
  return Undefined();
}


Handle<Value> Geometry::segmentize(const Arguments& args)
{
  HandleScope scope;

  double max_length;

  NODE_ARG_DOUBLE(0, "segment length", max_length);

  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());

  geom->this_->segmentize(max_length);

  return Undefined();
}

Handle<Value> Geometry::intersects(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to compare", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Boolean::New(geom->this_->Intersects(other->this_)));
}

Handle<Value> Geometry::equals(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to compare", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Boolean::New(geom->this_->Equals(other->this_)));
}

Handle<Value> Geometry::disjoint(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to compare", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Boolean::New(geom->this_->Disjoint(other->this_)));
}

Handle<Value> Geometry::touches(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to compare", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Boolean::New(geom->this_->Touches(other->this_)));
}

Handle<Value> Geometry::crosses(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to compare", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Boolean::New(geom->this_->Crosses(other->this_)));
}

Handle<Value> Geometry::within(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to compare", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Boolean::New(geom->this_->Within(other->this_)));
}

Handle<Value> Geometry::contains(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to compare", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Boolean::New(geom->this_->Contains(other->this_)));
}

Handle<Value> Geometry::overlaps(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to compare", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Boolean::New(geom->this_->Overlaps(other->this_)));
}

Handle<Value> Geometry::boundary(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Geometry::New(geom->this_->Boundary()));
}

Handle<Value> Geometry::distance(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to use for distance calculation", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Number::New(geom->this_->Distance(other->this_)));
}

Handle<Value> Geometry::convexHull(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Geometry::New(geom->this_->ConvexHull()));
}

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

Handle<Value> Geometry::intersection(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to use for intersection", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Geometry::New(geom->this_->Intersection(other->this_)));
}

Handle<Value> Geometry::unionGeometry(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to use for union", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Geometry::New(geom->this_->Union(other->this_)));
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

Handle<Value> Geometry::difference(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to use for difference", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Geometry::New(geom->this_->Difference(other->this_)));
}

Handle<Value> Geometry::symDifference(const Arguments& args)
{
  HandleScope scope;
  Geometry *other;
  NODE_ARG_WRAPPED(0, "geometry to use for symmetric difference", Geometry, other);
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Geometry::New(geom->this_->SymDifference(other->this_)));
}

/*Handle<Value> Geometry::centroid(const Arguments& args)*/
//{
  //HandleScope scope;
  //NODE_ARG_WRAPPED(0, "geometry to use for symmetric difference", Geometry, other);
  //Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  //return scope.Close(Geometry::New(geom->this_->Centroid(centroid)));
/*}*/

Handle<Value> Geometry::simplify(const Arguments& args)
{
  HandleScope scope;
  double tolerance;

  NODE_ARG_DOUBLE(0, "tolerance", tolerance);

  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());

  return scope.Close(Geometry::New(geom->this_->Simplify(tolerance)));
}

Handle<Value> Geometry::simplifyPreserveTopology(const Arguments& args)
{
  HandleScope scope;
  double tolerance;

  NODE_ARG_DOUBLE(0, "tolerance", tolerance);

  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());

  return scope.Close(Geometry::New(geom->this_->SimplifyPreserveTopology(tolerance)));
}

Handle<Value> Geometry::polygonize(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  return scope.Close(Geometry::New(geom->this_->Polygonize()));
}

Handle<Value> Geometry::swapXY(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());
  geom->this_->swapXY();
  return Undefined();
}
