
#include "ogr_layer.hpp"
#include "ogr_datasource.hpp"
#include "ogr_common.hpp"

// boost
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>

Persistent<FunctionTemplate> Datasource::constructor;

void Datasource::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Datasource::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("Datasource"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "toString", toString);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getName", getName);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getLayerByName", getLayerByName);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getLayerCount", getLayerCount);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getLayer", getLayer);
    NODE_SET_PROTOTYPE_METHOD(constructor, "deleteLayer", deleteLayer);
    NODE_SET_PROTOTYPE_METHOD(constructor, "testCapability", testCapability);
    NODE_SET_PROTOTYPE_METHOD(constructor, "executeSQL", executeSQL);
    NODE_SET_PROTOTYPE_METHOD(constructor, "syncToDisk", syncToDisk);

    target->Set(String::NewSymbol("Datasource"), constructor->GetFunction());
}

Datasource::Datasource(OGRDataSource *ds) 
: ObjectWrap(),
  this_(ds) 
{}

Datasource::Datasource()
: ObjectWrap(),
  this_(0)
{
}

Datasource::~Datasource()
{
}

Handle<Value> Datasource::New(const Arguments& args)
{
  HandleScope scope;

  if (!args.IsConstructCall())
      return ThrowException(String::New("Cannot call constructor as function, you need to use 'new' keyword"));

  if (args[0]->IsExternal()) {
      Local<External> ext = Local<External>::Cast(args[0]);
      void* ptr = ext->Value();
      Datasource *f =  static_cast<Datasource *>(ptr);
      f->Wrap(args.This());
      return args.This();
  }

  //Feature* f = new Feature(args[0]->IntegerValue());
  //f->Wrap(args.This());
  return args.This();
}

Handle<Value> Datasource::New(OGRDataSource *ds) {
  return ClosedPtr<Datasource, OGRDataSource>::Closed(ds);
}

Handle<Value> Datasource::toString(const Arguments& args)
{
  HandleScope scope;
  return scope.Close(String::New("Datasource"));
}

Handle<Value> Datasource::getName(const Arguments& args)
{
  HandleScope scope;
  Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());
  return scope.Close(String::New(ds->this_->GetName()));
}

Handle<Value> Datasource::getLayerByName(const Arguments& args)
{
  HandleScope scope;
  std::string layer_name;

  NODE_ARG_STR(0, "layer name", layer_name);

  try {
    Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());

    OGRLayer *layer = ds->this_->GetLayerByName(layer_name.c_str());

    if (layer)
      return scope.Close(Layer::New(layer));

  } catch (...) {
    return NODE_THROW("Unknown error. Does the layer exist?");
  }

  return Undefined();
}

Handle<Value> Datasource::getLayerCount(const Arguments& args)
{
  HandleScope scope;

  Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());

  return scope.Close(Integer::New(ds->this_->GetLayerCount()));
}

Handle<Value> Datasource::getLayer(const Arguments& args)
{
  HandleScope scope;
  int layer_index = 0;

  NODE_ARG_INT_OPT(0, "layer index", layer_index);

  Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());

  if (layer_index < 0 || layer_index >= ds->this_->GetLayerCount()) {
    return NODE_THROW("Invalid layer index.");
  }

  try {
    OGRLayer *layer = ds->this_->GetLayer(layer_index);

    if (layer)
      return scope.Close(Layer::New(layer));

  } catch (...) {
    return NODE_THROW("Unknown error. Does the layer exist?");
  }

  return Undefined();
}


Handle<Value> Datasource::deleteLayer(const Arguments& args)
{
  HandleScope scope;
  int layer_index = 0;

  NODE_ARG_INT_OPT(0, "layer index", layer_index);

  Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());

  OGRErr err = ds->this_->DeleteLayer(layer_index);

  return scope.Close(Integer::New(err));
}


Handle<Value> Datasource::testCapability(const Arguments& args)
{
  HandleScope scope;
  std::string capability;

  NODE_ARG_STR(0, "capability", capability);

  Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());

  int result = ds->this_->TestCapability(capability.c_str());

  return scope.Close(Boolean::New(!!result));
}



Handle<Value> Datasource::executeSQL(const Arguments& args)
{
  HandleScope scope;
  std::string sql;

  NODE_ARG_STR(0, "sql text", sql);

  Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());

  try {
    OGRLayer *layer = ds->this_->ExecuteSQL(sql.c_str(), NULL, NULL);

    if (layer)
      return scope.Close(Layer::New(layer));

  } catch (...) {
    return NODE_THROW("Unknown error. Does the layer exist?");
  }

  return Undefined();
}


Handle<Value> Datasource::syncToDisk(const Arguments& args)
{
  HandleScope scope;

  Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());

  return scope.Close(Integer::New(ds->this_->SyncToDisk()));
}


