/**
 * @file interface.h
 * @brief Included in the C files in our cFS app
 */
#ifndef __EDORAS_CORE_INTERFACE__
#define __EDORAS_CORE_INTERFACE__

#include <edoras_core/types.h>
#define EDORAS_CONST_CHAR_MAX_SIZE 100

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

  void* get_type_support_library(const char* _interface_name, 
                                 const char* _interface_type);
                                       
  const TypeInfo_t* get_type_info(const char* _interface_name, 
                                  const char* _interface_type);

  const TypeSupport_t* get_type_support(const char* _interface_name, 
                                        const char* _interface_type, 
                                        void* _ts_library);
                                               
  // From buffer (SerializedMsg) to return ROS C message structure
  uint8_t* from_uint_buffer_to_msg_pointer( const uint8_t* _buffer, size_t _offset, 
                                            const TypeSupport_t* _ts, 
                                            const TypeInfo_t* _ti, 
                                            size_t* _buffer_size);

  void debug_parse_buffer(uint8_t* _buffer, const TypeInfo_t* _ti);

  // from ROS C msg structure to SerializedMsg
  uint8_t* from_msg_pointer_to_uint_buffer( uint8_t* _msg_data, 
                                            const TypeSupport_t* _ts, 
                                            const TypeInfo_t* _ti, 
                                            size_t* _buffer_size);

  // Init msg
  uint8_t* create_msg(const TypeInfo_t* _ti);  

  // Helpers
  void printBuffer(uint8_t* _data, size_t _data_size, const char* _msg);

  // Sequences resizing
  bool resize_sequence(uint8_t* _buffer,
                       const TypeInfo_t* _ti, 
                       const char* _member_names,
                       const size_t &_size);

  
  // Get data
  bool get_float64(const uint8_t* _buffer, const TypeInfo_t* _ti, const char* _member_name, double *_val);
  bool get_uint8(const uint8_t* _buffer, const TypeInfo_t* _ti, const char* _member_name, uint8_t* _val);
  bool get_const_char(const uint8_t* _buffer, const TypeInfo_t* _ti, const char* _member_name, char _val[EDORAS_CONST_CHAR_MAX_SIZE]);
  
  
  // Set data
  bool set_float64(uint8_t* _buffer, const TypeInfo_t* _ti, const char* _member_name, const double &_val);
  bool set_const_char(uint8_t* _buffer, const TypeInfo_t* _ti, const char* _member_name, const char* _val);
  
#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __EDORAS_CORE_INTERFACE__
