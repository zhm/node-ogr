#ifndef __NODE_OGR_LAYER_H__
#define __NODE_OGR_LAYER_H__

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>

// ogr
#include <ogrsf_frmts.h>

// boost
#include <boost/shared_ptr.hpp>

using namespace v8;
using namespace node;

class Layer: public node::ObjectWrap {
  public:
    static Persistent<FunctionTemplate> constructor;
    static void Initialize(Handle<Object> target);
    static Handle<Value> New(const Arguments &args);
    static Handle<Value> New(OGRLayer *layer);
    static Handle<Value> toString(const Arguments &args);
    static Handle<Value> resetReading(const Arguments &args);
    static Handle<Value> getNextFeature(const Arguments &args);
    static Handle<Value> getLayerDefn(const Arguments &args);
    static Handle<Value> getFeature(const Arguments &args);
    static Handle<Value> setFeature(const Arguments &args);
    static Handle<Value> getFeatureCount(const Arguments &args);
    static Handle<Value> createFeature(const Arguments &args);
    static Handle<Value> deleteFeature(const Arguments &args);
    static Handle<Value> getGeomType(const Arguments &args);
    static Handle<Value> getName(const Arguments &args);
    static Handle<Value> testCapability(const Arguments &args);
    static Handle<Value> syncToDisk(const Arguments &args);
    static Handle<Value> getFIDColumn(const Arguments &args);
    static Handle<Value> getGeometryColumn(const Arguments &args);

    Layer();
    Layer(OGRLayer *ds);
    inline OGRLayer *get() { return this_; }

  private:
    ~Layer();
    OGRLayer *this_;
};

#endif

