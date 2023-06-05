#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define max 128

// employee details
typedef struct {
  char id[max];
  char name[max];
  char position[max];
  char department[max];
  char gender[max];
} employee;

// table dimensions
typedef struct{
  int id_w;
  int name_w;
  int dept_w;
  int pos_w;
  int gender_w;
  int table_w;
} dimensions;

dimensions table;

// tool functions
void load_files();
bool file_exists();
void put_to_file(employee details);
int id_exists(char * input_id);
void remove_from_file(char * employee_id);
void put_dir_to_temp(int skip_line);
void put_temp_to_dir();
void get_string_input(char * buffer, const int buffer_size);

// stack is used here:
void undo_action();

// main functions
void menu();
void navigate_menu();

void add_employee();
int get_input_count();
void get_employee_details();
void read_employee_id(char * buff, const int size);
void read_employee_fname(char * buff, const int size);
void read_employee_lname(char * buff, const int size);
void read_employee_dept(char * buff);
void read_employee_pos(char * buff);
void read_employee_gender(char * buff);

void view_employee();
void write_table_header();
void set_table_dimensions();
void draw_border(int length);
void add_space(const int width, int deduct);
int retrieve_from_file();
void print_line(employee details);

void update_employee();
void remove_employee();

int main(){
  (void)menu();
}

void print_line(employee details){
  (void)printf("\n\t");

  (void)printf("%s", details.id);
  (void)add_space(table.id_w, strlen(details.id));
  
  (void)printf("%s", details.name);
  (void)add_space(table.name_w, strlen(details.name));
  
  (void)printf("%s", details.department);
  (void)add_space(table.dept_w, strlen(details.department));
  
  (void)printf("%s", details.position);
  (void)add_space(table.pos_w, strlen(details.position));
  
  (void)printf("%s", details.gender);
  (void)add_space(table.gender_w, strlen(details.gender));
}

void clean_buffer(char * buffer, int size){
  for(int i = 0; i < size; i++){
    buffer[i] = '\0';
  }
}

int retrieve_from_file(){
  // buffer declaration
  char buff[max];

  // count retrieved lines
  int line_count = 0;

  // extract details from csv file
  FILE * fret = (FILE *)fopen("records.csv", "r");

  bool line_is_header = true;

  while((char *)fgets(buff, max, fret) != NULL){
    if(line_is_header){
      line_is_header = false;
      continue;
    }

    line_count++;

    int len = (int)strlen(buff);
    // remove newline
    buff[len - 1] = '\0';

    int comma = 0;
    int str_i = 0;

    // temporarily store in struct   
    employee current_employee;

    // clean struct members
    (void)clean_buffer(current_employee.id, max);
    (void)clean_buffer(current_employee.name, max);
    (void)clean_buffer(current_employee.department, max);
    (void)clean_buffer(current_employee.position, max);
    (void)clean_buffer(current_employee.gender, max);

    for(int i = 0; i < len; i++){
      if(buff[i] == ','){
        comma++;
        str_i = 0;
        continue;
      }
      switch(comma){
        case 0:
          current_employee.id[str_i] = buff[i];
          str_i++;
          break;
        case 1:
          current_employee.name[str_i] = buff[i];
          str_i++;
          break;
        case 2:
          current_employee.department[str_i] = buff[i];
          str_i++;
          break;
        case 3:
          current_employee.position[str_i] = buff[i];
          str_i++;
          break;
        case 4: 
          current_employee.gender[str_i] = buff[i];
          str_i++;
          break;
        default:
          (void)printf("\n\n(!) Program Error : Invalid ',' character detected.");
          (void)getch();
          (void)exit(1);
          break;
      }
    } // for loop }
    (void)print_line(current_employee);
  } // while loop }
  (void)fclose(fret);
  return line_count;
}

void add_space(const int width, int deduct){
  for(int i = 0; i < (width - deduct); i++){
    (void)printf(" ");
  }
}

void draw_border(int length){
  for(int i = 0; i < length; i++){
    (void)printf("-");
  }
}

void set_table_dimensions(){
  // set values for global struct table
  table.id_w = 10;
  table.name_w = 32;
  table.dept_w = 25;
  table.pos_w = 20;
  table.gender_w = 10;
  table.table_w = (table.id_w + table.name_w + table.dept_w + table.pos_w + table.gender_w);
}

