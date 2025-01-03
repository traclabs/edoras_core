
#include <edoras_core/conversion_private.h>

#include <rmw/rmw.h>
#include <rcutils/types/uint8_array.h>

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

  // DEBUG
  //printBufferImpl(_buffer, buffer_length, "*-*- Buffer content in gateway *-*-", offset);

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
  
  memcpy( (uint8_t*)serialized_array->buffer, _buffer + offset, buffer_length);
  serialized_array->buffer_length = buffer_length;

  // DEBUG
  //printBufferImpl( (uint8_t*)serialized_array->buffer, buffer_length, "*-*- Buffer content in serial *-*-", 0);
 
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
  if ( rmw_deserialize( serialized_array, _ts, data ) != RMW_RET_OK ) {
    printf("edoras_core: Failed to apply rmw_deserialize \n");
    return nullptr;
  }
 
 return data;
}

/**
 * @function from_msg_pointer_to_uint_buffer_impl
 * @input Pointer to a ROS C Class representing a message (_msg_data)
 * @output A buffer (uint8_t*) containing buffer_length + buffer_capacity + serialized_msg
 */
uint8_t* from_msg_pointer_to_uint_buffer_impl( const uint8_t* _msg_data, 
                                           const TypeSupport_t* _ts, 
                                           const TypeInfo_t* _ti, 
                                           size_t* _buffer_size)
{
   // 1. Convert _msg_data to SerializedMessage type
   rcutils_allocator_t allocator = rcutils_get_default_allocator();
   // Initialization: From rclcpp/src/serialized_message.cpp
   rcutils_uint8_array_t* serialized_array = new rcutils_uint8_array_t(rmw_get_zero_initialized_serialized_message());

   rmw_serialized_message_init(serialized_array, 0u, &allocator);   

   // Read that this is not implemented by most libraries. Should be deprecated, returns 0
   //rosidl_runtime_c__Sequence__bound bounds;
   //rmw_get_serialized_message_size(_ts, &bounds, &buffer_size);

   rmw_ret_t res = rmw_serialize(_msg_data, _ts, serialized_array);

    // Pack up the data:
    // size_t (8 bytes containing buffer_length)
    // size_t (8 bytes containing buffer_capacity)
    // serialized_array (buffer_length bytes containing the serialized message)          
   *_buffer_size = sizeof(size_t) + sizeof(size_t) + serialized_array->buffer_length;
   
   uint8_t* buffer = static_cast<uint8_t *>( malloc(*_buffer_size) );
   
   // Fill
   size_t offset = 0;
   memcpy( buffer + offset, &serialized_array->buffer_length, sizeof(size_t));
   offset += sizeof(size_t);
   memcpy( buffer + offset, &serialized_array->buffer_capacity, sizeof(size_t));
   offset += sizeof(size_t);
   
   memcpy( buffer + offset, serialized_array->buffer, serialized_array->buffer_length);
   // Line above equivalent to:
   //for(int i = 0; i < serialized_array->buffer_length; ++i)
   //   *(buffer + offset + i) = *(serialized_array->buffer + i);
  
   // Clean up
   res = rcutils_uint8_array_fini(serialized_array);
   if(res != RCUTILS_RET_OK)
     printf("edoras_core: Error releasing resources from serialized_array used to create buffer! \n");       
           
   return buffer;
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

///////////////////////////////
// Helpers

void printBufferImpl(const uint8_t* _data, size_t _data_size, const char* _msg, size_t _offset )
{
   printf("\n %s \n", _msg);
   for(size_t i = 0; i < _data_size; i++)
   {
      printf("%02x ", *(_data + _offset + i) );
      if(i % 8 == 7)
        printf("\n");
   } printf("\n");

}

