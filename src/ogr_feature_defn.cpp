
#include "ogr_common.hpp"
#include "ogr_feature_defn.hpp"
#include "ogr_field_defn.hpp"

// boost
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>

Persistent<FunctionTemplate> FeatureDefn::constructor;

void FeatureDefn::Initialize(Handle<Object> target) {
  HandleScope scope;

  constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(FeatureDefn::New));
  constructor->InstanceTemplate()->SetInternalFieldCount(1);
  constructor->SetClassName(String::NewSymbol("FeatureDefn"));

  NODE_SET_PROTOTYPE_METHOD(constructor, "toString", toString);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getName", getName);
  NODE_SET_PROTOTYPE_METHOD(constructor, "toString", toString);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldCount", getFieldCount);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldDefn", getFieldDefn);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldIndex", getFieldIndex);
  NODE_SET_PROTOTYPE_METHOD(constructor, "deleteFieldDefn", deleteFieldDefn);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getGeomType", getGeomType);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setGeomType", setGeomType);
  NODE_SET_PROTOTYPE_METHOD(constructor, "clone", clone);
  NODE_SET_PROTOTYPE_METHOD(constructor, "isGeometryIgnored", isGeometryIgnored);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setGeometryIgnored", setGeometryIgnored);
  NODE_SET_PROTOTYPE_METHOD(constructor, "isStyleIgnored", isStyleIgnored);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setStyleIgnored", setStyleIgnored);
  NODE_SET_PROTOTYPE_METHOD(constructor, "toString", toString);

  target->Set(String::NewSymbol("FeatureDefn"), constructor->GetFunction());
}

FeatureDefn::FeatureDefn(OGRFeatureDefn *layer)
: ObjectWrap(),
  this_(layer),
  owned_(true)
{}

FeatureDefn::FeatureDefn()
: ObjectWrap(),
  this_(0),
  owned_(true)
{
}

FeatureDefn::~FeatureDefn()
{
  if (owned_) {
    this_->Release();
  }
}

Handle<Value> FeatureDefn::New(const Arguments& args)
{
  HandleScope scope;

  if (!args.IsConstructCall())
      return ThrowException(String::New("Cannot call constructor as function, you need to use 'new' keyword"));

  if (args[0]->IsExternal()) {
      Local<External> ext = Local<External>::Cast(args[0]);
      void* ptr = ext->Value();
      FeatureDefn *f = static_cast<FeatureDefn *>(ptr);
      f->Wrap(args.This());
      return args.This();
  }

  return args.This();
}

Handle<Value> FeatureDefn::New(OGRFeatureDefn *def) {
  v8::HandleScope scope;
  FeatureDefn *wrapped = new FeatureDefn(def);
  //wrapped->size_ = geom->WkbSize();
  //V8::AdjustAmountOfExternalAllocatedMemory(wrapped->size_);

  v8::Handle<v8::Value> ext = v8::External::New(wrapped);
  v8::Handle<v8::Object> obj = FeatureDefn::constructor->GetFunction()->NewInstance(1, &ext);

  return scope.Close(obj);
}

Handle<Value> FeatureDefn::New(OGRFeatureDefn *def, bool owned) {
  v8::HandleScope scope;
  FeatureDefn *wrapped = new FeatureDefn(def);
  wrapped->owned_ = owned;
  v8::Handle<v8::Value> ext = v8::External::New(wrapped);
  v8::Handle<v8::Object> obj = FeatureDefn::constructor->GetFunction()->NewInstance(1, &ext);
  return scope.Close(obj);
}

Handle<Value> FeatureDefn::toString(const Arguments& args)
{
  HandleScope scope;
  return scope.Close(String::New("FeatureDefn"));
}


NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, getName, String, GetName);
NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, getFieldCount, Integer, GetFieldCount);
NODE_WRAPPED_METHOD_WITH_RESULT_1_STRING_PARAM(FeatureDefn, getFieldIndex, Integer, GetFieldIndex, "field name");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(FeatureDefn, deleteFieldDefn, Integer, DeleteFieldDefn, "field index");
NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, getGeomType, Integer, GetGeomType);
NODE_WRAPPED_METHOD_WITH_1_ENUM_PARAM(FeatureDefn, setGeomType, SetGeomType, OGRwkbGeometryType, "geometry type");
NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, clone, FeatureDefn, Clone);
NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, isGeometryIgnored, Boolean, IsGeometryIgnored);
NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, isStyleIgnored, Boolean, IsStyleIgnored);
NODE_WRAPPED_METHOD_WITH_1_BOOLEAN_PARAM(FeatureDefn, setGeometryIgnored, SetGeometryIgnored, "ignore");
NODE_WRAPPED_METHOD_WITH_1_BOOLEAN_PARAM(FeatureDefn, setStyleIgnored, SetStyleIgnored, "ignore");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(FeatureDefn, getFieldDefn, FieldDefn, GetFieldDefn, "feature index");

/*NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(FeatureDefn, setGeometry, Integer, SetGeometry, Geometry, "geometry");*/
//NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(FeatureDefn, setGeometryDirectly, Integer, SetGeometryDirectly, Geometry, "geometry");
//NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, getGeometry, Geometry, GetGeometryRef);
//NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, stealGeometry, Geometry, StealGeometry);
//NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, clone, FeatureDefn, Clone);
//NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(FeatureDefn, equal, Boolean, Equal, FeatureDefn, "feature");
//NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, getFieldCount, Integer, GetFieldCount);
//NODE_WRAPPED_METHOD_WITH_RESULT_1_STRING_PARAM(FeatureDefn, getFieldIndex, Integer, GetFieldIndex, "field name");
//NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(FeatureDefn, isFieldSet, Boolean, IsFieldSet, "field index");
//NODE_WRAPPED_METHOD_WITH_1_INTEGER_PARAM(FeatureDefn, unsetField, UnsetField, "field index");
//NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(FeatureDefn, getFieldAsInteger, Integer, GetFieldAsInteger, "field index");
//NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(FeatureDefn, getFieldAsDouble, Number, GetFieldAsDouble, "field index");
//NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(FeatureDefn, getFieldAsString, String, GetFieldAsString, "field index");
//NODE_WRAPPED_METHOD_WITH_RESULT(FeatureDefn, getFID, Integer, GetFID);
/*NODE_WRAPPED_METHOD_WITH_1_INTEGER_PARAM(FeatureDefn, setFID, SetFID, "feature identifier");*/

