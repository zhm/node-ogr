// v8
#include <v8.h>

// node
#include <node.h>
#include <node_buffer.h>
#include <node_version.h>

// gdal
#include <ogr_api.h>

// node-gdal
#include "ogr_common.hpp"
#include "ogr.hpp"
#include "ogr_driver.hpp"
#include "ogr_datasource.hpp"
#include "ogr_layer.hpp"
#include "ogr_feature_defn.hpp"
#include "ogr_field_defn.hpp"
#include "ogr_feature.hpp"
#include "ogr_geometry.hpp"

// boost
#include <boost/version.hpp>
#include <boost/foreach.hpp>

// std
#include <string>
#include <sstream>
#include <vector>

namespace node_ogr {

using namespace node;
using namespace v8;

static std::string format_version(int version)
{
    std::ostringstream s;
    s << version/100000 << "." << version/100 % 1000  << "." << version % 100;
    return s.str();
}

extern "C" {
  enum {
    Success = 0,
    NotEnoughData = 1,
    NotEnoughMemory = 2,
    UnsupportedGeometryType = 3,
    UnsupportedOperation = 4,
    CorruptData = 5,
    Failure = 6,
    UnsupportedSRS = 7,
    InvalidHandle = 8
  } Error;


  static void CPL_STDCALL NodeOGRErrorHandler(CPLErr err, int code, const char *message)
  {
  }

  static Handle<Value> QuietOutput(const Arguments &args)
  {
    CPLSetErrorHandler(CPLQuietErrorHandler);
    return Undefined();
  }

  static Handle<Value> VerboseOutput(const Arguments &args)
  {
    CPLSetErrorHandler(NULL);
    return Undefined();
  }

  static void Init(Handle<Object> target)
  {
    NODE_SET_METHOD(target, "open", node_ogr::open);
    NODE_SET_METHOD(target, "getDriverByName", node_ogr::getDriverByName);
    NODE_SET_METHOD(target, "getDriverCount", node_ogr::getDriverCount);
    NODE_SET_METHOD(target, "getDriver", node_ogr::getDriver);
    NODE_SET_METHOD(target, "getOpenDSCount", node_ogr::getOpenDSCount);
    NODE_SET_METHOD(target, "getOpenDS", node_ogr::getOpenDS);

    Driver::Initialize(target);
    Datasource::Initialize(target);
    Layer::Initialize(target);
    Feature::Initialize(target);
    FeatureDefn::Initialize(target);
    FieldDefn::Initialize(target);
    Geometry::Initialize(target);

    //CPLSetErrorHandler(NodeOGRErrorHandler);

    Local<Object> versions = Object::New();
    versions->Set(String::NewSymbol("node"), String::New(NODE_VERSION+1));
    versions->Set(String::NewSymbol("v8"), String::New(V8::GetVersion()));
    versions->Set(String::NewSymbol("boost"), String::New(format_version(BOOST_VERSION).c_str()));
    versions->Set(String::NewSymbol("boost_number"), Integer::New(BOOST_VERSION));
    target->Set(String::NewSymbol("versions"), versions);

    NODE_SET_METHOD(target, "quiet", QuietOutput);
    NODE_SET_METHOD(target, "verbose", VerboseOutput);

    Local<Object> supports = Object::New();
    target->Set(String::NewSymbol("supports"), supports);

    OGRRegisterAll();

    OGRSFDriverRegistrar *reg = OGRSFDriverRegistrar::GetRegistrar();

    int driver_count = reg->GetDriverCount();

    Local<Array> supported_drivers = Array::New(driver_count);

    for (int i = 0; i < driver_count; ++i) {
      OGRSFDriver *driver = reg->GetDriver(i);
      supported_drivers->Set(Integer::New(static_cast<int>(i)), String::New(driver->GetName()));
    }

    target->Set(String::NewSymbol("drivers"), supported_drivers);


    NODE_DEFINE_CONSTANT(target, wkbUnknown);
    NODE_DEFINE_CONSTANT(target, wkbPoint);
    NODE_DEFINE_CONSTANT(target, wkbLineString);
    NODE_DEFINE_CONSTANT(target, wkbPolygon);
    NODE_DEFINE_CONSTANT(target, wkbMultiPoint);
    NODE_DEFINE_CONSTANT(target, wkbMultiLineString);
    NODE_DEFINE_CONSTANT(target, wkbMultiPolygon);
    NODE_DEFINE_CONSTANT(target, wkbGeometryCollection);
    NODE_DEFINE_CONSTANT(target, wkbNone);
    NODE_DEFINE_CONSTANT(target, wkbLinearRing);
    NODE_DEFINE_CONSTANT(target, wkbPoint25D);
    NODE_DEFINE_CONSTANT(target, wkbLineString25D);
    NODE_DEFINE_CONSTANT(target, wkbPolygon25D);
    NODE_DEFINE_CONSTANT(target, wkbMultiPoint25D);
    NODE_DEFINE_CONSTANT(target, wkbMultiLineString25D);
    NODE_DEFINE_CONSTANT(target, wkbMultiPolygon25D);
    NODE_DEFINE_CONSTANT(target, wkbGeometryCollection25D);

    NODE_DEFINE_CONSTANT(target, OFTInteger);
    NODE_DEFINE_CONSTANT(target, OFTIntegerList);
    NODE_DEFINE_CONSTANT(target, OFTReal);
    NODE_DEFINE_CONSTANT(target, OFTRealList);
    NODE_DEFINE_CONSTANT(target, OFTString);
    NODE_DEFINE_CONSTANT(target, OFTStringList);
    NODE_DEFINE_CONSTANT(target, OFTWideString);
    NODE_DEFINE_CONSTANT(target, OFTWideStringList);
    NODE_DEFINE_CONSTANT(target, OFTBinary);
    NODE_DEFINE_CONSTANT(target, OFTDate);
    NODE_DEFINE_CONSTANT(target, OFTTime);
    NODE_DEFINE_CONSTANT(target, OFTDateTime);

    NODE_DEFINE_CONSTANT(target, Success);
    NODE_DEFINE_CONSTANT(target, NotEnoughData);
    NODE_DEFINE_CONSTANT(target, NotEnoughMemory);
    NODE_DEFINE_CONSTANT(target, UnsupportedGeometryType);
    NODE_DEFINE_CONSTANT(target, UnsupportedOperation);
    NODE_DEFINE_CONSTANT(target, CorruptData);
    NODE_DEFINE_CONSTANT(target, Failure);
    NODE_DEFINE_CONSTANT(target, UnsupportedSRS);
    NODE_DEFINE_CONSTANT(target, InvalidHandle);

    target->Set(String::NewSymbol("CreateDataSourceOption"), String::New(ODrCCreateDataSource));
    target->Set(String::NewSymbol("DeleteDataSourceOption"), String::New(ODrCDeleteDataSource));
  }

}

} // namespace node_ogr

NODE_MODULE(ogr, node_ogr::Init)
