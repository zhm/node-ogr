#ifndef __NODE_OGR_GLOBAL_H__
#define __NODE_OGR_GLOBAL_H__

// v8
#include <v8.h>

// node
#include <node.h>

// ogr
#include "ogr_common.hpp"
#include "ogr_driver.hpp"
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

  static Handle<Value> getDriverByName(const Arguments &args) {
    HandleScope scope;

    std::string driver_name;

    NODE_ARG_STR(0, "driver name", driver_name);

    OGRSFDriver *driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(driver_name.c_str());

    if (driver == NULL) {
      return ThrowException(Exception::Error(String::New("Error retrieving driver")));
    }

    return scope.Close(Driver::New(driver));
  }

  static Handle<Value> getDriverCount(const Arguments &args) {
    HandleScope scope;

    return scope.Close(Integer::New(OGRSFDriverRegistrar::GetRegistrar()->GetDriverCount()));
  }

  static Handle<Value> getDriver(const Arguments &args) {
    HandleScope scope;

    int driver_index;

    NODE_ARG_INT(0, "driver index", driver_index);

    return scope.Close(Driver::New(OGRSFDriverRegistrar::GetRegistrar()->GetDriver(driver_index)));
  }

  static Handle<Value> getOpenDSCount(const Arguments &args) {
    HandleScope scope;

    return scope.Close(Integer::New(OGRSFDriverRegistrar::GetRegistrar()->GetOpenDSCount()));
  }

  static Handle<Value> getOpenDS(const Arguments &args) {
    HandleScope scope;

    int ds_index;

    NODE_ARG_INT(0, "data source index", ds_index);

    return scope.Close(Datasource::New(OGRSFDriverRegistrar::GetRegistrar()->GetOpenDS(ds_index)));
  }
}

#endif