void write_table_header(){
  // dimensions
  (void)set_table_dimensions();
  // top border
  (void)printf("\n\n\t");
  (void)draw_border(table.table_w);
  (void)printf("\n\t");

  // print headers, add space to achieve desired width
  char header_1[max] = "ID";
  (void)printf("%s", header_1);
  (void)add_space(table.id_w, (int)strlen(header_1));
  
  char header_2[max] = "Name";
  (void)printf("%s", header_2);
  (void)add_space(table.name_w, (int)strlen(header_2));
  
  char header_3[max] = "Department";
  (void)printf("%s", header_3);
  (void)add_space(table.dept_w, (int)strlen(header_3));

  char header_4[max] = "Position";
  (void)printf("%s", header_4);
  (void)add_space(table.pos_w, (int)strlen(header_4));

  char header_5[max] = "Gender";
  (void)printf("%s", header_5);
  (void)add_space(table.gender_w, (int)strlen(header_5));

  // add bottom border for header
  (void)printf("\n\t");
  (void)draw_border(table.table_w);
}

void view_employee(){
  // this table will be of static size
  (void)system("cls");
  (void)write_table_header();
  // total retrieved lines from file, prints them as well
  int ret_lines = (int)retrieve_from_file();
  // draw bottom border
  (void)printf("\n\n\t");
  (void)draw_border(table.table_w);
  // show total records
  (void)printf("\n\n\tTotal Records: %d", ret_lines);
  (void)getch();
  (void)menu();
}

void read_employee_gender(char * buff){
  (void)printf("\n\nEnter Employee Gender:\n");
  (void)printf("\n\t1 :: Male");
  (void)printf("\n\t2 :: Female\n");
  (void)printf("\n(i) Press ESC to cancel.");
  bool key_press_invalid = true;
  while(key_press_invalid){
    switch((char)getch()){
      case '1':
        (void)strcpy(buff, "Male");
        key_press_invalid = false;
        break;
      case '2':
        (void)strcpy(buff, "Female");
        key_press_invalid = false;
        break;
        case 27:
        (void)menu();
        key_press_invalid = false;
        break;
      default:
        continue;
    }
  }
}

void read_employee_pos(char * buff){
  (void)printf("\n\nEnter Employee Position:\n");
  (void)printf("\n\t1 :: Dept. Head");
  (void)printf("\n\t2 :: Dept. Deputy Head");
  (void)printf("\n\t3 :: Manager");
  (void)printf("\n\t4 :: Asst. Manager");
  (void)printf("\n\t5 :: Team Leader");
  (void)printf("\n\t6 :: Staff");
  (void)printf("\n\t7 :: Intern\n");
  (void)printf("\n(i) Press ESC to cancel.");
  bool key_press_invalid = true;
  while(key_press_invalid){
    switch((char)getch()){
      case '1':
        (void)strcpy(buff, "Dept. Head");
        key_press_invalid = false;
        break;
      case '2':
        (void)strcpy(buff, "Dept. Deputy Head");
        key_press_invalid = false;
        break;
      case '3':
        (void)strcpy(buff, "Manager");
        key_press_invalid = false;
        break;
      case '4':
        (void)strcpy(buff, "Asst. Manager");
        key_press_invalid = false;
        break;
      case '5':
        (void)strcpy(buff, "Team Leader");
        key_press_invalid = false;
        break;
      case '6':
        (void)strcpy(buff, "Staff");
        key_press_invalid = false;
        break;
      case '7':
        (void)strcpy(buff, "Intern");
        key_press_invalid = false;
        break;
      case 27:
        (void)menu();
        key_press_invalid = false;
        break;
      default:
        continue;
    }
  }
}

void read_employee_dept(char * buff){
  (void)printf("\n\nSelect Employee Department:\n");
  (void)printf("\n\t1 :: General");
  (void)printf("\n\t2 :: Marketing");
  (void)printf("\n\t3 :: Engineering");
  (void)printf("\n\t4 :: Design");
  (void)printf("\n\t4 :: Logistics\n");
  (void)printf("\n(i) Press ESC to cancel.");
  bool key_press_invalid = true;
  while(key_press_invalid){
    switch((char)getch()){
      case '1':
        (void)strcpy(buff, "General");
        key_press_invalid = false;
        break;
      case '2':
        (void)strcpy(buff, "Marketing");
        key_press_invalid = false;
        break;
      case '3':
        (void)strcpy(buff, "Engineering");
        key_press_invalid = false;
        break;
      case '4':
        (void)strcpy(buff, "Design");
        key_press_invalid = false;
        break;
      case '5':
        (void)strcpy(buff, "Logistics");
        key_press_invalid = false;
        break;
      case 27:
        (void)menu();
        key_press_invalid = false;
        break;
      default:
        continue;
    }
  }
}

void read_employee_lname(char * buff, const int size){
  for(int i = 0; i < 10; i++){
    (void)system("cls");
    (void)printf("\nEnter Employee Last Name: ");
    (void)get_string_input(buff, size);
    // check length
    int len = (int)strlen(buff);
    if(len > (table.name_w / 2)){
      // clear buffer
      (void)strcpy(buff, "");
      (void)printf("\n\n(!) <Last Name> length should be no more than %d characters.", (table.name_w / 2));
      if((int)getch() == 27){
        (void)menu();
      } else {
        continue;
      }
    } else {
      return;
    }
  }
    (void)printf("\n\n(!) Too many invalid inputs. Returning to menu ....");
    (void)getch();
  (void)menu();
}

