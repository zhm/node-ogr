
#include "ogr_common.hpp"
#include "ogr_layer.hpp"
#include "ogr_feature.hpp"
#include "ogr_feature_defn.hpp"

Persistent<FunctionTemplate> Layer::constructor;

void Layer::Initialize(Handle<Object> target) {
  HandleScope scope;

  constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Layer::New));
  constructor->InstanceTemplate()->SetInternalFieldCount(1);
  constructor->SetClassName(String::NewSymbol("Layer"));

  NODE_SET_PROTOTYPE_METHOD(constructor, "toString", toString);
  NODE_SET_PROTOTYPE_METHOD(constructor, "resetReading", resetReading);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getNextFeature", getNextFeature);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getLayerDefn", getLayerDefn);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFeature", getFeature);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFeatureCount", getFeatureCount);
  NODE_SET_PROTOTYPE_METHOD(constructor, "setFeature", setFeature);
  NODE_SET_PROTOTYPE_METHOD(constructor, "createFeature", createFeature);
  NODE_SET_PROTOTYPE_METHOD(constructor, "deleteFeature", deleteFeature);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getGeomType", getGeomType);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getName", getName);
  NODE_SET_PROTOTYPE_METHOD(constructor, "testCapability", testCapability);
  NODE_SET_PROTOTYPE_METHOD(constructor, "syncToDisk", syncToDisk);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getFIDColumn", getFIDColumn);
  NODE_SET_PROTOTYPE_METHOD(constructor, "getGeometryColumn", getGeometryColumn);
  target->Set(String::NewSymbol("Layer"), constructor->GetFunction());
}

Layer::Layer(OGRLayer *layer)
: ObjectWrap(),
  this_(layer)
{}

Layer::Layer()
: ObjectWrap(),
  this_(0)
{
}

Layer::~Layer()
{
}

Handle<Value> Layer::New(const Arguments& args)
{
  HandleScope scope;

  if (!args.IsConstructCall())
    return ThrowException(String::New("Cannot call constructor as function, you need to use 'new' keyword"));

  if (args[0]->IsExternal()) {
    Local<External> ext = Local<External>::Cast(args[0]);
    void* ptr = ext->Value();
    Layer *f = static_cast<Layer *>(ptr);
    f->Wrap(args.This());
    return args.This();
  }

  return args.This();
}

Handle<Value> Layer::New(OGRLayer *layer) {
  return ClosedPtr<Layer, OGRLayer>::Closed(layer);
}

Handle<Value> Layer::toString(const Arguments& args)
{
  HandleScope scope;

  Layer *layer = ObjectWrap::Unwrap<Layer>(args.This());
  return scope.Close(String::New(layer->this_->GetName()));
}


NODE_WRAPPED_METHOD(Layer, syncToDisk, SyncToDisk);
NODE_WRAPPED_METHOD_WITH_RESULT(Layer, getGeomType, Integer, GetGeomType);
NODE_WRAPPED_METHOD_WITH_RESULT(Layer, getName, String, GetName);
NODE_WRAPPED_METHOD_WITH_RESULT(Layer, getFIDColumn, String, GetFIDColumn);
NODE_WRAPPED_METHOD_WITH_RESULT(Layer, getGeometryColumn, String, GetGeometryColumn);
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Layer, setFeature, Integer, SetFeature, Feature, "feature");
NODE_WRAPPED_METHOD_WITH_RESULT_1_WRAPPED_PARAM(Layer, createFeature, Integer, SetFeature, Feature, "feature");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Layer, deleteFeature, Integer, DeleteFeature, "feature");
NODE_WRAPPED_METHOD_WITH_RESULT_1_STRING_PARAM(Layer, testCapability, Boolean, TestCapability, "capability");

Handle<Value> Layer::getLayerDefn(const Arguments& args)
{
  return HandleScope().Close(FeatureDefn::New(ObjectWrap::Unwrap<Layer>(args.This())->this_->GetLayerDefn(), false));
}

Handle<Value> Layer::resetReading(const Arguments& args)
{
  HandleScope scope;

  Layer *layer = ObjectWrap::Unwrap<Layer>(args.This());

  layer->this_->ResetReading();

  return Undefined();
}

Handle<Value> Layer::getNextFeature(const Arguments& args)
{
  HandleScope scope;
  Layer *layer = ObjectWrap::Unwrap<Layer>(args.This());

  OGRFeature *next = layer->this_->GetNextFeature();

  if (next) {
    return scope.Close(Feature::New(next));
  }

  return Undefined();
}

Handle<Value> Layer::getFeature(const Arguments& args)
{
  HandleScope scope;
  int feature_id;

  NODE_ARG_INT(0, "feature id", feature_id);

  Layer *layer = ObjectWrap::Unwrap<Layer>(args.This());

  OGRFeature *feature = layer->this_->GetFeature(feature_id);

  if (feature) {
    return scope.Close(Feature::New(feature));
  }

  return Undefined();
}


Handle<Value> Layer::getFeatureCount(const Arguments& args)
{
  HandleScope scope;
  bool force = true;

  NODE_ARG_BOOL_OPT(0, "force", force);

  Layer *layer = ObjectWrap::Unwrap<Layer>(args.This());

  int count = layer->this_->GetFeatureCount(force);

  return scope.Close(Integer::New(count));
}
