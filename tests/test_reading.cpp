
#include <edoras_core/interface.h>

int main(int argc, char* argv[])
{
  const char* int_name = "std_msgs";
  const char* int_type = "Header";
  
  printf("Get type info \n");
  const TypeInfo_t* ti = get_type_info(int_name, int_type);
  
  // Create a message 
  printf("Initializing a message \n");
  uint8_t* msg = create_msg(ti);
  
  // Fill values
  printf("Set const char \n");
  set_const_char(msg, ti, "frame_id", "world_frame");
  
  // Get values
  const char* read_frame;
  printf("Get const char \n");
  get_const_char(msg, ti, "frame_id", read_frame);
  
  printf("Gotten frame: %s \n", read_frame);   
}  
