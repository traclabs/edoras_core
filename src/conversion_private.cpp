
#include <edoras_core/conversion_private.h>

#include <rmw/rmw.h>
#include <rcutils/types/uint8_array.h>

#include <dlfcn.h>
#include <string>

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
  printf("edoras_core: Loaded type support %s", introspection_ts->typesupport_identifier);
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
  printf("edoras_core. Loaded type support %s", ts->typesupport_identifier);

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
  printf("Offset start: %d \n", offset);
  // The buffer contains buffer_size + buffer_capacity
  size_t buffer_length;
  size_t buffer_capacity;
  memcpy( &buffer_length, _buffer + offset, sizeof(size_t));
  offset += sizeof(size_t); printf("Offset now: %d  , with size of size_t: %d \n", offset, sizeof(size_t));
  memcpy( &buffer_capacity, _buffer + offset, sizeof(size_t));  
  offset += sizeof(size_t); printf("Offset now: %d \n", offset);
  printf("Buffer length: %lu buffer capacity: %lu. Offset up to here: %d **** \n", buffer_length, buffer_capacity, offset);

  printf("Full buffer contents with header and all, including offset: %lu \n", _offset);
  for(size_t i = 0; i < buffer_length + _offset + sizeof(size_t) + sizeof(size_t); ++i)
  {
   uint8_t di;
   memcpy(&di, (uint8_t*)_buffer + i, sizeof(uint8_t));
   printf("%02x ", di);
  } printf("\n");



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
    printf("edoras_core: Error initializing array for deserialization process. RESULT: %d \n");
  }
  printf("Buffer length: %d capacity: %d. Offset: %d \n", 
          serialized_array->buffer_length, 
          serialized_array->buffer_capacity, offset);
  
  memcpy( (uint8_t*)serialized_array->buffer, _buffer + offset, buffer_length);
  serialized_array->buffer_length = buffer_length;
  printf("Buffer length 2: %d capacity: %d \n", serialized_array->buffer_length, serialized_array->buffer_capacity);

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
  printf("Allocating... Size: %ld \n", *_buffer_size);
  
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
