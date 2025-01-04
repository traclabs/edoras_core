
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
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_LONG_DOUBLE>
{
  using DataType = long double;
  using SequenceType = rosidl_runtime_c__long_double__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__long_double__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_CHAR>
{
  using DataType = signed char;
  using SequenceType = rosidl_runtime_c__char__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__char__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_WCHAR>
{
  using DataType = uint16_t;
  using SequenceType = rosidl_runtime_c__wchar__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__wchar__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN>
{
  using DataType = bool;
  using SequenceType = rosidl_runtime_c__bool__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__bool__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_OCTET>
{
  using DataType = uint8_t;
  using SequenceType = rosidl_runtime_c__octet__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__octet__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_UINT8>
{
  using DataType = uint8_t;
  using SequenceType = rosidl_runtime_c__uint8__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__uint8__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_INT8>
{
  using DataType = int8_t;
  using SequenceType = rosidl_runtime_c__int8__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__int8__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_UINT16>
{
  using DataType = uint16_t;
  using SequenceType = rosidl_runtime_c__uint16__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__uint16__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_INT16>
{
  using DataType = int16_t;
  using SequenceType = rosidl_runtime_c__int16__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__int16__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_UINT32>
{
  using DataType = uint32_t;
  using SequenceType = rosidl_runtime_c__uint32__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__uint32__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_INT32>
{
  using DataType = int32_t;
  using SequenceType = rosidl_runtime_c__int32__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__int32__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_UINT64>
{
  using DataType = uint64_t;
  using SequenceType = rosidl_runtime_c__uint64__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__uint64__Sequence__init;
};

template<>
struct TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_INT64>
{
  using DataType = int64_t;
  using SequenceType = rosidl_runtime_c__int64__Sequence;
  static constexpr SequenceInitFunc<SequenceType> sequence_init =
    rosidl_runtime_c__int64__Sequence__init;
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
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT>(_buffer, mi, _members, _val);
        } break;
        
        case rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE:
        {  printf("Type double.... \n");
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_LONG_DOUBLE:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_LONG_DOUBLE>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_CHAR:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_CHAR>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_WCHAR:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_WCHAR>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN>(_buffer, mi, _members, _val);
        } break;

        // OCTET seems to be just uint8_t in DDS (byte type in ROS)
        // https://docs.ros.org/en/foxy/Concepts/About-ROS-Interfaces.html
        case rosidl_typesupport_introspection_c__ROS_TYPE_OCTET:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_OCTET>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT8:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_UINT8>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_INT8:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_INT8>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT16:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_UINT16>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_INT16:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_INT16>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT32:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_UINT32>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_INT32:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_INT32>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT64:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_UINT64>(_buffer, mi, _members, _val);
        } break;

        case rosidl_typesupport_introspection_c__ROS_TYPE_INT64:
        {  
           return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_INT64>(_buffer, mi, _members, _val);
        } break;
        
        case rosidl_typesupport_introspection_c__ROS_TYPE_STRING:
        {
          return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_STRING>(_buffer, mi, _members, _val);
        } break;

//        case rosidl_typesupport_introspection_c__ROS_TYPE_WSTRING:
//        {
//          return write_member<rosidl_typesupport_introspection_c__ROS_TYPE_WSTRING>(_buffer, mi, _members, _val);
//        } break;

        
        case rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE:
        {  
           auto ms = _members;
           ms.pop_back();
           return write_member_nested<RosTypeId>(_buffer, mi, ms, _val);
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

template<int RosTypeId>
bool write_member_sequence(uint8_t* _buffer, 
                           const MemberInfo_t &_mi, 
                           const std::vector<std::string> &_members,
                           void* _val)
{
   // Read index
   if(_members.size() != 2 )
   {
      printf("Members size to write sequence should be 2, it is : %d \n", _members.size());
      return false;
   }
   size_t index = atoi( _members[0].c_str());

   using SequenceType = typename TypeMapping<RosTypeId>::SequenceType;

   auto seq = reinterpret_cast<SequenceType *>(_buffer);
   return write_member_item<RosTypeId>(reinterpret_cast<uint8_t *>(seq->data + index), _members, _val );
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
   if (is_sequence(_mi))
     return write_member_sequence<RosTypeId>(_buffer + _mi.offset_, _mi, _members, _val);
     
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
   double d = *(static_cast<DataType*>(_val));
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

/**
 * @brief sequence: Dynamically-sized array (bounded or unbounded).
 * @brief array: Fixed-sized array.
 * @brief Unbounded array is what we are mostly used to from ROS1 (e.g. joint_state.position)
 */
bool is_sequence(const MemberInfo_t &_mi)
{
   if ( (_mi.is_array_ && _mi.array_size_ == 0) || _mi.is_upper_bound_ )
     return true;
   else
     return false;
}

/**
 * @function is_primitive
 */
bool is_primitive(const int &RosTypeId)
{
  if( RosTypeId == rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT || 
      RosTypeId == rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE ||
      RosTypeId == rosidl_typesupport_introspection_c__ROS_TYPE_CHAR || 
      RosTypeId == rosidl_typesupport_introspection_c__ROS_TYPE_WCHAR ||
      RosTypeId == rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN || 
      RosTypeId == rosidl_typesupport_introspection_c__ROS_TYPE_STRING )
      return true;
  else
      return false;
}

////////////////////////

/**
 * @brief TODO: For Phase-I we are only resizing up to first-level of arrays (e.g. jointstate: position[]
 * @brief Resizing at a deeper level should not be much different, I just didn't have time to implement it robustly
 */
bool resize_sequence_impl(uint8_t* _buffer,
                          const TypeInfo_t* _ti, 
                          const std::vector<std::string> &_members,
                          const size_t &_size)
{
   // 1. Go to member that matched the member's name
   for(uint32_t i = 0; i < _ti->member_count_; ++i)
   {
      const MemberInfo_t& mi = _ti->members_[i];
      
      if(mi.name_ != _members.back())
      	continue;

      if(_members.size() != 1)
      {
         printf("ERROR: Phase-I we are only implementing depth-1 resizing for arrays \n");
         return false;
      }

      if( !is_primitive(mi.type_id_))
      {
         printf("ERROR: Phase-I we are resizing only primitive types for prototyping purposes. \n");
         return false;
      }

      
      switch(mi.type_id_)
      {
        case  rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT:
        {
           using SequenceType = typename TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT>::SequenceType;            
           auto seq = reinterpret_cast<SequenceType *>(_buffer + mi.offset_);
           return TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT>::sequence_init(seq, _size);      	
        } break;

        case  rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE:
        {  
           using SequenceType = typename TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE>::SequenceType;            
           auto seq = reinterpret_cast<SequenceType *>(_buffer + mi.offset_);
           return TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE>::sequence_init(seq, _size);      	
        } break;

        case  rosidl_typesupport_introspection_c__ROS_TYPE_STRING:
        {
           using SequenceType = typename TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_STRING>::SequenceType;            
           auto seq = reinterpret_cast<SequenceType *>(_buffer + mi.offset_);
           return TypeMapping<rosidl_typesupport_introspection_c__ROS_TYPE_STRING>::sequence_init(seq, _size);      	
        } break;
        
      }
           
   } // for i

   return false;

}

