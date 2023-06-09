#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define max 240

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

// main functions
void menu();
void navigate_menu();

void add_employee();
int ae_get_input_count();
void ae_get_employee_details();
void ae_read_employee_id(char * buff, const int size);
bool ae_id_is_invalid(char * buff, const int size);
void ae_read_employee_fname(char * buff, const int size);
void ae_read_employee_lname(char * buff, const int size);
void ae_read_employee_dept(char * buff);
void ae_read_employee_pos(char * buff);
void ae_read_employee_gender(char * buff);

void view_employee();
void ve_navigate_options();
void ve_write_table_header();
void ve_set_table_dimensions();
void ve_draw_border(int length);
void ve_add_space(const int width, int deduct);
int ve_retrieve_from_file();
void ve_clean_buffer(char * buffer, int size);
void ve_clean_struct_members(employee * details);
void ve_print_line(employee details);

void update_employee();
void ue_get_id(char * id_buff, int id_buff_size);
void ue_extract_line(int which_line, employee * details);
void ue_line_to_struct(char * buff, int size, employee * details);
void ue_show_details(employee details);
void ue_navigate_sd(employee details);
void ue_update_id(employee details);
void ue_get_new_id(char * buff, int size);
void ue_update_name(employee details);
void ue_get_new_fname(char * buff, int size);
void ue_get_new_lname(char * buff, int size);
void ue_update_dept(employee details);
void ue_get_new_dept(char * buff, int size);
void ue_nav_new_dept(char * buff, int size);
void ue_update_pos(employee details);
void ue_get_new_pos(char * buff, int size);
void ue_nav_new_pos(char * buff, int size);
void ue_update_gender(employee details);
void ue_get_new_gender(char * buff, int size);
void ue_nav_new_gender(char * buff, int size);

void remove_employee();

// stack is used here:
int st_get_record_last_line(char * buff, int size);
int st_get_trash_last_line(char * buff, int size);
void st_extract_id_from_line(char * buff, char * data_line);
void st_put_line_to_file(char * line);
void st_pop_last_line_record();
void st_push_last_line_trash();
void st_remove_line_from_trash(char * line);

int main(){
  (void)menu();
}

void st_remove_line_from_trash(char * line){
    FILE * count_line_file = (FILE *)fopen("trash.csv", "r");
    
    char buff[max];
    int line_found = 0;

    while(fgets(buff, max, count_line_file)){
        line_found++;
        if(strcmp(line, buff) == 0){
            break;
        }
    }

    fclose(count_line_file);

    FILE * trash_file = (FILE *)fopen("trash.csv", "r");
    FILE * temp_file = (FILE *)fopen("temp.csv", "w");

    ve_clean_buffer(buff, max);

    int line_count = 0;

    while(fgets(buff, max, trash_file)){
        line_count++;
        if(line_count == line_found){
            continue;
        }
        fprintf(temp_file, buff);
    }

    fclose(temp_file);
    fclose(trash_file);

    trash_file = (FILE *)fopen("trash.csv", "w");
    temp_file = (FILE *)fopen("temp.csv", "r");

    ve_clean_buffer(buff, max);

    while(fgets(buff, max, temp_file)){
        fprintf(trash_file, buff);
    }

    fclose(temp_file);
    fclose(trash_file);

    remove("temp.csv");
}

void st_push_last_line_trash(){
    char line_buff[max];
    int last_line = st_get_trash_last_line(line_buff, max);

    // stack should only hold 50

    if(last_line > 50){
        printf("\n\n\n\t(!) Stack Overflow.");
        getch();
        view_employee();
    } else if(last_line < 1){
        printf("\n\n\n\t(!) Recent is empty. Nothing to push here.");
        getch();
        view_employee();
    }

    st_put_line_to_file(line_buff);

    st_remove_line_from_trash(line_buff);

    (void)view_employee();
}

void st_pop_last_line_record(){
    char line_buff[max];
    int last_line = st_get_record_last_line(line_buff, max);

    if(last_line < 1){
        printf("\n\n\n\t(!) Stack Underflow");
        getch();
        view_employee();
    }

    char id_buff[max];
    (void)st_extract_id_from_line(id_buff, line_buff);
    (void)remove_from_file(id_buff);

    (void)view_employee();
}

