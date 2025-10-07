#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct User{
    int id;
    char name[50];
    int age;
};

void createFile(){ 
    FILE *file = fopen("users.txt", "a");
    if(file != NULL) fclose(file);
}
int isIdExists(int id){
    struct User userRecord;
    FILE *file = fopen("users.txt", "r");
    if(file != NULL){
        while(fscanf(file, "%d %s %d", &userRecord.id, userRecord.name, &userRecord.age) == 3){
            if(userRecord.id == id){
                fclose(file);
                return 1;
            }
        }
        fclose(file);
    }
    return 0;
}
int isValidName(char name[]){
    for(int i = 0; name[i] != '\0'; i++){
        if(!isalpha(name[i])) return 0;
    }
    return 1;
}
void addUser(){      
    struct User userData;
    printf("Enter id: ");
    if(scanf("%d", &userData.id ) != 1 || userData.id <= 0){
        printf("Invalid ID!\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    if(isIdExists(userData.id)){
        printf("ID already exists! Enter a unique ID.\n");
        return;
    }
    printf("Enter name: ");
    scanf("%s", userData.name);
    if(!isValidName(userData.name)){
        printf("Invalid name! Use letters only.\n");
        return;
    }
    printf("Enter age: ");
    if(scanf("%d", &userData.age) != 1 || userData.age <= 0){
        printf("Invalid age!\n");
        while(getchar() != '\n');
        return;
    }
    FILE *file = fopen("users.txt", "a");
    if(file != NULL){
        fprintf(file, "%d %s %d\n", userData.id, userData.name, userData.age);
        fclose(file);
        printf("Added successfully!\n");
    }
    else {
        printf("Error: Unable to open file for writing.\n");
    }
}

void showUsers() {
    struct User userData;
    FILE *file = fopen("users.txt", "r");
    if(file == NULL){
        printf("Cannot open file\n");
        return;
    }
    printf("\nID\tName\tAge\n");
    while(fscanf(file, "%d %s %d", &userData.id, userData.name, &userData.age) == 3) {
        printf("%d\t%s\t%d\n", userData.id, userData.name, userData.age);
    }
    fclose(file);
}

void updateUser() {
    struct User userData;
    int id, found = 0, newId;
    printf("Enter ID to update: ");
    scanf("%d", &id);
    FILE *file = fopen("users.txt", "r");
    FILE *temporaryFile = fopen("temp.txt", "w");
    if(file == NULL || temporaryFile == NULL){
        printf("Cannot open file\n");
        return;
    }

    while(fscanf(file, "%d %s %d", &userData.id, userData.name, &userData.age) == 3){
        if(userData.id == id) {
            found = 1;
            printf("Enter new ID: ");
            scanf("%d", &newId);
            if(newId != id && isIdExists(newId)){
                printf("ID already exists! Update cancelled.\n");
                fprintf(temporaryFile, "%d %s %d\n", userData.id, userData.name, userData.age);
                continue;
            }
            userData.id = newId;
            printf("Enter new Name: ");
            scanf("%s", userData.name);
            if(!isValidName(userData.name)){
                printf("Invalid name! Update cancelled.\n");
                fprintf(temporaryFile, "%d %s %d\n", userData.id, userData.name, userData.age);
                continue;
            }
            printf("Enter new Age: ");
            scanf("%d", &userData.age);
        }
        fprintf(temporaryFile, "%d %s %d\n", userData.id, userData.name, userData.age);
    }
    fclose(file); fclose(temporaryFile);

    if(found){
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("Updated successfully!\n");
    } 
    else{
        printf("ID not found\n");
        remove("temp.txt");
    }
}

void deleteUser(){
    struct User userData;
    int id, found = 0;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    FILE *file = fopen("users.txt", "r");
    FILE *temporaryFile = fopen("temp.txt", "w");
    if(file == NULL || temporaryFile == NULL){
        printf("Cannot open file\n");
        return;
    }
    while(fscanf(file, "%d %s %d", &userData.id, userData.name, &userData.age) == 3) {
        if(userData.id != id) fprintf(temporaryFile, "%d %s %d\n", userData.id, userData.name, userData.age);
        else found = 1;
    }
    fclose(file); fclose(temporaryFile);
    if(found){
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("Deleted successfully!\n");
    } 
    else{
        printf("ID not found\n");
        remove("temp.txt");
    }
}
void showMenu() {
    int choice;
    while(1) {
        printf("\n1.Add\n2.Show\n3.Update\n4.Delete\n5.Exit\n");
        printf("Choice: "); 
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if(choice == 5) {
            printf("Done\n");
            return;
        }
        switch(choice){
            case 1: addUser(); break;
            case 2: showUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            default: printf("Wrong choice\n");
        }
    }
}

int main() {
    createFile();
    showMenu();
    return 0;
}

