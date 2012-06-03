
#include "ogr_common.hpp"
#include "ogr_feature.hpp"
#include "ogr_feature_defn.hpp"
#include "ogr_geometry.hpp"
#include "ogr_field_defn.hpp"

// node
#include <node/node_buffer.h>

Persistent<FunctionTemplate> Feature::constructor;

void Feature::Initialize(Handle<Object> target) {
  HandleScope scope;

  constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Feature::New));
  constructor->InstanceTemplate()->SetInternalFieldCount(1);
  constructor->SetClassName(String::NewSymbol("Feature"));

  NODE_SET_PROTOTYPE_METHOD(constructor, "toString", toString);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getDefn", getDefn);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getGeometry", getGeometry);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setGeometryDirectly", setGeometryDirectly);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setGeometry", setGeometry);
  NODE_SET_PROTOTYPE_METHOD(constructor, "stealGeometry", stealGeometry);
  NODE_SET_PROTOTYPE_METHOD(constructor, "clone", clone);
  NODE_SET_PROTOTYPE_METHOD(constructor, "equal", equal);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldCount", getFieldCount);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldDefn", getFieldDefn);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldIndex", getFieldIndex);
  NODE_SET_PROTOTYPE_METHOD(constructor, "isFieldSet", isFieldSet);
  NODE_SET_PROTOTYPE_METHOD(constructor, "unsetField", unsetField);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsInteger", getFieldAsInteger);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsDouble", getFieldAsDouble);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsString", getFieldAsString);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsIntegerList", getFieldAsIntegerList);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsDoubleList", getFieldAsDoubleList);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsStringList", getFieldAsStringList);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsBinary", getFieldAsBinary);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFieldAsDateTime", getFieldAsDateTime);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setField", setField);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFID", getFID);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setFID", setFID);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setFrom", setFrom);

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
    OGRFeature::DestroyFeature(this_);
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

NODE_WRAPPED_METHOD_WITH_RESULT(Feature, stealGeometry, Geometry, StealGeometry);
NODE_WRAPPED_METHOD_WITH_RESULT(Feature, clone, Feature, Clone);
NODE_WRAPPED_METHOD_WITH_RESULT(Feature, getFieldCount, Integer, GetFieldCount);
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Feature, setGeometry, Integer, SetGeometry, Geometry, "geometry");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Feature, equal, Boolean, Equal, Feature, "feature");
NODE_WRAPPED_METHOD_WITH_RESULT_1_STRING_PARAM(Feature, getFieldIndex, Integer, GetFieldIndex, "field name");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Feature, isFieldSet, Boolean, IsFieldSet, "field index");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Feature, getFieldAsInteger, Integer, GetFieldAsInteger, "field index");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Feature, getFieldAsDouble, Number, GetFieldAsDouble, "field index");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Feature, getFieldAsString, String, GetFieldAsString, "field index");
NODE_WRAPPED_METHOD_WITH_RESULT(Feature, getFID, Integer, GetFID);
NODE_WRAPPED_METHOD_WITH_1_INTEGER_PARAM(Feature, setFID, SetFID, "feature identifier");
NODE_WRAPPED_METHOD_WITH_1_INTEGER_PARAM(Feature, unsetField, UnsetField, "field index");


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

Handle<Value> Feature::setFrom(const Arguments& args)
{
  HandleScope scope;
  Feature *other_feature;
  bool forgiving = true;
  Handle<Array> index_map;

  NODE_ARG_WRAPPED(0, "feature", Feature, other_feature);

  if (args.Length() <= 2) {
    NODE_ARG_BOOL_OPT(1, "forgiving", forgiving);

    Feature *feature = ObjectWrap::Unwrap<Feature>(args.This());

    return scope.Close(Integer::New(feature->this_->SetFrom(other_feature->this_, forgiving ? TRUE : FALSE)));
  }
  else if (args.Length() > 2) {
    NODE_ARG_ARRAY(1, "index map", index_map);
    NODE_ARG_BOOL_OPT(2, "forgiving", forgiving);

    if (index_map->Length() < 1) {
      return NODE_THROW("index map must contain at least 1 index");
    }

    int *index_map_ptr = new int[index_map->Length()];

    for (unsigned index = 0; index < index_map->Length(); index++) {
      Handle<Value> field_index(index_map->Get(Integer::New(index)));

      if (!field_index->IsUint32()) {
        delete [] index_map_ptr;
        return NODE_THROW("index map must contain only integer values");
      }

      index_map_ptr[index] = (int)field_index->Uint32Value();
    }

    Feature *feature = ObjectWrap::Unwrap<Feature>(args.This());

    OGRErr err = feature->this_->SetFrom(other_feature->this_,
                                         index_map_ptr,
                                         forgiving ? TRUE : FALSE);

    delete [] index_map_ptr;

    return scope.Close(Integer::New(err));
  }

  return Undefined();
}


