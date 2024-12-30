
#ifndef __EDORAS_CONVERSION_PRIVATE__
#define __EDORAS_CONVERSION_PRIVATE__

#include <edoras_core/types.h>
#include <vector>
#include <string>

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

uint8_t* create_msg_impl(const TypeInfo_t* _ti);

bool msg_to_val_impl(const uint8_t* _buffer, 
                     const TypeInfo_t* _ti, 
                     const std::vector<std::string> &_members, 
                     double* _value);

bool member_to_val_impl(const MemberInfo_t &_mi, 
                        const uint8_t* _buffer, 
                        const std::vector<std::string> &_members, 
                        double* _val);

std::vector<std::string> split(const char* _name, char _delimiter, bool _backwards);
                                                                               
#endif /* __EDORAS_CONVERSION_PRIVATE__ */
