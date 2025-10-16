#include<stdio.h>

void fillSpiralMatrix( int numberOfCells, int (*matrixPointer)[numberOfCells]){
    int cellValue=1;
    int topRow=0, bottomRow= numberOfCells-1;
    int leftColumn=0, rightColumn= numberOfCells-1;

    while(topRow<= bottomRow && leftColumn<= rightColumn){

        for( int column=leftColumn; column<=rightColumn; column++){
            *(*(matrixPointer+ topRow)+ column)= cellValue++;
        }
        topRow++;

        for( int row=topRow; row<=bottomRow; row++){
            *(*(matrixPointer+ row)+ rightColumn)= cellValue++;
        }
        rightColumn--;

        if(topRow<=bottomRow){
            
            for( int column=rightColumn; column>=leftColumn; column--){
                *(*(matrixPointer+ bottomRow)+ column)= cellValue++;
            }
            bottomRow--;
        }
        
        if(leftColumn<=rightColumn){
            
            for( int row=bottomRow; row>=topRow; row--){
                *(*(matrixPointer+ row)+ leftColumn)= cellValue++;
            }
            leftColumn++;
        }
    }
}

void displayMatrixAs2DArray(int numberOfCells, int (*matrixPointer)[numberOfCells]){

    printf("[");

    for( int row=0; row< numberOfCells; row++){

        printf("[");

        for( int column=0; column< numberOfCells; column++){

            printf("%d", *(*(matrixPointer + row)+column));

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

int isValidInput(int numberOfCells){

    if(numberOfCells<=0){
        return 0;
    }
    return 1;
}

int main(){
    
    int numberOfCells;

    printf("Enter Number Of Cells for Square Matrix : ");
    
    if(scanf("%d", &numberOfCells) != 1 || !isValidInput(numberOfCells)){
        printf("Error: Number Of Cells Should Be A Positive Integer.\n");
        return 1;
    }

    int Matrix[numberOfCells][numberOfCells];

    int (*matrixPointer)[numberOfCells]= Matrix;

    fillSpiralMatrix(numberOfCells, matrixPointer);
    displayMatrixAs2DArray(numberOfCells, matrixPointer);

    return 0;

}