void read_employee_fname(char * buff, const int size){
  for(int i = 0; i < 10; i++){
    (void)system("cls");
    (void)printf("\nEnter Employee First Name: ");
    (void)get_string_input(buff, size);
    // check length
    int len = (int)strlen(buff);
    if(len > (table.name_w / 2)){
      // clear buffer
      (void)strcpy(buff, "");
      (void)printf("\n\n(!) <First Name> length should be no more than %d characters.", (table.name_w / 2));
      if((int)getch() == 27){
        (void)menu();
      } else {
        continue;
      }
    } else {
      return;
    }
  }
  (void)printf("\n\n(!) Too many invalid inputs. Returning to menu ....");
  (void)getch();
  (void)menu();
}

bool id_is_invalid(char * buff, const int size){
  // valid id input : "123-123"
  if(size > 7 || size < 0){
    return true;
  }
  for(int i = 0; i < size; i++){
    if(buff[i] == '-'){
      if(i != 3){
        return true;
      } else {
        continue;
      }
    }
    if(buff[i] < '0' || buff[i] > '9'){
      return true;
    }
  }
  return false;
}

void read_employee_id(char * buff, const int size){
  // ten invalid attempts and the program quits to menu
  for(int i = 0; i < 10; i++){
    (void)system("cls");
    (void)printf("\n(i) Valid ID is written as such : \"123-123\"\n");
    (void)printf("\nEnter Employee ID: ");
    (void)get_string_input(buff, size);
    // id should only have numbers and dashes
    int len = (int)strlen(buff);
    if((bool)id_is_invalid(buff, len)){
      (void)printf("(!) Error: Invalid ID.");
      if((int)getch() == 27){
        (void)menu();
      } else {
        continue;
      }
    } 
    if((bool)id_exists(buff)){
      (void)printf("\n\n(!) Employee ID already exists.");
      if((int)getch() == 27){
        (void)menu();
      } else {
        continue;
      }
    }
    else {
      return;
    }
    (void)strcpy(buff, "");
  }
  (void)printf("\n\n(!) Too many invalid inputs. Returning to menu.");
  (void)getch();
  (void)menu();
}

void get_employee_details(){
  (void)system("cls");
  // write input to local variables
  char employee_id[max];
  (void)read_employee_id(employee_id, max);
  char employee_fname[max];
  (void)read_employee_fname(employee_fname, max);
  char employee_lname[max];
  (void)read_employee_lname(employee_lname, max);
  char employee_dept[max];
  (void)read_employee_dept(employee_dept);
  char employee_pos[max];
  (void)read_employee_pos(employee_pos);
  char employee_gender[max];
  (void)read_employee_gender(employee_gender);
  // then clone to struct
  employee new_employee;
  (void)strcpy(new_employee.id, employee_id);
  (void)strcpy(new_employee.name, employee_fname);
  (void)strcat(new_employee.name, " ");
  (void)strcat(new_employee.name, employee_lname);
  (void)strcpy(new_employee.position, employee_pos);
  (void)strcpy(new_employee.department, employee_dept);
  (void)strcpy(new_employee.gender, employee_gender);
  // finally, print to file
  (void)put_to_file(new_employee);
}

bool invalid_character_exists(char * string, const int size){
  // check for characters that are not numbers
  for(int i = 0; i < size; i++){
      if(string[i] < '0' || string[i] > '9'){
        return true;
      }
  }
  return false;
}

int get_input_count(){
    // count how many inputs are needed
    system("cls");
    // echo prompt
    (void)printf("\n(i) Note : You can only add 10 employees at a time.\n");
    (void)printf("\nHow many employees would you like to enter? : ");
    // get input in string format
    char str_input_count[max];
    (void)get_string_input(str_input_count, max);
    int len = (int)strlen(str_input_count);
    // check first for any invalid character before converting to int
    if((bool)invalid_character_exists(str_input_count, len)){
      (void)printf("\n\n(!) Error : Invalid characters detected.");
      if((int)getch() == 27){
        (void)menu();
      } else {
        (void)add_employee();
      }
    }
    // convert to int
    int input_count = (int)atoi(str_input_count);
    // take only up to 10 inputs
    if(input_count > 10){
      (void)printf("\n\n(!) You can only record up to 10 employees at a time.");
      if((int)getch() == 27){
        (void)menu();
      } else {
        (void)add_employee();
      }
    }
    // when all is good:
    return input_count;
}

