
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

uint8_t* from_msg_pointer_to_uint_buffer_impl( const uint8_t* _msg_data, 
                                               const TypeSupport_t* _ts, 
                                               const TypeInfo_t* _ti, 
                                               size_t* _buffer);

uint8_t* create_msg_impl(const TypeInfo_t* _ti);

void printBufferImpl(const uint8_t* _data, size_t _data_size, const char* _msg, size_t _offset = 0);

std::vector<std::string> split(const char* _name, char _delimiter, bool _backwards);

///////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool msg_to_val_impl(const uint8_t* _buffer, 
                     const TypeInfo_t* _ti, 
                     const std::vector<std::string> &_members, 
                     T* _val);

template <typename T>
bool member_to_val_impl(const MemberInfo_t &_mi, 
                        const uint8_t* _buffer, 
                        const std::vector<std::string> &_members, 
                        T* _val);

template <typename T>
bool primitive_to_val_impl(const MemberInfo_t &_mi, 
                           const uint8_t* _buffer, 
                           T* _val);

template <typename T>
bool dynamic_array_element_to_val(const MemberInfo_t &_mi, 
                                  const uint8_t* _buffer, 
                                  size_t _index, 
                                  T* _val);

#include <edoras_core/message_reading_impl.hpp>
                                          
////////////////////////////////////////////////////////////////////////////

template <typename T>
bool val_to_msg_field_impl(uint8_t* _buffer, 
                           const TypeInfo_t* _ti, 
                           const std::vector<std::string> &_members, 
                           const T &_val);                                          

template<typename T>
bool write_member_nested(uint8_t* _buffer,
                         const MemberInfo_t &_mi,
                         const std::vector<std::string> &_members,
                         const T& _val);
                             
template<int RosTypeId>
bool write_member(uint8_t* _buffer,
                  const MemberInfo_t &_mi,
                  const std::vector<std::string> &_members, 
                  void* _val);                             
                    
template<int RosTypeId>                    
bool write_member_item(uint8_t* _buffer,
                       const std::vector<std::string> &_members,
                       void* _val);   
                       
bool is_sequence(const MemberInfo_t &_mi);
bool is_primitive(const int &RosTypeId);

bool resize_sequence_impl(uint8_t* _buffer,
                          const TypeInfo_t* _ti, 
                          const std::vector<std::string> &_members,
                          const size_t &_size);

#include <edoras_core/message_parsing_impl.hpp>                                          
                                                                               
#endif /* __EDORAS_CONVERSION_PRIVATE__ */
