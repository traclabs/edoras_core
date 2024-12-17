#ifndef __EDORAS_CORE_TYPES__
#define __EDORAS_CORE_TYPES__

#include <rosidl_typesupport_introspection_c/message_introspection.h>

// TypeSupport
using TypeSupport_t = rosidl_message_type_support_t;

// Structure used to store the introspection information for a single interface type
using TypeInfo_t = rosidl_typesupport_introspection_c__MessageMembers;

// Structure used to store the introspection information for a single field of a interface type
using MemberInfo_t = rosidl_typesupport_introspection_c__MessageMember;

typedef const rosidl_message_type_support_t * (* get_message_ts_func)();

#endif // __EDORAS_CORE_TYPES__
