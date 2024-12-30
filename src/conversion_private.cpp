
#include <edoras_core/conversion_private.h>

#include <rmw/rmw.h>
#include <rcutils/types/uint8_array.h>
#include <rosidl_typesupport_introspection_c/field_types.h>

#include <dlfcn.h>
#include <string>
#include <sstream>

typedef const rosidl_message_type_support_t * (* get_message_ts_func)();


/**
 * @function get_type_support_library_impl
 */
void* get_type_support_library_impl(const char* _interface_name, 
                                    const char* _interface_type)
{
  // Load the type support library for the package containing the requested type
  std::string ts_lib_name;
  ts_lib_name = "lib" + std::string(_interface_name) + "__rosidl_typesupport_c.so";

  void * type_support_lib = dlopen(ts_lib_name.c_str(), RTLD_LAZY);
  if (type_support_lib == nullptr) {
    printf("edoras_core: Failed to load type support library: %s", dlerror());
    return nullptr;
  }

  return type_support_lib;
}


const TypeInfo_t* get_type_info_impl(const char* _interface_name, 
                                     const char* _interface_type)
{                                
  // Load the introspection library for the package containing the requested type
  std::string ts_lib_name;
  ts_lib_name = "lib" + std::string(_interface_name) + "__rosidl_typesupport_introspection_c.so";
  printf("edoras_core: Loading introspection type support library %s \n",
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
  printf("edoras_core: Loading type support function %s \n", ts_func_name.c_str());

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
  printf("edoras_core: Loaded type support %s \n", introspection_ts->typesupport_identifier);
  const TypeInfo_t * type_info =
    reinterpret_cast<const TypeInfo_t *>(
    introspection_ts->data);

  return type_info;
}

/**
 * @function get_type_support_impl
 */
const TypeSupport_t* get_type_support_impl(const char* _interface_name, 
                                           const char* _interface_type, 
                                           void* _ts_library)
{
  // Load the function that, when called, will give us the type support for the interface type we
  // are interested in
  std::string ts_func_name;
  ts_func_name = "rosidl_typesupport_c__get_message_type_support_handle__" + std::string(_interface_name) +
    "__msg__" + std::string(_interface_type);
  printf("edoras_core: Loading type support function %s \n", ts_func_name.c_str());

  get_message_ts_func type_support_handle_func =
    reinterpret_cast<get_message_ts_func>(dlsym(_ts_library, ts_func_name.c_str()));
  if (type_support_handle_func == nullptr) {
    printf("edoras_core: Failed to load type support function: %s", dlerror());
    return nullptr;
  }

  // Call the function to get the type support we want
  const TypeSupport_t * ts = type_support_handle_func();
  printf("edoras_core. Loaded type support %s \n", ts->typesupport_identifier);

  return ts;
}

/**
 * @function from_uint_buffer_to_msg_pointer_impl
 */
uint8_t* from_uint_buffer_to_msg_pointer_impl( const uint8_t* _buffer, size_t _offset, 
                                               const TypeSupport_t* _ts, 
                                               const TypeInfo_t* _ti, 
                                               size_t* _buffer_size)
{
  // 1. Get buffer that starts from _offset onwards
  //    this buffer is the binary rc structure
  size_t offset = _offset;
  // The buffer contains buffer_size + buffer_capacity
  size_t buffer_length;
  size_t buffer_capacity;
  memcpy( &buffer_length, _buffer + offset, sizeof(size_t));
  offset += sizeof(size_t);
  memcpy( &buffer_capacity, _buffer + offset, sizeof(size_t));  
  offset += sizeof(size_t);
  printf("Buffer length: %ld buffer capacity: %ld. Offset up to here: %ld **** \n", buffer_length, buffer_capacity, offset);

  printf("Buffer content in gateway: \n");
  for(size_t i = 0; i < buffer_length; ++i)
  {
   uint8_t di;
   memcpy(&di, (uint8_t*)_buffer + offset + i, sizeof(uint8_t));
   printf("%02x ", di);
  } printf("\n");

  // Initialize SerializedMessage
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  rcutils_uint8_array_t* serialized_array = new rcutils_uint8_array_t;
  *serialized_array = rcutils_get_zero_initialized_uint8_array();
  rcutils_uint8_array_init(serialized_array, buffer_capacity, &allocator);
 
  // Allocate space for the pointer to the C data
  if( rcutils_uint8_array_resize(serialized_array, buffer_capacity) != RCUTILS_RET_OK)
  {
    printf("edoras_core: Error initializing array for deserialization process \n");
  }
  printf("Buffer length: %ld capacity: %ld. Offset: %ld \n", 
          serialized_array->buffer_length, 
          serialized_array->buffer_capacity, offset);
  
  memcpy( (uint8_t*)serialized_array->buffer, _buffer + offset, buffer_length);
  serialized_array->buffer_length = buffer_length;
  printf("Buffer length 2: %ld capacity: %ld \n", serialized_array->buffer_length, serialized_array->buffer_capacity);

  printf("Buffer content in serial: \n");
  for(size_t i = 0; i < buffer_length; ++i)
  {
   uint8_t di;
   memcpy(&di, (uint8_t*)serialized_array->buffer + i, sizeof(uint8_t));
   printf("%02x ", di);
  } printf("\n");

  
  // Get default buffer size
  *_buffer_size = _ti->size_of_;


  // Initialise the message buffer according to the interface type
  // Allocate space to store the binary representation of the message
  printf("Allocating  %ld bytes for C structure ROS Msg \n", *_buffer_size);
  
  rcutils_allocator_t * alloca;
  rcutils_allocator_t default_allocator = rcutils_get_default_allocator();
  alloca = &default_allocator;
  
  uint8_t* data = static_cast<uint8_t *>(alloca->allocate(*_buffer_size, alloca->state));
  _ti->init_function(data, ROSIDL_RUNTIME_C_MSG_INIT_ALL);

  if (data == nullptr) {
    printf("edoras_core: Error allocating space \n");
    return nullptr;
  }

  
  // 3. Deserialize the message into a ROS message C structure
  printf("Deserializing.... \n");
  if ( rmw_deserialize( serialized_array, _ts, data ) != RMW_RET_OK ) {
    printf("Failed to apply rmw_deserialize \n");
    return nullptr;
  }

 return data;
 
 return nullptr;
}

/**
 * @function create_msg_impl
 */
uint8_t* create_msg_impl(const TypeInfo_t* _ti)
{
  rcutils_allocator_t default_allocator = rcutils_get_default_allocator();
  rcutils_allocator_t* allocator = &default_allocator;
  

  // Allocate space to store the binary representation of the message
  uint8_t * data =
    static_cast<uint8_t *>(allocator->allocate(_ti->size_of_, allocator->state));

  if (nullptr == data) {
    return nullptr;
  }
  // Initialise the message buffer according to the interface type
  _ti->init_function(data, ROSIDL_RUNTIME_C_MSG_INIT_ALL);

  return data;
}

/**
 * @function get_value_impl
 * @brief members: (x.position, w.linear)
 */
bool msg_to_val_impl(const uint8_t* _buffer, 
                     const TypeInfo_t* _ti, 
                     const std::vector<std::string> &_members, 
                     double* _val)
{   
   if( _members.size() == 1 )
   {
      for(uint32_t i = 0; i < _ti->member_count_; i++)
      {
         const MemberInfo_t& member_info = _ti->members_[i];
         const uint8_t* member_data = &_buffer[member_info.offset_];
         
         if( member_info.name_ != _members.back() )
           continue;
         
         if(member_info.type_id_ == rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE ||
            member_info.is_array_ )
            return false;
            
         if(member_info.type_id_ != rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE)
            return false;
              
         // Parse
         *_val = *reinterpret_cast<const double *>(member_data);
         return true;
                  
      } // for i 
      
   } else {

     for(uint32_t i = 0; i < _ti->member_count_; i++)
     {
         const MemberInfo_t& member_info = _ti->members_[i];
         const uint8_t* member_data = &_buffer[member_info.offset_];
         if (member_info.name_ != _members.back())
           continue;
         
         auto ms = _members;
         ms.pop_back();
         return member_to_val_impl(member_info, member_data, ms, _val);                   
     } // for i

   
   
   } // else
   
   return false;   
}

/**
 * @function member_to_val_impl
 */
bool member_to_val_impl(const MemberInfo_t &_mi, 
                        const uint8_t* _buffer, 
                        const std::vector<std::string> &_members, 
                        double* _val)
{
   if( _mi.is_array_)
   {
   
   } else {
   
     if(_mi.type_id_ == rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE) {

       const TypeInfo_t* ti = reinterpret_cast<const TypeInfo_t*>(_mi.members_->data);
       return msg_to_val_impl(_buffer, ti, _members, _val);                 
     }
   
   
   }
   
   return false;
}

/**
 * @function split
 */
std::vector<std::string> split(const char* _name, char _delimiter, bool _backwards)
{
   std::stringstream ss(_name);
   
   std::vector<std::string> names;
   std::string token;
   
   while(std::getline(ss, token, _delimiter))
     names.push_back(token);
 
   std::vector<std::string> result;
   if(_backwards)
   {
      for(int i = names.size() - 1; i >= 0; i--)
 	result.push_back(names[i]);     
   }
   else
     result = names;
     
   return result;
}
