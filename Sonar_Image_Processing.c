#include<stdio.h>
#include<stdlib.h>
#include<time.h>

const int MIN_CELLS=2;
const int MAX_CELLS=10;
const int MAX_INTENSITY_VALUE=255;

int isValidInput(int numberOfCells){

    if(numberOfCells<MIN_CELLS || numberOfCells>MAX_CELLS){
        return 0;
    }

    return 1;
}

void generateRandomImageMatrix(int numberOfCells, int (*imagePointer)[numberOfCells]) {

    srand(time(NULL));
    for (int matrixRow = 0; matrixRow < numberOfCells; matrixRow++) {
        for (int matrixColumn = 0; matrixColumn < numberOfCells; matrixColumn++) {
            *(*(imagePointer+matrixRow)+matrixColumn) = rand() % MAX_INTENSITY_VALUE;
        }
    }
}

void displayImageMatrix(int numberOfCells, int (*imagePointer)[numberOfCells]) {

    for (int matrixRow = 0; matrixRow < numberOfCells; matrixRow++) {
        for (int matrixColumn = 0; matrixColumn < numberOfCells; matrixColumn++) {
            printf("%3d ", *(*(imagePointer + matrixRow) + matrixColumn));
        }
        printf("\n");
    }
}

void swapCellsInPlace(int *topRow, int *leftColumn, int *bottomRow, int *rightColumn) {
    int temporaryCell = *leftColumn;
    *leftColumn = *bottomRow;
    *bottomRow = *rightColumn;
    *rightColumn = *topRow;
    *topRow = temporaryCell;
}

void applySmoothingFilter(int numberOfCells, int (*imagePointer)[numberOfCells]){

    int previousSmoothenImage[numberOfCells];
    int currentSmoothenImage[numberOfCells];

    for(int rowInRotatedImage=0; rowInRotatedImage<numberOfCells; rowInRotatedImage++){
        for( int columnInRotatedImage=0; columnInRotatedImage<numberOfCells; columnInRotatedImage++){
            int sumOfNeighbours=0;
            int countOfNeighbours=0;

            for(int neighbourRow= rowInRotatedImage-1; neighbourRow<= rowInRotatedImage+1; neighbourRow++){

                for(int neighbourColumn= columnInRotatedImage-1; neighbourColumn<= columnInRotatedImage+1; neighbourColumn++){

                    if(neighbourRow>=0 && neighbourRow<numberOfCells && neighbourColumn>=0 && neighbourColumn<numberOfCells){

                        sumOfNeighbours+= *(*(imagePointer + neighbourRow)+ neighbourColumn);
                        countOfNeighbours++;
                    }
                }
            }
            *(currentSmoothenImage+columnInRotatedImage)= sumOfNeighbours/ countOfNeighbours;

        }
        if(rowInRotatedImage>0){
            for(int copyColumn=0; copyColumn<numberOfCells; copyColumn++){
                *(*(imagePointer + (rowInRotatedImage-1))+ copyColumn)= *(previousSmoothenImage+ copyColumn);
            }
        }

        for(int copyColumn=0; copyColumn<numberOfCells; copyColumn++){
            *(previousSmoothenImage+ copyColumn)= *(currentSmoothenImage+ copyColumn);
        }
    }
    for(int copyColumn=0; copyColumn<numberOfCells; copyColumn++){
        *(*(imagePointer + (numberOfCells-1))+ copyColumn)= *(previousSmoothenImage+ copyColumn);
    }
}

void rotateMatrixBy90Degrees(int numberOfCells, int (*imagePointer)[numberOfCells]){
    
    for (int layer = 0; layer < numberOfCells / 2; layer++) {
        int firstLayer = layer;
        int lastLayer = numberOfCells - 1 - layer;

        for (int iterator = firstLayer; iterator < lastLayer; iterator++) {

            int currentPositionInLayer = iterator - firstLayer;

            int *topRow = (*(imagePointer + firstLayer) + iterator);
            int *leftColumn = (*(imagePointer + (lastLayer - currentPositionInLayer)) + firstLayer);
            int *bottomRow = (*(imagePointer + lastLayer) + (lastLayer - currentPositionInLayer));
            int *rightColumn = (*(imagePointer + iterator) + lastLayer);

            swapCellsInPlace(topRow, leftColumn, bottomRow, rightColumn);
        }
    }
}

int main() {

    int numberOfCells;
    printf("Enter Number Of Cells for Image generation: ");

    if(scanf("%d", &numberOfCells) != 1 || !isValidInput(numberOfCells)){
        printf("Error: Number Of Cells Should Be In Range [2,10].\n");
        return 1;
    }

    int sonarIntensityValueImage[numberOfCells][numberOfCells];

    int (*imagePointer)[numberOfCells]= sonarIntensityValueImage;

    printf("Original Randomly Generated Matrix:\n");
    generateRandomImageMatrix(numberOfCells, imagePointer);
    displayImageMatrix(numberOfCells, imagePointer);

    printf("Matrix after 90 degree Clockwise Rotation:\n");
    rotateMatrixBy90Degrees(numberOfCells, imagePointer);
    displayImageMatrix(numberOfCells, imagePointer);

    printf("Matrix after Applying 3x3 Smoothing Filter:\n");
    applySmoothingFilter(numberOfCells, imagePointer);
    displayImageMatrix(numberOfCells, imagePointer);

    return 0;

}