void st_put_line_to_file(char * line){
    // check first if there is identical ID
    char id[max];
    (void)st_extract_id_from_line(id, line);
    if((int)id_exists(id) != 0){
        printf("(!)");
        getch();
        return;
    }

    FILE * append_file = (FILE *)fopen("records.csv", "a");
    (void)fprintf(append_file, line);
    (void)fclose(append_file);
}

void st_extract_id_from_line(char * buff, char * data_line){
    int len = (int)strlen(data_line);

    for(int i = 0; i < len; i++){
        if(data_line[i] == ','){
            buff[i] = '\0';
            break;
        }
        buff[i] = data_line[i];
    }
}

int st_get_trash_last_line(char * buff, int size){
    char buffer[max];

    FILE * line_count_file = (FILE *)fopen("trash.csv", "r");

    if(line_count_file == NULL){
        fclose(line_count_file);
        return 0;
    }

    int last_line = 0;

    while(fgets(buffer, max, line_count_file)){
        last_line++;
    }

    fclose(line_count_file);

    ve_clean_buffer(buffer, max);

    FILE * ftake = (FILE *)fopen("trash.csv", "r");

    int line_count = 0;

    while((char *)fgets(buffer, max, ftake) != NULL){
        line_count++;
        if(line_count == last_line){
            (void)strcpy(buff, buffer);
        }
    }

    (void)fclose(ftake);
    return last_line;
}

int st_get_record_last_line(char * buff, int size){
    char buffer[max];

    int last_line = -1;

    FILE * count_file = fopen("records.csv", "r");

    while(fgets(buffer, max, count_file)){
        last_line++;
    }

    ve_clean_buffer(buffer, max);

    int line_count = -1;

    FILE * ftake = (FILE *)fopen("records.csv", "r");

    while((char *)fgets(buffer, max, ftake) != NULL){
        line_count++;
        if(line_count == last_line){
            (void)strcpy(buff, buffer);
        }
    }
    (void)fclose(ftake);
    return last_line;
}

void remove_employee(){
    char s[max];
    int count = st_get_record_last_line(s, max);
  
    if(count < 1){
        system("cls");
        printf("\n(!) No records found. Would you like to add ? [Y/n]");
        char c = getch();
        if(c == 'y' || c == 'Y' || c == 13){
            add_employee();
        } else {
            menu();
        }
    } 

    (void)system("cls");
    (void)printf("\nThis option will remove the employee from the record.\n");
    (void)printf("\nDo you wish to continue ? [Y/n]\n");

    bool invalid = true;
    while(invalid){
        switch((char)getch()){
            case 'y':
            case 'Y':
            case 13:
                invalid = false;
                break;
            case 'n':
            case 'N':
            case 27:
                menu();
                break;
            default:
                continue;
        }
    }

    char id[max];
    (void)ue_get_id(id, max);
    (void)remove_from_file(id);

    (void)system("cls");
    (void)printf("\n(-) Employee removed successfully.");
    (void)getch();
    (void)view_employee();
}

void ue_nav_new_gender(char * buff, int size){
  for(int i = 0; i < 10; i++){
    switch((char)getch()){
      case '1':
        strcpy(buff, "Male");
        break;
      case '2':
        strcpy(buff, "Female");
        break;
      case 27:
        (void)menu();
      default:
        continue;
    }
    return;
  }
}

void ue_get_new_gender(char * buff, int size){
  (void)system("cls");
  (void)printf("\n(i) Notice : Choose Position to update to.\n");
  (void)printf("\n\t1 :: Male");
  (void)printf("\n\t2 :: Female");
  (void)printf("\n(i) Notice : Press ESC to cancel.\n");
  (void)ue_nav_new_gender(buff, size);
}

void ue_update_gender(employee details){
  (void)remove_from_file(details.id);
  (void)ve_clean_buffer(details.gender, max);

  (void)ue_get_new_pos(details.gender, max);
  (void)put_to_file(details);

  (void)printf("\n\n(i) Notice : Gender info was successfully updated.");
  (void)getch();
  (void)view_employee(); 
}

