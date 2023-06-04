#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define max 64

typedef struct {
    char id[max];
    char name[max];
    char position[max];
    char department[max];
    char gender[max];
} employee;

// tool functions
void load_files();
bool file_exists();
void put_to_file(employee details);
int id_exists(char * input_id);
void remove_from_file(employee details);
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
void read_employee_pos(char * buff, const int size);
void read_employee_dept(char * buff, const int size);
void read_employee_gender(char * buff, const int size);

void view_employee();
void update_employee();
void remove_employee();

int main(){
    (void)menu();
}

void read_employee_gender(char * buff, const int size){
  (void)printf("\nEnter Employee Gender: ");
  (void)get_string_input(buff, size);
}

void read_employee_dept(char * buff, const int size){
  (void)printf("\nEnter Employee Position: ");
  (void)get_string_input(buff, size);
}

void read_employee_pos(char * buff, const int size){
  (void)printf("\nEnter Employee Position: ");
  (void)get_string_input(buff, size);
}

void read_employee_lname(char * buff, const int size){
  (void)printf("\nEnter Employee Last Name: ");
  (void)get_string_input(buff, size);
}

void read_employee_fname(char * buff, const int size){
  (void)printf("\nEnter Employee First Name: ");
  (void)get_string_input(buff, size);
}

void read_employee_id(char * buff, const int size){
  (void)printf("\nEnter Employee ID: ");
  (void)get_string_input(buff, size);
}

void get_employee_details(){
  (void)system("cls");
  // write input to local variables
  char employee_id[max];
  (void)read_employee_id(employee_id, max);
  char employee_fname[max];
  if(id_exists(employee_id)){
    printf("\n\n(!) Employee ID already exists.");
    if((int)getch() == 27){
      (void)menu();
    } else {
      (void)get_employee_details();
    }
  }
  (void)read_employee_fname(employee_fname, max);
  char employee_lname[max];
  (void)read_employee_lname(employee_lname, max);
  char employee_pos[max];
  (void)read_employee_pos(employee_pos, max);
  char employee_dept[max];
  (void)read_employee_dept(employee_dept, max);
  char employee_gender[max];
  (void)read_employee_gender(employee_gender, max);
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
    printf("\n(i) Note : You can only add 10 employees at a time.\n");
    printf("\nHow many employees would you like to enter? : ");
    // get input in string format
    char str_input_count[max];
    get_string_input(str_input_count, max);
    int len = strlen(str_input_count);
    // check first for any invalid character before converting to int
    if(invalid_character_exists(str_input_count, len)){
      printf("\n\n(!) Error : Invalid input.");
      if(getch() == 27){
        menu();
      } else {
        add_employee();
      }
    }
    // convert to int
    int input_count = atoi(str_input_count);
    // take only up to 10 inputs
    if(input_count > 10){
      printf("\n\n(!) You can only record 10 employees at a time.");
      if(getch() == 27){
        menu();
      } else {
      add_employee();
      }
    }
    // when all is good:
    return input_count;
}

void add_employee(){
    // read how many inputs are needed (10 max)
    int input_count = get_input_count();
    // then call this function iteratively
    for(int i = 0; i < input_count; i++){

    }
    // clear screen for affirmation message
    system("cls");
    printf("\n\n(i) Employee details were recorded successfully.");
    getch();
    //view_employee();
    menu();
}

void navigate_menu(){
    bool choice_is_invalid = true;
    // only break loop when intended
    while(choice_is_invalid){
      printf("\n\nYour choice: ");
      switch(getch()){
          case '1':
              add_employee();    
              break;
          case '2':
              view_employee();    
              break;
          case '3':
              update_employee();    
              break;
          case '4':
              remove_employee();    
              break;
          case '5':
          case 27:
              printf("\n\n(!) Exiting Program ...");
              exit(0);
              break;
          default:
              continue;
      }
    }

}

void menu(){
    // load/reload necessary files
    load_files();
    
    system("cls");
    printf("\n:: Employee Record System ::\n\n");

    printf("\n1 :: Add Employee/s");
    printf("\n2 :: View Employee/s");
    printf("\n3 :: Update Employee/s");
    printf("\n4 :: Remove Employee/s");
    printf("\n5 :: Exit Program\n");

    navigate_menu();
}

void get_string_input(char * buffer, const int buffer_size){
    // fgets shenanigans
    fflush(stdin);
    fgets(buffer, buffer_size, stdin);
    int len = strlen(buffer);
    buffer[len - 1] = '\0';
}

void put_temp_to_dir(){
    FILE * fdir = fopen("./files/records.csv", "w");
    FILE * ftmp = fopen("./files/temp.csv", "r");

    char buff[max];
    // place back content of temp file to original file
    while(fgets(buff, max, ftmp)){
        fprintf(fdir, buff);
    }

    fclose(ftmp);
    fclose(fdir);
}

void put_dir_to_temp(int skip_line){
    FILE * fdir = fopen("./files/records.csv", "r");
    FILE * ftmp = fopen("./files/temp.csv", "w");

    char buff[max];
    int line_number = 0;
    // transfer records to a temporary file, skipping line of matched id
    while(fgets(buff, max, fdir)){
        if(line_number == skip_line){
            continue;
        }
        fprintf(ftmp, buff);
    }

    fclose(ftmp);
    fclose(fdir);
}

void remove_from_file(employee details){
    // check first for an id match
    int id_location = id_exists(details.id);

    // error catch when id was not found
    if(id_location == 0){
        printf("\n\n(!) Error: Employee ID was nonexistent.");
        getchar();
        exit(1);
    }

    // file removal SOP
    put_dir_to_temp(id_location);
    put_temp_to_dir();
    remove("./files/temp.csv");
}

int id_exists(char * input_id){
    // read file and navigate
    FILE * ffind = fopen("./files/records.csv", "r");

    char buff[max];
    int line_number = 0;
    bool line_is_header = true;

    while(fgets(buff, max, ffind)){
        if(line_is_header){
            line_is_header = false;
            continue;
        }
        line_number++;
        int len = strlen(buff);
        buff[len - 1] = '\0';
        char id_buff[max] = "";
        for(int i = 0; i < len; i++){
            if(buff[i] == ','){
                break;
            }
            id_buff[i] = buff[i];
        }
        // when there's a match:
        if(strcmp(input_id, id_buff) == 0){
            return line_number;
        }
    }
    fclose(ffind);
    // when there's no match:
    return 0;
}

void put_to_file(employee details){
    // append struct to file
    FILE * frecords = fopen("./files/records.csv", "a");

    fprintf(frecords, "%s,", details.id);
    fprintf(frecords, "%s,", details.name);
    fprintf(frecords, "%s,", details.position);
    fprintf(frecords, "%s,", details.department);
    fprintf(frecords, "%s\n", details.gender);

    fclose(frecords);
}

bool file_exists(){
  FILE * fcheck = fopen("./files/records.csv", "r");
  if(fcheck == NULL){
    fclose(fcheck);
    return false;
  } else {
    fclose(fcheck);
    return true;
  }
}

void load_files(){
    // check if necessary files exist
    // if nonexistent, write one
    if(file_exists() == false){
        FILE * fcreate = fopen("./files/records.csv", "w");
        // include a header line
        fprintf(fcreate, "ID,Name,Position,Department,Gender\n");
        fclose(fcreate);
    }
}