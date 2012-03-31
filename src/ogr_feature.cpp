
#include "ogr_common.hpp"
#include "ogr_feature.hpp"
#include "ogr_feature_defn.hpp"
#include "ogr_geometry.hpp"
#include "ogr_field_defn.hpp"

// boost
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>

Persistent<FunctionTemplate> Feature::constructor;

void Feature::Initialize(Handle<Object> target) {
  HandleScope scope;

  constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Feature::New));
  constructor->InstanceTemplate()->SetInternalFieldCount(1);
  constructor->SetClassName(String::NewSymbol("Feature"));

  NODE_SET_PROTOTYPE_METHOD(constructor, "toString", toString);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getGeometry", getGeometry);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setGeometryDirectly", setGeometryDirectly);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setGeometry", setGeometry);
  NODE_SET_PROTOTYPE_METHOD(constructor, "stealGeometry", stealGeometry);
  NODE_SET_PROTOTYPE_METHOD(constructor, "clone", clone);
  NODE_SET_PROTOTYPE_METHOD(constructor, "equal", equal);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldCount", getFieldCount);
  //NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldDefn", getFieldDefn);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldIndex", getFieldIndex);
  NODE_SET_PROTOTYPE_METHOD(constructor, "isFieldSet", isFieldSet);
  NODE_SET_PROTOTYPE_METHOD(constructor, "unsetField", unsetField);
  //NODE_SET_PROTOTYPE_METHOD(constructor, "getRawField", getRawField);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsInteger", getFieldAsInteger);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsDouble", getFieldAsDouble);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsString", getFieldAsString);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setField", setField);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFID", getFID);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setFID", setFID);
  //NODE_SET_PROTOTYPE_METHOD(constructor, "setFrom", setFrom);

  target->Set(String::NewSymbol("Feature"), constructor->GetFunction());
}

Feature::Feature(OGRFeature *layer)
: ObjectWrap(),
  this_(layer),
  owned_(true)
{}

Feature::Feature()
: ObjectWrap(),
  this_(0),
  owned_(true)
{
}

Feature::~Feature()
{
  if (owned_) {
    delete this_;
  }
}

Handle<Value> Feature::New(const Arguments& args)
{
  HandleScope scope;

  if (!args.IsConstructCall())
      return ThrowException(String::New("Cannot call constructor as function, you need to use 'new' keyword"));

  if (args[0]->IsExternal()) {
      Local<External> ext = Local<External>::Cast(args[0]);
      void* ptr = ext->Value();
      Feature *f = static_cast<Feature *>(ptr);
      f->Wrap(args.This());
      return args.This();
  }

  return args.This();
}

Handle<Value> Feature::New(OGRFeature *feature) {
  v8::HandleScope scope;
  Feature *wrapped = new Feature(feature);
  //wrapped->size_ = geom->WkbSize();
  //V8::AdjustAmountOfExternalAllocatedMemory(wrapped->size_);

  v8::Handle<v8::Value> ext = v8::External::New(wrapped);
  v8::Handle<v8::Object> obj = Feature::constructor->GetFunction()->NewInstance(1, &ext);

  return scope.Close(obj);
}

Handle<Value> Feature::New(OGRFeature *feature, bool owned) {
  v8::HandleScope scope;
  Feature *wrapped = new Feature(feature);
  wrapped->owned_ = owned;
  v8::Handle<v8::Value> ext = v8::External::New(wrapped);
  v8::Handle<v8::Object> obj = Feature::constructor->GetFunction()->NewInstance(1, &ext);
  return scope.Close(obj);
}

Handle<Value> Feature::toString(const Arguments& args)
{
  return HandleScope().Close(String::New("Feature"));
}


Handle<Value> Feature::getGeometry(const Arguments& args)
{
  return HandleScope().Close(Geometry::New(ObjectWrap::Unwrap<Feature>(args.This())->this_->GetGeometryRef(), false));
}

Handle<Value> Feature::getDefn(const Arguments& args)
{
  return HandleScope().Close(FeatureDefn::New(ObjectWrap::Unwrap<Feature>(args.This())->this_->GetDefnRef(), false));
}

Handle<Value> Feature::getFieldDefn(const Arguments& args)
{
  int field_index;
  NODE_ARG_INT(0, "field index", field_index);
  return HandleScope().Close(FieldDefn::New(ObjectWrap::Unwrap<Feature>(args.This())->this_->GetFieldDefnRef(field_index), false));
}

NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Feature, setGeometry, Integer, SetGeometry, Geometry, "geometry");
//NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Feature, setGeometryDirectly, Integer, SetGeometryDirectly, Geometry, "geometry");
//NODE_WRAPPED_METHOD_WITH_RESULT(Feature, getGeometry, Geometry, GetGeometryRef);
NODE_WRAPPED_METHOD_WITH_RESULT(Feature, stealGeometry, Geometry, StealGeometry);
NODE_WRAPPED_METHOD_WITH_RESULT(Feature, clone, Feature, Clone);
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Feature, equal, Boolean, Equal, Feature, "feature");
NODE_WRAPPED_METHOD_WITH_RESULT(Feature, getFieldCount, Integer, GetFieldCount);
NODE_WRAPPED_METHOD_WITH_RESULT_1_STRING_PARAM(Feature, getFieldIndex, Integer, GetFieldIndex, "field name");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Feature, isFieldSet, Boolean, IsFieldSet, "field index");
NODE_WRAPPED_METHOD_WITH_1_INTEGER_PARAM(Feature, unsetField, UnsetField, "field index");
//NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Feature, getRawField, Field, GetRawFieldRef, "field index");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Feature, getFieldAsInteger, Integer, GetFieldAsInteger, "field index");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Feature, getFieldAsDouble, Number, GetFieldAsDouble, "field index");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Feature, getFieldAsString, String, GetFieldAsString, "field index");
NODE_WRAPPED_METHOD_WITH_RESULT(Feature, getFID, Integer, GetFID);
NODE_WRAPPED_METHOD_WITH_1_INTEGER_PARAM(Feature, setFID, SetFID, "feature identifier");


Handle<Value> Feature::setField(const Arguments& args)
{
  HandleScope scope;
  int field_index;

  NODE_ARG_INT(0, "field index", field_index);

  if (args.Length() < 2) {
    return NODE_THROW("A value must be specified");
  }

  Feature *feature = ObjectWrap::Unwrap<Feature>(args.This());

  if (args[1]->IsInt32()) {
    feature->this_->SetField(field_index, args[1]->Int32Value());
  }
  else if (args[1]->IsNumber()) {
    feature->this_->SetField(field_index, args[1]->NumberValue());
  }
  else if (args[1]->IsString()) {
    feature->this_->SetField(field_index, TOSTR(args[1]));
  }
  //else if (!args[1]->IsNull() && !args[1]->IsUndefined() && args[1]->IsObject() && Field::constructor->HasInstance(args[1])) {
    //feature->this_->SetField(field_index, ObjectWrap<Field>::Unwrap(args[1])->get());
  //}
  else {
    return NODE_THROW("Invalid value specified to setField");
  }

  return Undefined();
}



Handle<Value> Feature::setGeometryDirectly(const Arguments& args)
{
  HandleScope scope;
  Geometry *geom;
  NODE_ARG_WRAPPED(0, "geometry", Geometry, geom);

  Feature *feature = ObjectWrap::Unwrap<Feature>(args.This());

  OGRErr err = feature->this_->SetGeometryDirectly(geom->get());

  if (err == OGRERR_NONE) {
    geom->owned_ = false;
  }

  return scope.Close(Integer::New(err));
}