void ue_nav_new_pos(char * buff, int size){
  for(int i = 0; i < 10; i++){
    switch((char)getch()){
      case '1':
        strcpy(buff, "Dept. Head");
        break;
      case '2':
        strcpy(buff, "Dept. Deputy Head");
        break;
      case '3':
        strcpy(buff, "Manager");
        break;
      case '4':
        strcpy(buff, "Asst. Manager");
        break;
      case '5':
        strcpy(buff, "Team Leader");
        break;
      case '6':
        strcpy(buff, "Staff");
        break;
      case 27:
        (void)menu();
      default:
        continue;
    }
    return;
  }
  // when return is not triggered
  (void)printf("\n\n(!) Notice : Too many invalid attempts. Taking you back to menu ...");
  (void)getch();
  (void)menu();
}

void ue_get_new_pos(char * buff, int size){
  (void)system("cls");
  (void)printf("\n(i) Notice : Choose Position to update to.\n");
  (void)printf("\n\t1 :: Dept. Head");
  (void)printf("\n\t2 :: Dept. Deputy Head");
  (void)printf("\n\t3 :: Manager");
  (void)printf("\n\t4 :: Asst. Manager");
  (void)printf("\n\t5 :: Team Leader\n");
  (void)printf("\n\t6 :: Staff\n");
  (void)printf("\n(i) Notice : Press ESC to cancel.\n");
  (void)ue_nav_new_pos(buff, size);
}

void ue_update_pos(employee details){
  (void)remove_from_file(details.id);
  (void)ve_clean_buffer(details.position, max);

  (void)ue_get_new_pos(details.position, max);
  (void)put_to_file(details);

  (void)printf("\n\n(i) Notice : Position info was successfully updated.");
  (void)getch();
  (void)view_employee(); 
}

void ue_nav_new_dept(char * buff, int size){
  for(int i = 0; i < 10; i++){
    switch((char)getch()){
      case '1':
        strcpy(buff, "General");
        break;
      case '2':
        strcpy(buff, "Marketing");
        break;
      case '3':
        strcpy(buff, "Engineering");
        break;
      case '4':
        strcpy(buff, "Design");
        break;
      case '5':
        strcpy(buff, "Logistics");
        break;
      case 27:
        (void)menu();
      default:
        continue;
    }
    return;
  }
  // when return is not triggered
  (void)printf("\n\n(!) Notice : Too many invalid attempts. Taking you back to menu ...");
  (void)getch();
  (void)menu();
}

void ue_get_new_dept(char * buff, int size){
  (void)system("cls");
  (void)printf("\n(i) Notice : Choose Department to update to.\n");
  (void)printf("\n\t1 :: General");
  (void)printf("\n\t2 :: Marketing");
  (void)printf("\n\t3 :: Engineering");
  (void)printf("\n\t4 :: Design");
  (void)printf("\n\t5 :: Logistics\n");
  (void)printf("\n(i) Notice : Press ESC to cancel.\n");
  (void)ue_nav_new_dept(buff, size);
}

void ue_update_dept(employee details){
  (void)remove_from_file(details.id);
  (void)ve_clean_buffer(details.department, max);

  (void)ue_get_new_dept(details.department, max);
  (void)put_to_file(details);

  (void)printf("\n\n(i) Notice : Department info was successfully updated.");
  (void)getch();
  (void)view_employee(); 
}

void ue_get_new_lname(char * buff, int size){
  // give 10 chances
  for(int i = 0; i < 10; i++){
    (void)system("cls");
    // get new first name
    (void)printf("\nEnter New Last Name : ");
    (void)get_string_input(buff, size);
    // check name length
    if((int)strlen(buff) > 16){
      (void)printf("\n\n(!) Notice : You could only input up to 16 characters.");
      if((int)getch() == 27){
        (void)menu();
      } else {
        continue;
      }
    } else {
      // if checks passed
      return;
    }
  }
}

