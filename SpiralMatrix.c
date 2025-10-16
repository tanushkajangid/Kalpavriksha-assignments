#include<stdio.h>

void fillSpiralMatrix( int numberOfCells, int (*matrixPtr)[numberOfCells]){
    int cellValue=1;
    int topRow=0, bottomRow= numberOfCells-1;
    int leftColumn=0, rightColumn= numberOfCells-1;

    while(topRow<= bottomRow && leftColumn<= rightColumn){

        for( int column=leftColumn; column<=rightColumn; column++){
            *(*(matrixPtr+ topRow)+ column)= cellValue++;
        }
        topRow++;

        for( int row=topRow; row<=bottomRow; row++){
            *(*(matrixPtr+ row)+ rightColumn)= cellValue++;
        }
        rightColumn--;

        if(topRow<=bottomRow){
            
            for( int column=rightColumn; column>=leftColumn; column--){
                *(*(matrixPtr+ bottomRow)+ column)= cellValue++;
            }
            bottomRow--;
        }
        
        if(leftColumn<=rightColumn){
            
            for( int row=bottomRow; row>=topRow; row--){
                *(*(matrixPtr+ row)+ leftColumn)= cellValue++;
            }
            leftColumn++;
        }
    }
}

void displayMatrixAs2DArray(int numberOfCells, int (*matrixPtr)[numberOfCells]){

    printf("[");

    for( int row=0; row< numberOfCells; row++){

        printf("[");

        for( int column=0; column< numberOfCells; column++){

            printf("%d", *(*(matrixPtr + row)+column));

            if(column!= numberOfCells-1){
                printf(",");
            }
        }
        printf("]");

        if(row!= numberOfCells-1){
            printf(",");
        }
    }

    printf("]\n");
}

int main(){
    
    int numberOfCells;

    printf("Enter Number Of Cells for Square Matrix : ");
    scanf("%d", &numberOfCells);

    int Matrix[numberOfCells][numberOfCells];

    int (*matrixPtr)[numberOfCells]= Matrix;

    fillSpiralMatrix(numberOfCells, matrixPtr);
    displayMatrixAs2DArray(numberOfCells, matrixPtr);

    return 0;

}