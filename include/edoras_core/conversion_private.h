
#ifndef __EDORAS_CONVERSION_PRIVATE__
#define __EDORAS_CONVERSION_PRIVATE__

#include <edoras_core/types.h>

void* get_type_support_library_impl(const char* _interface_name, 
                                    const char* _interface_type);
                                    
const TypeInfo_t* get_type_info_impl(const char* _interface_name, 
                                     const char* _interface_type);                                 
                                    
const TypeSupport_t* get_type_support_impl(const char* _interface_name, 
                                           const char* _interface_type, 
                                           void* _ts_library);

uint8_t* from_uint_buffer_to_msg_pointer_impl(const uint8_t* _buffer, size_t _offset, 
                                              const TypeSupport_t* _ts, 
                                              const TypeInfo_t* _ti, 
                                              size_t* _buffer_size);                                           
                                                                               
#endif /* __EDORAS_CONVERSION_PRIVATE__ */