Handle<Value> Feature::getFieldAsIntegerList(const Arguments& args)
{
  HandleScope scope;
  int field_index;
  NODE_ARG_INT(0, "field index", field_index);

  int count_of_values = 0;

  Feature *feature = ObjectWrap::Unwrap<Feature>(args.This());

  const int *values = feature->this_->GetFieldAsIntegerList(field_index, &count_of_values);

  assert(count_of_values >= 0);

  Local<Array> return_array = Array::New(count_of_values);

  if (count_of_values > 0) {
    for (int index = 0; index < count_of_values; index++) {
      return_array->Set(index, Integer::New(values[index]));
    }
  }

  return scope.Close(return_array);
}


Handle<Value> Feature::getFieldAsDoubleList(const Arguments& args)
{
  HandleScope scope;
  int field_index;
  NODE_ARG_INT(0, "field index", field_index);

  int count_of_values = 0;

  Feature *feature = ObjectWrap::Unwrap<Feature>(args.This());

  const double *values = feature->this_->GetFieldAsDoubleList(field_index, &count_of_values);

  assert(count_of_values >= 0);

  Local<Array> return_array = Array::New(count_of_values);

  if (count_of_values > 0) {
    for (int index = 0; index < count_of_values; index++) {
      return_array->Set(index, Number::New(values[index]));
    }
  }

  return scope.Close(return_array);
}


Handle<Value> Feature::getFieldAsStringList(const Arguments& args)
{
  HandleScope scope;
  int field_index;
  NODE_ARG_INT(0, "field index", field_index);

  Feature *feature = ObjectWrap::Unwrap<Feature>(args.This());

  char **values = feature->this_->GetFieldAsStringList(field_index);

  int count_of_values = CSLCount(values);

  assert(count_of_values >= 0);

  Local<Array> return_array = Array::New(count_of_values);

  if (count_of_values > 0) {
    for (int index = 0; index < count_of_values; index++) {
      return_array->Set(index, String::New(values[index]));
    }
  }

  return scope.Close(return_array);
}


Handle<Value> Feature::getFieldAsBinary(const Arguments& args)
{
  HandleScope scope;
  int field_index;
  NODE_ARG_INT(0, "field index", field_index);

  Feature *feature = ObjectWrap::Unwrap<Feature>(args.This());

  int count_of_bytes = 0;

  GByte *values = feature->this_->GetFieldAsBinary(field_index, &count_of_bytes);

  assert(count_of_bytes >= 0);

  if (count_of_bytes > 0) {
    char *data = new char[count_of_bytes];
    memcpy(data, values, count_of_bytes);
    Local<Buffer> return_buffer = Buffer::New(data, count_of_bytes);
    return scope.Close(return_buffer->handle_);
  }

  return Undefined();
}


Handle<Value> Feature::getFieldAsDateTime(const Arguments& args)
{
  HandleScope scope;
  int field_index;
  NODE_ARG_INT(0, "field index", field_index);

  Feature *feature = ObjectWrap::Unwrap<Feature>(args.This());

  int year, month, day, hour, minute, second, timezone;

  year = month = day = hour = minute = second = timezone = NULL;

  int result = feature->this_->GetFieldAsDateTime(field_index, &year, &month,
      &day, &hour, &minute, &second, &timezone);

  if (result == TRUE) {
    Local<Object> hash = Object::New();

    if (year)
      hash->Set(String::New("year"), Integer::New(year));
    if (month)
      hash->Set(String::New("month"), Integer::New(month));
    if (day)
      hash->Set(String::New("day"), Integer::New(day));
    if (hour)
      hash->Set(String::New("hour"), Integer::New(hour));
    if (minute)
      hash->Set(String::New("minute"), Integer::New(minute));
    if (second)
      hash->Set(String::New("second"), Integer::New(second));
    if (timezone)
      hash->Set(String::New("timezone"), Integer::New(timezone));

    return scope.Close(hash);
  } else {
    return Undefined();
  }
}