void add_employee(){
    // read how many inputs are needed (10 max)
    int input_count = (int)get_input_count();
    // then call this function iteratively
    for(int i = 0; i < input_count; i++){
      (void)get_employee_details();
    }
    // clear screen for affirmation message
    (void)system("cls");
    (void)printf("\n\n(i) Employee details were recorded successfully.");
    (void)getch();
    //view_employee();
    (void)menu();
}

void navigate_menu(){
    bool choice_is_invalid = true;
    // only break loop when intended
    while(choice_is_invalid){
      switch((char)getch()){
          case '1':
              (void)add_employee();    
              break;
          case '2':
              (void)view_employee();    
              break;
          case '3':
              //(void)update_employee();    
              break;
          case '4':
              //(void)remove_employee();    
              break;
          case '5':
          case 27:
              (void)printf("\n\n(!) Exiting Program ...");
              (void)exit(0);
              break;
          default:
              continue;
      }
    }
}

void menu(){
    // load/reload necessary files
    (void)load_files();
    // main menu proper
    (void)system("cls");
    (void)printf("\n:: Employee Record System ::\n\n");
    (void)printf("\n1 :: Create Record/s");
    (void)printf("\n2 :: Review Record/s");
    (void)printf("\n3 :: Update Record/s");
    (void)printf("\n4 :: Delete Record/s");
    (void)printf("\n5 :: Exit Program");
    // function for keypress
    (void)navigate_menu();
}

void get_string_input(char * buffer, const int buffer_size){
    // fgets shenanigans
    (void)fflush(stdin);
    (void)fgets(buffer, buffer_size, stdin);
    int len = (int)strlen(buffer);
    buffer[len - 1] = '\0';
}

void put_temp_to_dir(){
    FILE * fdir = (FILE *)fopen("./files/records.csv", "w");
    FILE * ftmp = (FILE *)fopen("./files/temp.csv", "r");

    char buff[max];
    // place back content of temp file to original file
    while((char *)fgets(buff, max, ftmp) != NULL){
        (void)fprintf(fdir, buff);
    }

    (void)fclose(ftmp);
    (void)fclose(fdir);
}

void put_dir_to_temp(int skip_line){
    FILE * fdir = (FILE *)fopen("./files/records.csv", "r");
    FILE * ftmp = (FILE *)fopen("./files/temp.csv", "w");

    char buff[max];
    int line_number = 0;
    // transfer records to a temporary file, skipping line of matched id
    while((char *)fgets(buff, max, fdir) != NULL){
        if(line_number == skip_line){
            continue;
        }
        (void)fprintf(ftmp, buff);
    }

    (void)fclose(ftmp);
    (void)fclose(fdir);
}

void remove_from_file(char * employee_id){
    // check first for an id match
    int id_location = (int)id_exists(employee_id);

    // error catch when id was not found
    if(id_location == 0){
        (void)printf("\n\n(!) Error: Employee ID was nonexistent.");
        (void)getchar();
        (void)exit(1);
    }

    // file removal SOP
    (void)put_dir_to_temp(id_location);
    (void)put_temp_to_dir();
    (void)remove("./files/temp.csv");
}

int id_exists(char * input_id){
    // read file and navigate
    FILE * ffind = (FILE *)fopen("records.csv", "r");

    char buff[max];
    int line_number = 0;
    bool line_is_header = true;

    while((char *)fgets(buff, max, ffind) != NULL){
        if(line_is_header){
            line_is_header = false;
            continue;
        }
        line_number++;
        int len = (int)strlen(buff);
        buff[len - 1] = '\0';
        char id_buff[max] = "";
        for(int i = 0; i < len; i++){
            if(buff[i] == ','){
                break;
            }
            id_buff[i] = buff[i];
        }
        // when there's a match:
        if((int)strcmp(input_id, id_buff) == 0){
            return line_number;
        }
    }
    (void)fclose(ffind);
    // when there's no match:
    return 0;
}

void put_to_file(employee details){
    // append struct to file
    FILE * frecords = (FILE *)fopen("records.csv", "a");

    (void)fprintf(frecords, "%s,", details.id);
    (void)fprintf(frecords, "%s,", details.name);
    (void)fprintf(frecords, "%s,", details.position);
    (void)fprintf(frecords, "%s,", details.department);
    (void)fprintf(frecords, "%s\n", details.gender);

    (void)fclose(frecords);
}

bool file_exists(){
  FILE * fcheck = (FILE *)fopen("records.csv", "r");
  if(fcheck == NULL){
    (void)fclose(fcheck);
    return false;
  } else {
    (void)fclose(fcheck);
    return true;
  }
}

void load_files(){
    // check if necessary files exist
    // if nonexistent, write one
    if((bool)file_exists() == false){
        FILE * fcreate = (FILE *)fopen("records.csv", "w");
        // include a header line
        (void)fprintf(fcreate, "ID,Name,Position,Department,Gender\n");
        (void)fclose(fcreate);
    }
}