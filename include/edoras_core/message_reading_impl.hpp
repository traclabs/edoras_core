
#include <rosidl_typesupport_introspection_c/field_types.h>


/**
 * @function get_value_impl
 * @brief members: (x.position, w.linear)
 */
template <typename T>
bool msg_to_val_impl(const uint8_t* _buffer, 
                     const TypeInfo_t* _ti, 
                     const std::vector<std::string> &_members, 
                     T* _val)
{
   for(uint32_t i = 0; i < _ti->member_count_; i++)
   {
       const MemberInfo_t& member_info = _ti->members_[i];
       const uint8_t* member_data = &_buffer[member_info.offset_];
       if (member_info.name_ != _members.back())
         continue;
         
       auto ms = _members;
       ms.pop_back();
       return member_to_val_impl<T>(member_info, member_data, ms, _val);                   
   } // for i
     
   return false;   
}

/**
 * @function member_to_val_impl
 */
template <typename T>
bool member_to_val_impl(const MemberInfo_t &_mi, 
                        const uint8_t* _buffer, 
                        const std::vector<std::string> &_members, 
                        T* _val)
{
   if( _mi.is_array_)
   {
     // Read index
     if(_members.size() != 1 )
     {
        printf("Members size to get array member should be 1 \n");
        return false;
     }
     size_t index = atoi( _members[0].c_str());
      
     if( _mi.is_upper_bound_ || _mi.array_size_ == 0)
       return dynamic_array_element_to_val<T>(_mi, _buffer, index, _val); 
     else
     {
       //fixed_array_element_to_val(_mi, _buffer, index, _val);     
       return false;
     }
   } else {
   
     if(_mi.type_id_ == rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE) {

       const TypeInfo_t* ti = reinterpret_cast<const TypeInfo_t*>(_mi.members_->data);
       return msg_to_val_impl(_buffer, ti, _members, _val);                 
     } else {    
       return primitive_to_val_impl<T>(_mi, _buffer, _val);
     }
   
   
   }
   
   return false;
}

template <typename T>
bool dynamic_array_element_to_val(const MemberInfo_t &_mi, 
                                  const uint8_t* _buffer, 
                                  size_t _index, 
                                  T* _val)
{
  switch(_mi.type_id_)
  {
     case rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE:
     {
       *_val = *( reinterpret_cast<const double *>(&(reinterpret_cast<const rosidl_runtime_c__double__Sequence *>(_buffer))->data[_index]) );
       return true;
     } break; 
     case rosidl_typesupport_introspection_c__ROS_TYPE_STRING:
     {
       std::string value;
       value = std::string(reinterpret_cast<const rosidl_runtime_c__String *>(&(reinterpret_cast<const rosidl_runtime_c__String__Sequence *>(_buffer))->data[_index])->data);
       *reinterpret_cast<std::string *>(_val) = value.c_str();
       return true;
         
       //*_val = *(reinterpret_cast<const rosidl_runtime_c__String *>(&(reinterpret_cast<const rosidl_runtime_c__String__Sequence *>(_buffer))->data[_index])->data);
      // *_val = value.data();
      // *_val = *value.c_str();
       ///         *_val = *reinterpret_cast<const rosidl_runtime_c__String *>(_buffer)->data;
       printf("Recovering val string---: %s \n", value.c_str());
       return true;
     } break; 
     
     
  }
  
  return false;
}                        

/**
 * @function primitive_to_val_impl
 */
template <typename T>
bool primitive_to_val_impl(const MemberInfo_t &_mi, 
                           const uint8_t* _buffer, 
                           T* _val)
{
   switch(_mi.type_id_)
   {
      case rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT:
      {
         *_val = *reinterpret_cast<const float *>(_buffer);
         return true;
      }

      case rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE:
      {
         *_val = *reinterpret_cast<const double *>(_buffer);
         return true;
      }

      case rosidl_typesupport_introspection_c__ROS_TYPE_LONG_DOUBLE:
      {
         *_val = *reinterpret_cast<const long double *>(_buffer);
         return true;
      }

      case rosidl_typesupport_introspection_c__ROS_TYPE_UINT8:
      {
         *_val = *reinterpret_cast<const uint8_t *>(_buffer);
         return true;
      }

      case rosidl_typesupport_introspection_c__ROS_TYPE_INT8:
      {
         *_val = *reinterpret_cast<const int8_t *>(_buffer);
         return true;
      }

      case rosidl_typesupport_introspection_c__ROS_TYPE_UINT16:
      {
         *_val = *reinterpret_cast<const uint16_t *>(_buffer);
         return true;
      }

      case rosidl_typesupport_introspection_c__ROS_TYPE_INT16:
      {
         *_val = *reinterpret_cast<const int16_t *>(_buffer);
         return true;
      }


      case rosidl_typesupport_introspection_c__ROS_TYPE_UINT32:
      {
         *_val = *reinterpret_cast<const uint32_t *>(_buffer);
         return true;
      }

      case rosidl_typesupport_introspection_c__ROS_TYPE_INT32:
      {
         *_val = *reinterpret_cast<const int32_t *>(_buffer);
         return true;
      }


      case rosidl_typesupport_introspection_c__ROS_TYPE_UINT64:
      {
         *_val = *reinterpret_cast<const uint64_t *>(_buffer);
         return true;
      }

      case rosidl_typesupport_introspection_c__ROS_TYPE_INT64:
      {
         *_val = *reinterpret_cast<const int64_t *>(_buffer);
         return true;
      }

      case rosidl_typesupport_introspection_c__ROS_TYPE_STRING:
      {
         //*_val = *reinterpret_cast<const rosidl_runtime_c__String *>(_buffer)->data;
         std::string value = std::string(reinterpret_cast<const rosidl_runtime_c__String *>(_buffer)->data);
         *reinterpret_cast<std::string *>(_val) = value.c_str();
         return true;
      }


  
   } // switch

  return false;
}



