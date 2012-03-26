#ifndef __NODE_OGR_GLOBAL_H__
#define __NODE_OGR_GLOBAL_H__

// v8
#include <v8.h>

// node
#include <node.h>

// ogr
#include "ogr_common.hpp"
#include "ogr_datasource.hpp"

using namespace v8;
using namespace node;

namespace node_ogr {
  static Handle<Value> open(const Arguments &args) {
    HandleScope scope;

    std::string ds_name;
    bool is_update = false;

    NODE_ARG_STR(0, "datasource", ds_name);
    NODE_ARG_BOOL_OPT(1, "update", is_update);

    OGRDataSource *ds = NULL;

    ds = OGRSFDriverRegistrar::Open(ds_name.c_str(), is_update);

    if (ds == NULL) {
      return ThrowException(Exception::Error(String::New("Error creating datasource")));
    }

    return scope.Close(Datasource::New(ds));
  }
}

#endif

