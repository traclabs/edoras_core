
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


/**
 * @function val_to_msg_field_impl
 * @brief _members: (position.x)
 */
template <typename T>
bool val_to_msg_field_impl(uint8_t* _buffer, 
                           const TypeInfo_t* _ti, 
                           const std::vector<std::string> &_members, 
                           const T &_val)
{
   // 1. Go to member that matched the member's name
   for(uint32_t i = 0; i < _ti->member_count_; ++i)
   {
      const MemberInfo_t& mi = _ti->members_[i];
      
      if(mi.name_ != _members.back())
      	continue;
      	
      switch(mi.type_id_) {
      
        case rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT:
        {
           write_member<rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT, T>(_buffer, mi, _members, _val);
           return true;
        }
        case rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE:
        {
           write_member<rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE, T>(_buffer, mi, _members, _val);
           return true;
        }
        case rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE:
        {  
           auto ms = _members;
           ms.pop_back();
           write_member_nested<T>(_buffer, mi, ms, _val);
        }
      
      } // switch
      	
           
   } // for i

   return false;
}

/**
 * @function write_member_nested
 */
template<typename T>
bool write_member_nested(uint8_t* _buffer,
                         const MemberInfo_t &_mi,
                         const std::vector<std::string> &_members,
                         const T& _val)
{
   // if (is_sequence())
   
   const TypeInfo_t * ti = reinterpret_cast<const TypeInfo_t *>(_mi.members_->data);
   if (_mi.is_array_) {
   
   } else {
     return val_to_msg_field_impl<T>(_buffer + _mi.offset_, ti, _members, _val);
   }
   
   return false;
}



/**
 * @function write_member
 */
template<int RosTypeId, typename T>
bool write_member(uint8_t* _buffer,
                  const MemberInfo_t &_mi,
                  const std::vector<std::string> &_members, 
                  const T& _val)
{
   // Handle sequences
   
   // Handle classic C arrays
   if(_mi.is_array_)
   {
   
   } else {
     return write_member_item<RosTypeId, T>(_buffer + _mi.offset_, _members, _val);   
   }
   
   return false;
}                   

/**
 * @function write_member_item
 */
template<int RosTypeId, typename T>
bool write_member_item(uint8_t* _buffer,
                       const std::vector<std::string> &_members, 
                       const T& _val)
{
   using DataType = typename TypeMapping<RosTypeId>::DataType;
   *reinterpret_cast<DataType *>(_buffer) = _val;
   return true;
}
