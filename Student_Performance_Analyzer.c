#include<stdio.h>

struct Student_Record{
    int rollNumber;
    char name[100];
    int marks1;
    int marks2;
    int marks3;
};

int isDuplicateRoll(int roll, struct Student_Record students_data[], int count) {
    
    for (int i = 0; i < count; i++) {
        if (students_data[i].rollNumber == roll)
            return 1;
    }
    return 0;
}

char Grade_Evaluator(float average_score, int *invalid_score) {

    if (average_score < 0 || average_score > 100) {
        *invalid_score = 1;
        return 'X';
    }

    if (average_score >= 85){
        return 'A';
    }
    
    else if (average_score >= 70) {
        return 'B';
    }

    else if (average_score >= 50) {
        return 'C';
    }

    else if (average_score >= 35) {
        return 'D';
    }

    else{
         return 'F';
    }
 
}

int Total_Score(struct Student_Record students_data, int *invalid_score) {

    if (students_data.marks1 < 0 || students_data.marks1 > 100 || 
        students_data.marks2 < 0 || students_data.marks2 > 100 || 
        students_data.marks3< 0 || students_data.marks3 > 100) {
        *invalid_score = 1;
        return -1;
    }

    return students_data.marks1 + students_data.marks2 +students_data.marks3;
}

float Average_Score(int total_score, int *invalid_score) {
    if (total_score < 0 || total_score > 300) {
        *invalid_score = 1;
        return -1;
    }
    return total_score / 3.0;
}

void Performance_Evaluator(char grade, int *invalid_score) {
    int stars=0;

    if (grade == 'X') {
        *invalid_score = 1;
        return ;
    }

    switch(grade) {
        case 'A':
            stars = 5;
            break;
        case 'B':
            stars = 4;
            break;
        case 'C':
            stars = 3;
            break;
        case 'D':
            stars = 2;
            break;
        default:
            stars=0;
    }

    for(int i=0; i<stars; i++){
        printf("*");
    }
    printf("\n");
}

void PrintRollNumber(int index,int N, struct Student_Record students_data[]){

    if(index==N) {
        return;
    }

    printf("%d ",students_data[index].rollNumber);
    PrintRollNumber(index+1,N,students_data);
}

int main(){

    int N;
    printf("Enter number of students: ");

    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("Invalid number of students!\n");
        return 1;
    }

    struct Student_Record students_data[N];

    for(int i=0;i<N;i++){
        printf("Enter Details of Student %d (Roll Number, Name, Marks1, Marks2, Marks3): ", i+1);
        
        if(scanf("%d %s %d %d %d", &students_data[i].rollNumber,students_data[i].name,
            &students_data[i].marks1,&students_data[i].marks2,&students_data[i].marks3)!=5 || 
            students_data[i].rollNumber <= 0 || isDuplicateRoll(students_data[i].rollNumber, students_data, i)){
            if (isDuplicateRoll(students_data[i].rollNumber, students_data, i)) {
                printf("Duplicate roll number! Please enter a unique one.\n");
            }
            printf("Invalid input! Please enter valid details.\n");
            while(getchar() != '\n');
            i--;
            continue;
        }

        if (students_data[i].marks1 < 0 || students_data[i].marks1 > 100 || 
            students_data[i].marks2 < 0 || students_data[i].marks2 > 100 || 
            students_data[i].marks3 < 0 || students_data[i].marks3 > 100) {
            printf("Invalid marks (should be 0-100). Please re-enter.\n");
            i--;
            continue;
        }
    }

    for( int i=0;i<N;i++){

        int invalid_score=0;
        int totalScore=Total_Score(students_data[i],&invalid_score);
        float averageScore=Average_Score(totalScore,&invalid_score);
        char grade=Grade_Evaluator(averageScore,&invalid_score);

        printf("\nRoll: %d\n",students_data[i].rollNumber);
        printf("Name: %s\n",students_data[i].name);
        printf("Total: %d\n",totalScore);
        printf("Average: %.2f\n",averageScore);
        printf("Grade: %c\n",grade);

        if(grade=='F'){
            continue;
        }

        printf("Performance: ");
        Performance_Evaluator(grade,&invalid_score);
    }

    printf("\nList of all Roll Numbers (via recursion): ");
    PrintRollNumber(0,N,students_data);
    printf("\n");

    return 0;

}