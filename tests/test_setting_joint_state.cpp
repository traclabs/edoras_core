
#include <edoras_core/interface.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  const char* int_name = "sensor_msgs";
  const char* int_type = "JointState";
  
  const TypeInfo_t* ti = get_type_info(int_name, int_type);
  
  // Create a message 
  printf("Initializing a message \n");
  uint8_t* msg = create_msg(ti);
  
  printf("BEFORE...................\n");
  
  // Debug parse:
  debug_parse_buffer(msg, ti);
  
  // Resize
  if(!resize_sequence(msg, ti, "position", 3))
    printf("ERROR Resizing position \n");

  resize_sequence(msg, ti, "name", 3);
  // Fill values
  if( !set_float64(msg, ti, "position.0", 0.2))
    printf("ERROR setting position.0! \n");
  if( !set_float64(msg, ti, "position.1", 0.3))
    printf("ERROR setting position.1 \n");
  if( !set_float64(msg, ti, "position.2", 0.4))
    printf("ERROR setting position.2 \n");

  if( !set_const_char(msg, ti, "name.0", "joint_0"))
    printf("ERROR setting name.0! \n");
  if( !set_const_char(msg, ti, "name.1", "joint_1"))
    printf("ERROR setting name.1 \n");
  if( !set_const_char(msg, ti, "name.2", "joint_2"))
    printf("ERROR setting name.2 \n");

      
  //printf("Set const char \n");
  //set_const_char(msg, ti, "header.frame_id", "world_frame");
  
  // Get values
  double position_0;
  double position_1;
  get_float64(msg, ti, "position.0", &position_0);
  printf("Gotten position_0: %f \n", position_0);    
  get_float64(msg, ti, "position.1", &position_1);
  printf("Gotten position_1: %f \n", position_1);    

  char name_0[EDORAS_CONST_CHAR_MAX_SIZE];
  char name_1[EDORAS_CONST_CHAR_MAX_SIZE];
  char name_2[EDORAS_CONST_CHAR_MAX_SIZE];  
  get_const_char(msg, ti, "name.0", name_0);
  printf("Gotten name_0: %s \n", name_0);    
  get_const_char(msg, ti, "name.0", name_0);
  printf("Gotten name_1: %s \n", name_1);    
  get_const_char(msg, ti, "name.1", name_1);
  printf("Gotten name_1: %s \n", name_1);    
  get_const_char(msg, ti, "name.2", name_2);
  printf("Gotten name_2: %s \n", name_2);    
  
  
  printf("AFTER...................\n");  
    debug_parse_buffer(msg, ti);
    printf("\n");
  

}  
