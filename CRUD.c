#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct user{
    int id;
    char name[50];
    int age;
};

void create(){ 
    FILE *f = fopen("users.txt", "a");
    if(f != NULL) fclose(f);
}
int idcheck(int id){
    struct user u;
    FILE *f = fopen("users.txt", "r");
    if(f != NULL){
        while(fscanf(f, "%d %s %d", &u.id, u.name, &u.age) == 3){
            if(u.id == id){
                fclose(f);
                return 1;
            }
        }
        fclose(f);
    }
    return 0;
}
int namecheck(char n[]){
    for(int i = 0; n[i] != '\0'; i++){
        if(!isalpha(n[i])) return 0;
    }
    return 1;
}
void add(){      
    struct user u, temp;
    int exists = 0;
    printf("Enter id: ");
    if(scanf("%d", &u.id) != 1){
        printf("Invalid!\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    if(idcheck(u.id)){
        printf("ID already exists! Enter a unique ID.\n");
        return;
    }
    printf("Enter name: ");
    scanf("%s", u.name);
    if(!namecheck(u.name)){
        printf("Invalid name! Use letters only.\n");
        return;
    }
    printf("Enter age: ");
    if(scanf("%d", &u.age) != 1 || u.age <= 0){
        printf("Invalid age!\n");
        while(getchar() != '\n');
        return;
    }
    FILE *f = fopen("users.txt", "a");
    if(f != NULL){
        fprintf(f, "%d %s %d\n", u.id, u.name, u.age);
        fclose(f);
        printf("Added successfully!\n");
    }
}

void show() {
    struct user u;
    FILE *f = fopen("users.txt", "r");
    if(f == NULL){
        printf("Cannot open file\n");
        return;
    }
    printf("\nID\tName\tAge\n");
    while(fscanf(f, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        printf("%d\t%s\t%d\n", u.id, u.name, u.age);
    }
    fclose(f);
}

void update() {
    struct user u;
    int id, found = 0, newId;
    printf("Enter ID to update: ");
    scanf("%d", &id);
    FILE *f = fopen("users.txt", "r");
    FILE *t = fopen("temp.txt", "w");
    if(f == NULL || t == NULL){
        printf("Cannot open file\n");
        return;
    }

    while(fscanf(f, "%d %s %d", &u.id, u.name, &u.age) == 3){
        if(u.id == id) {
            found = 1;
            printf("Enter new ID: ");
            scanf("%d", &newId);
            if(newId != id && idcheck(newId)){
                printf("ID already exists! Update cancelled.\n");
                fprintf(t, "%d %s %d\n", u.id, u.name, u.age);
                continue;
            }
            u.id = newId;
            printf("Enter new Name: ");
            scanf("%s", u.name);
            if(!namecheck(u.name)){
                printf("Invalid name! Update cancelled.\n");
                fprintf(t, "%d %s %d\n", u.id, u.name, u.age);
                continue;
            }
            printf("Enter new Age: ");
            scanf("%d", &u.age);
        }
        fprintf(t, "%d %s %d\n", u.id, u.name, u.age);
    }
    fclose(f); fclose(t);

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

void delete(){
    struct user u;
    int id, found = 0;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    FILE *f = fopen("users.txt", "r");
    FILE *t = fopen("temp.txt", "w");
    if(f == NULL || t == NULL){
        printf("Cannot open file\n");
        return;
    }
    while(fscanf(f, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if(u.id != id) fprintf(t, "%d %s %d\n", u.id, u.name, u.age);
        else found = 1;
    }
    fclose(f); fclose(t);
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
void menu() {
    int ch;
    while(1) {
        printf("\n1.Add\n2.Show\n3.Update\n4.Delete\n5.Exit\n");
        printf("Choice: "); 
        scanf("%d", &ch);
        if(ch == 5) {
            printf("Done\n");
            return;
        }
        switch(ch){
            case 1: add(); break;
            case 2: show(); break;
            case 3: update(); break;
            case 4: delete(); break;
            default: printf("Wrong choice\n");
        }
    }
}

int main() {
    create();
    menu();
    return 0;
}
