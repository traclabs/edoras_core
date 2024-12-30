

#include <edoras_core/interface.h>
#include <string>

#include <edoras_core/conversion_private.h>
#include <edoras_core/debug_helpers.h>

/**
 * @function get_type_support_library
 */
void* get_type_support_library(const char* _interface_name, 
                               const char* _interface_type)
{
  return get_type_support_library_impl(_interface_name, _interface_type);
}
   
/**
 * @function get_type_info
 */                                       
const TypeInfo_t* get_type_info(const char* _interface_name, 
                                const char* _interface_type)
{
  return get_type_info_impl(_interface_name, _interface_type);
}

/**
 * @function get_type_support
 */
const TypeSupport_t* get_type_support(const char* _interface_name, 
                                      const char* _interface_type, 
                                      void* _ts_library)
{
  return get_type_support_impl(_interface_name, _interface_type, _ts_library);
}
                                               
uint8_t* from_uint_buffer_to_msg_pointer( const uint8_t* _buffer, size_t _offset, 
                                          const TypeSupport_t* _ts, 
                                          const TypeInfo_t* _ti, 
                                          size_t* _buffer_size)
{
  return from_uint_buffer_to_msg_pointer_impl(_buffer, _offset, _ts, _ti, _buffer_size);
}

void debug_parse_buffer(uint8_t* _buffer, const TypeInfo_t* _ti)
{
   return debug_parse_buffer_impl(_buffer, _ti);
}

uint8_t* create_msg(const TypeInfo_t* _ti)
{
   return create_msg_impl(_ti);
}

//////////////////////////////////////////

bool get_float64(const uint8_t* _buffer, const TypeInfo_t* _ti, const char* _member_names, double* _val)
{
   // 1. Get the name separated by '.'
   std::vector<std::string> members = split(_member_names, '.', true);

   return msg_to_val_impl(_buffer, _ti, members, _val);
}
