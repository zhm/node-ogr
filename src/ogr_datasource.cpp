
#include "ogr_layer.hpp"
#include "ogr_datasource.hpp"
#include "ogr_geometry.hpp"
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
    NODE_SET_PROTOTYPE_METHOD(constructor, "createLayer", createLayer);
    NODE_SET_PROTOTYPE_METHOD(constructor, "copyLayer", copyLayer);
    NODE_SET_PROTOTYPE_METHOD(constructor, "releaseResultSet", releaseResultSet);

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
  //OGRDataSource::DestroyDataSource(this_);
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

NODE_WRAPPED_METHOD_WITH_RESULT(Datasource, getName, String, GetName);
NODE_WRAPPED_METHOD_WITH_RESULT(Datasource, getLayerCount, Integer, GetLayerCount);
NODE_WRAPPED_METHOD_WITH_RESULT(Datasource, syncToDisk, Integer, SyncToDisk);
NODE_WRAPPED_METHOD_WITH_RESULT_1_STRING_PARAM(Datasource, getLayerByName, Layer, GetLayerByName, "layer name");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Datasource, getLayer, Layer, GetLayer, "layer index to fetch");
NODE_WRAPPED_METHOD_WITH_RESULT_1_INTEGER_PARAM(Datasource, deleteLayer, Integer, DeleteLayer, "layer index to delete");
NODE_WRAPPED_METHOD_WITH_RESULT_1_STRING_PARAM(Datasource, testCapability, Boolean, TestCapability, "capability to test");
NODE_WRAPPED_METHOD_WITH_1_WRAPPED_PARAM(Datasource, releaseResultSet, ReleaseResultSet, Layer, "layer to release");

Handle<Value> Datasource::executeSQL(const Arguments& args)
{
  HandleScope scope;
  std::string sql;
  std::string sql_dialect;
  Geometry *spatial_filter = NULL;

  NODE_ARG_STR(0, "sql text", sql);
  NODE_ARG_WRAPPED_OPT(1, "spatial filter geometry", Geometry, spatial_filter);
  NODE_ARG_OPT_STR(2, "sql dialect", sql_dialect);

  Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());

  try {
    OGRLayer *layer = ds->this_->ExecuteSQL(sql.c_str(),
                                            spatial_filter ? spatial_filter->get() : NULL,
                                            sql_dialect.empty() ? NULL : sql_dialect.c_str());

    if (layer)
      return scope.Close(Layer::New(layer));

  } catch (...) {
    return NODE_THROW("Unknown error. Does the layer exist?");
  }

  return Undefined();
}


Handle<Value> Datasource::createLayer(const Arguments& args)
{
  HandleScope scope;
  std::string layer_name;
  std::string spatial_ref = "";
  OGRwkbGeometryType geom_type = wkbUnknown;
  Handle<Array> layer_options = Array::New(0);

  NODE_ARG_STR(0, "layer name", layer_name);
  NODE_ARG_OPT_STR(1, "spatial reference", spatial_ref);
  NODE_ARG_ENUM_OPT(2, "geometry type", OGRwkbGeometryType, geom_type);
  NODE_ARG_ARRAY_OPT(3, "layer creation options", layer_options);

  Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());

  char **options = NULL;

  if (layer_options->Length() > 0) {
    options = new char* [layer_options->Length()];
    for (unsigned int i = 0; i < layer_options->Length(); ++i) {
      options[i] = TOSTR(layer_options->Get(i));
    }
  }

  OGRLayer *layer = ds->this_->CreateLayer(layer_name.c_str(),
                                           NULL,
                                           geom_type,
                                           options);

  if (options) {
    delete [] options;
  }

  if (layer) {
    return scope.Close(Layer::New(layer));
  }

  return Undefined();
}



Handle<Value> Datasource::copyLayer(const Arguments& args)
{
  HandleScope scope;
  Layer *layer_to_copy;
  std::string new_name = "";
  Handle<Array> layer_options = Array::New(0);

  NODE_ARG_WRAPPED(0, "layer to copy", Layer, layer_to_copy);
  NODE_ARG_STR(1, "new layer name", new_name);
  NODE_ARG_ARRAY_OPT(2, "layer creation options", layer_options);

  Datasource *ds = ObjectWrap::Unwrap<Datasource>(args.This());

  char **options = NULL;

  if (layer_options->Length() > 0) {
    options = new char* [layer_options->Length()];
    for (unsigned int i = 0; i < layer_options->Length(); ++i) {
      options[i] = TOSTR(layer_options->Get(i));
    }
  }

  OGRLayer *layer = ds->this_->CopyLayer(layer_to_copy->get(),
                                         new_name.c_str(),
                                         options);

  if (options) {
    delete [] options;
  }

  if (layer) {
    return scope.Close(Layer::New(layer));
  }

  return Undefined();
}