void ue_get_new_fname(char * buff, int size){
  // give 10 chances
  for(int i = 0; i < 10; i++){
    (void)system("cls");
    // get new first name
    (void)printf("\nEnter New First Name : ");
    (void)get_string_input(buff, size);
    // check name length
    if((int)strlen(buff) > 16){
      (void)printf("\n\n(!) Notice : You could only input up to 16 characters.");
      if((int)getch() == 27){
        (void)menu();
      } else {
        continue;
      }
    } else {
      // if checks passed
      return;
    }
  }
  // if return was not triggered
  (void)printf("(!) Notice : Too many invalid attempts. Returning to menu ...");
  (void)getch();
  (void)menu();
}

void ue_update_name(employee details){
  (void)remove_from_file(details.id);
  (void)ve_clean_buffer(details.name, max);

  char fname[max];
  (void)ue_get_new_fname(fname, max);

  char lname[max];
  (void)ue_get_new_lname(lname, max);

  (void)strcpy(details.name, fname);
  (void)strcat(details.name, " ");
  (void)strcat(details.name, lname);

  (void)put_to_file(details);

  (void)printf("\n\n(i) Notice : Name info was successfully updated.");
  (void)getch();
  (void)view_employee(); 
}

void ue_get_new_id(char * buff, int size){
  // give 10 chances
  for(int i = 0; i < 10; i++){
    (void)system("cls");
    // get new id
    (void)printf("\nEnter new ID : ");
    (void)get_string_input(buff, size);
    // check new id
    if((bool)id_exists(buff)){
      (void)printf("\n\n(!) Notice : ID already exists.");
      if((int)getch() == 27){
        (void)menu();
      }
      continue;
    } else if ((bool)ae_id_is_invalid(buff, (int)strlen(buff))){
      (void)printf("\n\n(!) Notice : ID is invalid.");
      if((int)getch() == 27){
        (void)menu();
      }

    } else {
      // if checks passed
      return;
    }
  }
  // if return was not triggered
  (void)printf("(!) Notice : Too many invalid attempts. Returning to menu ...");
  (void)getch();
  (void)menu();
}

void ue_update_id(employee details){
  (void)remove_from_file(details.id);
  (void)ve_clean_buffer(details.id, max);

  (void)ue_get_new_id(details.id, max);
  (void)put_to_file(details);

  (void)printf("\n\n(i) Notice : ID info was successfully updated.");
  (void)getch();
  (void)view_employee();
}

void ue_navigate_sd(employee details){
  bool invalid = true;
  for(int i = 0; i < 10; i++){
    switch((char)getch()){
      case '1':
        invalid = false;
        (void)ue_update_id(details);
        break;
      case '2':
        invalid = false;
        (void)ue_update_name(details);
        break;
      case '3':
        invalid = false;
        (void)ue_update_dept(details);
        break;
      case '4':
        invalid = false;
        break;
      case '5':
        invalid = false;
        break;
      case '6':
      case 27:
        invalid = false;
        (void)menu();
        break;
      default:
        continue;
    }
  }
  if(invalid){
    (void)printf("(!) Too many invalid attempts. Returning to menu ...");
    (void)getch();
    (void)menu();
  }
}

void ue_show_details(employee details){
  (void)system("cls");
  (void)printf("\n(i) Select which field you wish to edit.\n");
  (void)printf("\n[1] Employee ID : %s", details.id);
  (void)printf("\n[2] Employee Name : %s", details.name);
  (void)printf("\n[3] Employee Department : %s", details.department);
  (void)printf("\n[4] Employee Position : %s", details.position);
  (void)printf("\n[5] Employee Gender : %s\n", details.gender);
  (void)printf("\n[6] Cancel");
}

