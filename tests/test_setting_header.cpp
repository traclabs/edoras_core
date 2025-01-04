
#include <edoras_core/interface.h>
#include <stdio.h>
#include <string>


int main(int argc, char* argv[])
{
  const char* int_name = "geometry_msgs";
  const char* int_type = "PoseStamped";
  
  const TypeInfo_t* ti = get_type_info(int_name, int_type);
  
  // Create a message 
  printf("Initializing a message \n");
  uint8_t* msg = create_msg(ti);
  
  printf("BEFORE...................\n");
  
  // Debug parse:
  debug_parse_buffer(msg, ti);
  
  // Fill values
  if( !set_const_char(msg, ti, "header.frame_id", "world_edoras"))
    printf("ERROR setting frame_id! \n");


  char frame[EDORAS_CONST_CHAR_MAX_SIZE];
  printf("Get const char \n");
  get_const_char(msg, ti, "header.frame_id", frame);
  printf("Gotten frame: : %s \n", frame);    
  
  printf("AFTER...................\n");  
    debug_parse_buffer(msg, ti);
    printf("\n");
  

}  
