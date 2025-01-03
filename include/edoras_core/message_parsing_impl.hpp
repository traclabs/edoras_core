
#include <rosidl_runtime_c/string.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rosidl_runtime_c/primitives_sequence_functions.h>

// Helper structures
// based on dynmsg/msg_parser_c.cpp

template<typename SequenceType>
using SequenceInitFunc = bool (*)(SequenceType *, size_t);

template<int RosTypeId>
struct TypeMapping 
{};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT>
{
   using DataType = float;
   using SequenceType = rosidl_runtime_c__float32__Sequence;
   static constexpr SequenceInitFunc<SequenceType> sequence_init =
     rosidl_runtime_c__float32__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE>
{
   using DataType = double;
   using SequenceType = rosidl_runtime_c__double__Sequence;
   static constexpr SequenceInitFunc<SequenceType> sequence_init =
     rosidl_runtime_c__double__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_STRING>
{
   using DataType = rosidl_runtime_c__String;
   using SequenceType = rosidl_runtime_c__String__Sequence;
   static constexpr SequenceInitFunc<SequenceType> sequence_init =
     rosidl_runtime_c__String__Sequence__init;
};


/**
 * @function val_to_msg_field_impl
 * @brief _members: (position.x)
 */
template <int RosTypeId>
bool val_to_msg_field_impl(uint8_t* _buffer, 
                           const TypeInfo_t* _ti, 
                           const std::vector<std::string> &_members, 
                           void* _val)
{
   // 1. Go to member that matched the member's name
   for(uint32_t i = 0; i < _ti->member_count_; ++i)
   {
      const MemberInfo_t& mi = _ti->members_[i];
      
      if(mi.name_ != _members.back())
      	continue;
      	
      if( _members.size() == 1 && (RosTypeId != mi.type_id_) )
      {
         printf("Error: Trying to set a value with mismatched type \n");
         return false;
      }
            	
      switch(mi.type_id_) {
      
        case rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT:
        {
           write_member<rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT>(_buffer, mi, _members, _val);
           return true;
        } break;
        
        case rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE:
        {
           write_member<rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE>(_buffer, mi, _members, _val);
           return true;
        } break;
        
        case rosidl_typesupport_introspection_c__ROS_TYPE_STRING:
        {
          write_member<rosidl_typesupport_introspection_c__ROS_TYPE_STRING>(_buffer, mi, _members, _val);
          return true;
        } break;
        
        case rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE:
        {  
           auto ms = _members;
           write_member_nested<RosTypeId>(_buffer, mi, ms, _val);
        } break;

      } // switch
      	
           
   } // for i

   return false;
}

/**
 * @function write_member_nested
 */
template<int RosTypeId>
bool write_member_nested(uint8_t* _buffer,
                         const MemberInfo_t &_mi,
                         const std::vector<std::string> &_members,
                         void* _val)
{
   // if (is_sequence())
   
   const TypeInfo_t * ti = reinterpret_cast<const TypeInfo_t *>(_mi.members_->data);
   if (_mi.is_array_) {
   
   } else {
     return val_to_msg_field_impl<RosTypeId>(_buffer + _mi.offset_, ti, _members, _val);
   }
   
   return false;
}



/**
 * @function write_member
 */
template<int RosTypeId>
bool write_member(uint8_t* _buffer,
                  const MemberInfo_t &_mi,
                  const std::vector<std::string> &_members, 
                  void* _val)
{
   // Handle sequences
   
   // Handle classic C arrays
   if(_mi.is_array_)
   {
   
   } else {
     return write_member_item<RosTypeId>(_buffer + _mi.offset_, _members, _val);   
   }
   
   return false;
}                   

/**
 * @function write_member_item - Generic
 * (types: float, double, int, char)
 */
template<int RosTypeId>
bool write_member_item(uint8_t* _buffer,
                       const std::vector<std::string> &_members, 
                       void* _val)
{
   using DataType = typename TypeMapping<RosTypeId>::DataType;
   *reinterpret_cast<DataType *>(_buffer) = *(static_cast<DataType*>(_val));
   return true;
}

// Specific for string
template<>
bool write_member_item<rosidl_typesupport_introspection_c__ROS_TYPE_STRING>(uint8_t* _buffer,
  const std::vector<std::string> &_members,
  void* _val)
{
   using DataType = typename TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_STRING>::DataType;
   std::string val_string= *(static_cast<std::string*>(_val));
   DataType* ros_string = reinterpret_cast<DataType *>(_buffer);
   if (!rosidl_runtime_c__String__assign(ros_string, val_string.data())) 
   {
      printf("Runtime error - Error assigning rosidl string");
      return false;
   }  
   
   return true;
}  


