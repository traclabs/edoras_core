

#include <edoras_core/interface.h>
#include <string>
#include <dlfcn.h>

typedef const rosidl_message_type_support_t * (* get_message_ts_func)();


void* get_type_support_library(const char* _interface_name, 
                               const char* _interface_type)
{

  return nullptr;
}
                                       
const TypeInfo_t* get_type_info(const char* _interface_name, 
                                const char* _interface_type)
{
  // Load the introspection library for the package containing the requested type
  std::string ts_lib_name;
  ts_lib_name = "lib" + std::string(_interface_name) + "__rosidl_typesupport_introspection_c.so";
  printf("edoras_core: Loading introspection type support library %s",
    ts_lib_name.c_str());
    
  void * introspection_type_support_lib = dlopen(ts_lib_name.c_str(), RTLD_LAZY);
  if (introspection_type_support_lib == nullptr) {
    printf("edoras_core: failed to load introspection type support library: %s", dlerror());
    return nullptr;
  }
  // Load the function that, when called, will give us the introspection information for the
  // interface type we are interested in
  std::string ts_func_name;
  ts_func_name = "rosidl_typesupport_introspection_c__get_message_type_support_handle__" +
    std::string(_interface_name) + "__msg__" + std::string(_interface_type);
  printf("edoras_core: Loading type support function %s", ts_func_name.c_str());

  get_message_ts_func introspection_type_support_handle_func =
    reinterpret_cast<get_message_ts_func>(dlsym(
      introspection_type_support_lib,
      ts_func_name.c_str()));
  if (introspection_type_support_handle_func == nullptr) {
    printf("edoras_core: Failed to load introspection type support function: %s",
      dlerror());
    return nullptr;
  }

  // Call the function to get the introspection information we want
  const TypeSupport_t * introspection_ts =
    introspection_type_support_handle_func();
  printf("edoras_core: Loaded type support %s", introspection_ts->typesupport_identifier);
  const TypeInfo_t * type_info =
    reinterpret_cast<const TypeInfo_t *>(
    introspection_ts->data);

  return type_info;


}

const TypeSupport_t* get_type_support_2(const char* _interface_name, 
                                        const char* _interface_type, 
                                        void* _ts_library)
{
 return nullptr;
}
                                               
                                               // TODO: IS IT CONST?
uint8_t* from_uint_buffer_to_msg_pointer( const uint8_t* _buffer, size_t _offset, 
                                          const TypeSupport_t* _ts, 
                                          const TypeInfo_t* _ti, 
                                          size_t* _buffer_size)
{

 return nullptr;
}

void debug_parse_buffer(uint8_t* _buffer, const TypeInfo_t* _ti)
{

}