void ue_line_to_struct(char * buff, int size, employee * details){
  buff[size - 1] = '\0';
  int comma = 0;
  int str_i = 0;
  for(int i = 0; i < size; i++){
    if(buff[i] == ','){
      comma++;
      str_i = 0;
      continue;
    }
    switch(comma){
      case 0:
        details->id[str_i] = buff[i];
        str_i++;
        break;
      case 1:
        details->name[str_i] = buff[i];
        str_i++;
        break;
      case 2:
        details->department[str_i] = buff[i];
        str_i++;
        break;
      case 3:
        details->position[str_i] = buff[i];
        str_i++;
        break;
      case 4:
        details->gender[str_i] = buff[i];
        str_i++;
        break;
      default:
        (void)printf("\n\n(!) Unexpected ',' character at file. Exiting program ...");
        (void)getch();
        (void)exit(1);
    }
  }
}

void ue_extract_line(int which_line, employee * details){
  // get line from file
  char buff[max];
  int line_number = 0;
  bool line_is_header = true;
  FILE * fxtr = (FILE *)fopen("records.csv", "r");
  while((char *)fgets(buff, max, fxtr) != NULL){
    if(line_is_header){
      line_is_header = false;
      continue;
    }
    line_number++;
    if(line_number >= 50){
      (void)printf("\n\n(!) Error : File exceeded expected length. Exiting program ...");
      (void)exit(1);
    } else if (line_number == which_line){
      (void)ue_line_to_struct(buff, (int)strlen(buff), details);
      break;
    }
  }
  (void)fclose(fxtr);
}

void ue_get_id(char * id_buff, int id_buff_size){
  for(int i = 0; i < 10; i++){
    (void)system("cls");
    (void)printf("\n(i) Enter ID of the employe record you wish to edit\n");
    // get id
    (void)printf("\nEnter ID : ");
    (void)get_string_input(id_buff, id_buff_size);
    // check id
    if((bool)ae_id_is_invalid(id_buff, strlen(id_buff))){
      (void)printf("\n\n(!) Warning : ID input invalid.");
      if((int)getch() == 27){
        (void)menu();
      } else {
        continue;
      }
    } else if((bool)id_exists(id_buff) == false){
      (void)printf("\n\n(!) Warning : ID does not exists.");
      if((int)getch() == 27){
        (void)menu();
      } else {
        continue;
      }
    }
    // if it passes checks
    return;
  }
  // if return was not triggered
  (void)printf("\n\n(!) Notice : Too many invalid attempts. Returning to menu ...");
  (void)getch();
  (void)menu();
}

void update_employee(){
  char s[max];
  int count = st_get_record_last_line(s, max);
  
  if(count < 1){
    system("cls");
    printf("\n(!) No records found. Would you like to add ? [Y/n]");
    char c = getch();
    if(c == 'y' || c == 'Y' || c == 13){
        add_employee();
    } else {
        menu();
    }
  }

  // grab id
  char id[max];
  (void)ue_get_id(id, max);

  // find id in file
  int id_location = (int)id_exists(id);

  // struct to store record details
  employee to_edit;

  // clean struct members first
  (void)ve_clean_struct_members(&to_edit);
  (void)ue_extract_line(id_location, &to_edit);

  // show details of record to be edited
  (void)ue_show_details(to_edit);

  // navigate update menu
  (void)ue_navigate_sd(to_edit);
}

void ve_print_line(employee details){
  (void)printf("\n\t");

  (void)printf("%s", details.id);
  (void)ve_add_space(table.id_w, strlen(details.id));
  
  (void)printf("%s", details.name);
  (void)ve_add_space(table.name_w, strlen(details.name));
  
  (void)printf("%s", details.department);
  (void)ve_add_space(table.dept_w, strlen(details.department));
  
  (void)printf("%s", details.position);
  (void)ve_add_space(table.pos_w, strlen(details.position));
  
  (void)printf("%s", details.gender);
  (void)ve_add_space(table.gender_w, strlen(details.gender));
}

void ve_clean_struct_members(employee * details){
  // set default size to max
  ve_clean_buffer(details->id, max);
  ve_clean_buffer(details->name, max);
  ve_clean_buffer(details->department, max);
  ve_clean_buffer(details->position, max);
  ve_clean_buffer(details->gender, max);
}

void ve_clean_buffer(char * buffer, int size){
  for(int i = 0; i < size; i++){
    buffer[i] = '\0';
  }
}

