#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bool.h>

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
void put_to_file(employee details);

int id_exists(char * input_id);

void remove_from_file(employee details);

// stack is used here:

void undo_action();

// main functions

void menu();
void add_employee();
void view_employee();
void update_employee();
void remove_employee();

int main(){

    menu();

}

void add_employee(){

    system("clear");

    // count how many inputs are needed

    char temp_input_count[max];

    printf("\nHow many employees would you like to enter? : ");
    fflush(stdin);
    fgets(temp_input_count, max, stdin);

    int last_string_len = strlen(temp_input_count);

    temp_input_count[last_string_len - 1] = '\0';

    // check first for any character before converting to int

    for(int i = 0; i < last_string_len; i++){
        if(temp_input_count >= 'a'){
            printf("(!) Error: Input should only be unsigned integer.");
        }
    }

    employee new_employee;

}

void menu(){

    system("clear");

    printf("\n:: Employee Record System ::\n\n");

    printf("\n1 :: Add Employee/s");
    printf("\n2 :: View Employee/s");
    printf("\n3 :: Update Employee/s");
    printf("\n4 :: Remove Employee/s");
    printf("\n5 :: Exit Program\n");

    int choice = 0;
    bool choice_is_invalid = true;

    while(choice_is_invalid){
        printf("\n\nYour choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                add_employee();    
                break;
            case 2:
                view_employee();    
                break;
            case 3:
                update_employee();    
                break;
            case 4:
                remove_employee();    
                break;
            case 5:
                printf("\n\n(!) Exiting Program ...");
                exit(0);
                break;
            default:
                continue;
        }
    }
    

}

void remove_from_file(employee details){

    // check first for an id match

    int id_location = id_exists(detail.id);

    // if there's no match:

    if(id_location == 0){
        printf("\n\n(!) Error: Employee ID was nonexistent.");
        getchar();
        exit(1);
    }

    // else:

    FILE * fdir = fopen("./files/records.csv", "r");
    FILE * ftmp = fopen("./files/temp.csv", "w");

    // transfer records to a temporary file, skipping line of matched id

    char buff[max];
    int line_number = 0;
    bool line_is_header = true;

    while(fgets(buff, max, fpdir)){
        if(line_is_header){
            continue;
        }
        if(line_number == id_location){
            continue;
        }
        fprintf(ftmp, buff);
    }

    fclose(ftmp);
    fclose(fdir);

    // then transfer back temporary file's content to original file

    fdir = fopen("./files/records.csv", "w");
    ftmp = fopen("./files/temp.csv", "r");

    fprintf(fdir, "ID,Name,Position,Department,Gender\n");

    while(fgets(buff, max, ftmp)){
        fprintf(fdir, buff);
    }

    fclose(ftmp);
    fclose(fdir);

}

int id_exists(char * input_id){

    // read file and navigate

    FILE * ffind = fopen("./files/records.csv", "r");

    char buff[max];

    bool line_is_header = true;

    int line_number = 0;

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

void load_files(){
    
    // check if necessary files exist

    FILE * fcheck = fopen("./files/records.csv", "r");

    bool file_exists;

    if(fcheck == NULL){
        file_exists = false;
    } else {
        file_exists = true;
    }

    fclose(fcheck);

    // if nonexistent, write one

    if(file_exists == false){
        fcheck = fopen("./files/records.csv", "w");
        // include a header line
        fprintf(fcheck, "ID,Name,Position,Department,Gender\n");
        fclose(fcheck);
    }

}