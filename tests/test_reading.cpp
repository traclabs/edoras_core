
#include <edoras_core/interface.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  const char* int_name = "geometry_msgs";
  const char* int_type = "PoseStamped";
  
  printf("Get type info \n");
  const TypeInfo_t* ti = get_type_info(int_name, int_type);
  
  // Create a message 
  printf("Initializing a message \n");
  uint8_t* msg = create_msg(ti);
  
  printf("BEFORE...................\n");
  
  // Debug parse:
  debug_parse_buffer(msg, ti);
  
  // Fill values
  printf("Starting to set float 64.... \n");
  if( !set_float64(msg, ti, "pose.position.x", 0.2))
    printf("ERROR setting pose.position.x! \n");
  if( !set_float64(msg, ti, "pose.position.y", 0.3))
    printf("ERROR setting pose.position.y \n");
  if( !set_float64(msg, ti, "pose.position.z", 0.4))
    printf("ERROR setting pose.position.z \n");
      
  printf("Set const char \n");
  set_const_char(msg, ti, "header.frame_id", "world_frame");
  
  // Get values
  char read_frame[EDORAS_CONST_CHAR_MAX_SIZE];
  get_const_char(msg, ti, "header.frame_id", read_frame);
  
  printf("Gotten frame back: %s \n", read_frame);   
  
  
  printf("AFTER...................\n");  
    debug_parse_buffer(msg, ti);
  

}  