int ve_retrieve_from_file(){
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
    // temporarily store in struct   
    employee current_employee;
    // clean struct members
    (void)ve_clean_struct_members(&current_employee);
    
    (void)ue_line_to_struct(buff, (int)strlen(buff), &current_employee);

    (void)ve_print_line(current_employee);
  } 
  (void)fclose(fret);
  return line_count;
}

void ve_add_space(const int width, int deduct){
  for(int i = 0; i < (width - deduct); i++){
    (void)printf(" ");
  }
}

void ve_draw_border(int length){
  for(int i = 0; i < length; i++){
    (void)printf("-");
  }
}

void ve_set_table_dimensions(){
  // set values for global struct table
  table.id_w = 10;
  table.name_w = 32;
  table.dept_w = 20;
  table.pos_w = 25;
  table.gender_w = 10;
  table.table_w = (table.id_w + table.name_w + table.dept_w + table.pos_w + table.gender_w);
}

void ve_write_table_header(){
  // dimensions
  (void)ve_set_table_dimensions();
  // top border
  (void)printf("\n\n\t");
  (void)ve_draw_border(table.table_w);
  (void)printf("\n\t");

  // print headers, add space to achieve desired width
  char header_1[max] = "ID";
  (void)printf("%s", header_1);
  (void)ve_add_space(table.id_w, (int)strlen(header_1));
  
  char header_2[max] = "Name";
  (void)printf("%s", header_2);
  (void)ve_add_space(table.name_w, (int)strlen(header_2));
  
  char header_3[max] = "Department";
  (void)printf("%s", header_3);
  (void)ve_add_space(table.dept_w, (int)strlen(header_3));

  char header_4[max] = "Position";
  (void)printf("%s", header_4);
  (void)ve_add_space(table.pos_w, (int)strlen(header_4));

  char header_5[max] = "Gender";
  (void)printf("%s", header_5);
  (void)ve_add_space(table.gender_w, (int)strlen(header_5));

  // add bottom border for header
  (void)printf("\n\t");
  (void)ve_draw_border(table.table_w);
}

void ve_navigate_options(){
    bool key_invalid = true;
    while(key_invalid){
        switch(getch()){
            case 'z':
                (void)st_pop_last_line_record();
                break;
            case 'x':
                (void)st_push_last_line_trash();
                break;
            case 'c':
                (void)add_employee();
                break;
            case 27:
            case 13:
                (void)menu();
                break;
            default:
                continue;
        }
    }
}

void view_employee(){
  char s[max];
  int count = st_get_trash_last_line(s, max);

  if(count < 1){
    remove("trash.csv");
  }

  // this table will be of static size
  (void)system("cls");
  (void)ve_write_table_header();

  // total retrieved lines from file, prints them as well
  int ret_lines = (int)ve_retrieve_from_file();

  // draw bottom border
  (void)printf("\n\n\t");
  (void)ve_draw_border(table.table_w);

  // show total records
  (void)printf("\n\n\tTotal Records: %d", ret_lines);

  if(ret_lines == 0){
    printf("\n\n\t(!) Stack is EMPTY.");
  } else if(ret_lines == 50){
    printf("\n\n\t(!) Stack is FULL.");
  }

  (void)printf("\n\n\n");

  (void)printf("\t[z] : Pop");
  (void)printf("\t\t[x] : Push Recent");
  (void)printf("\t\t[c] : Push New");
  (void)printf("\t\t[ESC] : Exit");

  (void)ve_navigate_options();
}

