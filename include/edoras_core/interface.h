/**
 * @file interface.h
 * @brief Included in the C files in our cFS app
 */
#ifndef __EDORAS_CORE_INTERFACE__
#define __EDORAS_CORE_INTERFACE__

#include <edoras_core/types.h>

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

  void* get_type_support_library(const char* _interface_name, 
                                 const char* _interface_type);
                                       
  const TypeInfo_t* get_type_info(const char* _interface_name, 
                                  const char* _interface_type);

  const TypeSupport_t* get_type_support_2(const char* _interface_name, 
                                          const char* _interface_type, 
                                          void* _ts_library);
                                               
                                               // TODO: IS IT CONST?
  uint8_t* from_uint_buffer_to_msg_pointer( const uint8_t* _buffer, size_t _offset, 
                                            const TypeSupport_t* _ts, 
                                            const TypeInfo_t* _ti, 
                                            size_t* _buffer_size);

  void debug_parse_buffer(uint8_t* _buffer, const TypeInfo_t* _ti);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __EDORAS_CORE_INTERFACE__
