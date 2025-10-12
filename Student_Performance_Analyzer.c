#include<stdio.h>

const int GRADE_MAX_MARKS   = 100;
const int GRADE_A_MIN_MARKS = 85;
const int GRADE_B_MIN_MARKS = 70;
const int GRADE_C_MIN_MARKS = 50;
const int GRADE_D_MIN_MARKS = 35;
const int GRADE_MIN_MARKS   = 0;

struct studentRecord{
    int rollNumber;
    char name[100];
    int marks1;
    int marks2;
    int marks3;
};

int isDuplicateRoll(int roll, struct studentRecord studentsData[], int count) {
    
    for (int i = 0; i < count; i++) {
        if (studentsData[i].rollNumber == roll)
            return 1;
    }
    return 0;
}

int isInvalidMarks(struct studentRecord studentsData) {
    return(studentsData.marks1 < 0 || studentsData.marks1 > 100 || 
        studentsData.marks2 < 0 || studentsData.marks2 > 100 || 
        studentsData.marks3 < 0 || studentsData.marks3 > 100);
}

char gradeEvaluator(float averageScore, int *invalidScore) {

    if (averageScore < GRADE_MIN_MARKS || averageScore > GRADE_MAX_MARKS) {
        *invalidScore = 1;
        return 'X';
    }

    if (averageScore >= GRADE_A_MIN_MARKS){
        return 'A';
    }
    
    else if (averageScore >= GRADE_B_MIN_MARKS) {
        return 'B';
    }

    else if (averageScore >= GRADE_C_MIN_MARKS) {
        return 'C';
    }

    else if (averageScore >= GRADE_D_MIN_MARKS) {
        return 'D';
    }

    else{
         return 'F';
    }
}

int calculateTotalScore(struct studentRecord studentsData, int *invalidScore) {

    if (isInvalidMarks(studentsData)) {
        *invalidScore = 1;
        return -1;
    }

    return studentsData.marks1 + studentsData.marks2 +studentsData.marks3;
}

float calculateAverageScore(int totalScore, int *invalidScore) {

    if (totalScore < 0 || totalScore > 300) {
        *invalidScore = 1;
        return -1;
    }

    return totalScore / 3.0;
}

void performanceEvaluator(char grade, int *invalidScore) {
    int stars=0;

    if (grade == 'X') {
        *invalidScore = 1;
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

void printRollNumber(int index,int N, struct studentRecord studentsData[]){

    if(index==N) {
        return;
    }

    printf("%d ",studentsData[index].rollNumber);
    printRollNumber(index+1,N,studentsData);
}

void displayStudentDetails(struct studentRecord studentsData){

    int invalidScore=0;
    int totalScore=calculateTotalScore(studentsData ,&invalidScore);
    float averageScore=calculateAverageScore(totalScore,&invalidScore);
    char grade=gradeEvaluator(averageScore,&invalidScore);

    printf("\nRoll: %d\n",studentsData.rollNumber);
    printf("Name: %s\n",studentsData.name);
    printf("Total: %d\n",totalScore);
    printf("Average: %.2f\n",averageScore);
    printf("Grade: %c\n",grade);

    if(grade=='F'){
        return;
    }

    printf("Performance: ");
    performanceEvaluator(grade,&invalidScore);
}

int getStudentInput(struct studentRecord *student,int index) {
   
    printf("Enter Details of Student %d (Roll Number, Name, Marks1, Marks2, Marks3): ", index+1);

    if (scanf("%d %s %d %d %d", &student->rollNumber, student->name, 
        &student->marks1, &student->marks2, &student->marks3) != 5) {
        return 1;
    }
    
    return 0;
}

int main(){
    
    int N;
    printf("Enter number of students: ");

    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("Invalid number of students!\n");
        return 1;
    }

    struct studentRecord studentsData[N];

    for(int i=0;i<N;i++){
        
        if(getStudentInput(&studentsData[i],i) || studentsData[i].rollNumber <= 0 ){
            printf("Invalid input! Please enter valid details.\n");
            while(getchar() != '\n');
            i--;
            continue;
        }
        
        if (isDuplicateRoll(studentsData[i].rollNumber, studentsData, i)) {
            printf("Duplicate roll number! Please enter a unique one.\n");
            i--;
            continue;
        }

        if (isInvalidMarks(studentsData[i])) {
            printf("Invalid marks (should be 0-100). Please re-enter.\n");
            i--;
            continue;
        }
    }

    for( int i=0;i<N;i++){
        displayStudentDetails(studentsData[i]);
    }

    printf("\nList of all Roll Numbers (via recursion): ");
    printRollNumber(0,N,studentsData);
    printf("\n");

    return 0;
    
}