void ae_read_employee_gender(char * buff){
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

void ae_read_employee_pos(char * buff){
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

void ae_read_employee_dept(char * buff){
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

void ae_read_employee_lname(char * buff, const int size){
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

void ae_read_employee_fname(char * buff, const int size){
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

bool ae_id_is_invalid(char * buff, const int size){
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

void ae_read_employee_id(char * buff, const int size){
  // ten invalid attempts and the program quits to menu
  for(int i = 0; i < 10; i++){
    (void)system("cls");
    (void)printf("\n(i) Valid ID is written as such : \"123-123\"\n");
    (void)printf("\nEnter Employee ID: ");
    (void)get_string_input(buff, size);
    
    // id should only have numbers and dashes
    int len = (int)strlen(buff);

    if((bool)ae_id_is_invalid(buff, len)){
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

    ve_clean_buffer(buff, max);
  }

  (void)printf("\n\n(!) Too many invalid inputs. Returning to menu.");
  (void)getch();
  (void)menu();
}

void ae_get_employee_details(){
  (void)system("cls");

  // write input to local variables
  char employee_id[max];
  (void)ae_read_employee_id(employee_id, max);

  char employee_fname[max];
  (void)ae_read_employee_fname(employee_fname, max);

  char employee_lname[max];
  (void)ae_read_employee_lname(employee_lname, max);

  char employee_dept[max];
  (void)ae_read_employee_dept(employee_dept);

  char employee_pos[max];
  (void)ae_read_employee_pos(employee_pos);

  char employee_gender[max];
  (void)ae_read_employee_gender(employee_gender);

  // then clone to struct
  employee new_employee;
  (void)strcpy(new_employee.id, employee_id);
  (void)strcpy(new_employee.name, employee_fname);
  (void)strcat(new_employee.name, " ");
  (void)strcat(new_employee.name, employee_lname);
  (void)strcpy(new_employee.department, employee_dept);
  (void)strcpy(new_employee.position, employee_pos);
  (void)strcpy(new_employee.gender, employee_gender);

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

int ae_get_input_count(){
    system("cls");
    (void)printf("\n(i) Note : You can only add 10 employees at a time.\n");
    (void)printf("\n(i) Note : You can enter \"0\" to return to menu.\n");
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

    if(input_count > 10){
      (void)printf("\n\n(!) You can only record up to 10 employees at a time.");
      if((int)getch() == 27){
        (void)menu();
      } else {
        (void)add_employee();
      }
    }

    return input_count;
}

void add_employee(){
    FILE * count_file = fopen("records.csv", "r");

    char s[max];
    int count = 0;

    while(fgets(s, max, count_file)){
        count++;
    }

    fclose(count_file);

    if(count >= 50){
        system("cls");
        printf("(!) Employee records full. You can only store 50 records at a time.");
        getch();
        menu();
    }

    // read how many inputs are needed (10 max)
    int input_count = (int)ae_get_input_count();

    // if count was 0, cancel operation
    if(input_count == 0){
        menu();
    }

    // then call this function iteratively
    for(int i = 0; i < input_count; i++){
      (void)ae_get_employee_details();
    }

    // clear screen for affirmation message
    (void)system("cls");
    (void)printf("\n\n(i) Employee details were recorded successfully.");
    (void)getch();
    (void)view_employee();
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
              (void)update_employee();    
              break;
          case '4':
              (void)remove_employee();    
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
    FILE * fdir = (FILE *)fopen("records.csv", "w");
    FILE * ftmp = (FILE *)fopen("temp.csv", "r");

    char buff[max];
    // place back content of temp file to original file
    while((char *)fgets(buff, max, ftmp) != NULL){
        (void)fprintf(fdir, buff);
    }

    (void)fclose(ftmp);
    (void)fclose(fdir);
}

void put_dir_to_temp(int skip_line){
    FILE * fdir = (FILE *)fopen("records.csv", "r");
    FILE * ftmp = (FILE *)fopen("temp.csv", "w");
    FILE * recently_deleted_file = (FILE *)fopen("trash.csv", "a");

    char buff[max];
    int line_number = -1;
    bool header = true;
    // transfer records to a temporary file, skipping line of matched id
    while((char *)fgets(buff, max, fdir) != NULL){
        line_number++;
        if(line_number == skip_line){
            (void)fprintf(recently_deleted_file, buff);
            continue;
        }
        (void)fprintf(ftmp, buff);
    }

    (void)fclose(recently_deleted_file);
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
    (void)remove("temp.csv");
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
    (void)fprintf(frecords, "%s,", details.department);
    (void)fprintf(frecords, "%s,", details.position);
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
        (void)fprintf(fcreate, "ID,Name,Department,Position,Gender\n");
        (void)fclose(fcreate);
    }
}