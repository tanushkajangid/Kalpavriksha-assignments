#include<stdio.h>

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

int isInvalidTotal(int totalMarks) {
    return (totalMarks < 0 || totalMarks > 300);
}

int isInvalidAverage(float averageMarks) {
    return (averageMarks < 0 || averageMarks > 100);
}

char gradeEvaluator(float average_score, int *invalidScore) {

    if (isInvalidAverage(average_score)) {
        *invalidScore = 1;
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

int calculateTotalScore(struct studentRecord studentsData, int *invalidScore) {

    if (isInvalidMarks(studentsData)) {
        *invalidScore = 1;
        return -1;
    }

    return studentsData.marks1 + studentsData.marks2 +studentsData.marks3;
}

float calculateAverageScore(int total_score, int *invalidScore) {

    if (isInvalidTotal(total_score)) {
        *invalidScore = 1;
        return -1;
    }

    return total_score / 3.0;
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
    return;
}

int getStudentInput(struct studentRecord *student) {
   
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
        printf("Enter Details of Student %d (Roll Number, Name, Marks1, Marks2, Marks3): ", i+1);
        
        if(getStudentInput(&studentsData[i]) || studentsData[i].rollNumber <= 0 ){
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
