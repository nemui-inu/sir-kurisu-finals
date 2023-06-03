#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bool.h>

#define max 64

typedef struct {

    char id[64];
    char name[64];
    char position[64];
    char department[64];
    char gender[64];

} employee;

// tool functions

void load_files();
void put_to_file(employee details);
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

void load(){
    
}