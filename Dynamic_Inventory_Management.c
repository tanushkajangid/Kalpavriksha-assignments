#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INITIAL_PRODUCTS 100
#define MAX_NAME_LENGTH 50
#define MAX_ID 10000
#define MAX_PRICE 100000
#define MAX_QUANTITY 1000000
#define MIN_VALUE 0

struct ProductInformation        
{
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
};

int isProductNumberValid(int numberOfProducts){

    if(numberOfProducts <= MIN_VALUE || numberOfProducts > MAX_INITIAL_PRODUCTS){
        return 1;
    }

    return 0;
}

int isIdUnique(struct ProductInformation *product, int currentIndexValue, int IdToCheck){

    for(int index = 0; index < currentIndexValue; index++){

        if(product[index].id == IdToCheck){
            return 1;
        }
    }

    return 0;
}

int isIdValid(int id){

    if(id <= MIN_VALUE || id > MAX_ID){
        return 1;
    }

    return 0;
}

int isNameValid(char name[]){

    for(int index = 0; name[index] !='\0'; index++){

        if(!isalpha(name[index]) && name[index] != '_' && name[index] != '-'){
            return 1;
        }
    }

    return 0;
}

int isPriceValid(float price){

    if(price < MIN_VALUE || price > MAX_PRICE){
        return 1;
    }

    return 0;
}

int isQuantityValid(int quantity){

    if(quantity < MIN_VALUE || quantity > MAX_QUANTITY){
        return 1;
    }

    return 0;
}

void inputSingleProduct(struct ProductInformation *product, int currentCount, int index){

    do{

        printf("\nEnter details for product %d", index + 1);

        printf("\nProduct ID: ");

        if(scanf("%d", &product[index].id) != 1 || product[index].id <= MIN_VALUE || product[index].id > MAX_ID){
            printf("Error: Invalid Input for Product ID \n");
            while(getchar() != '\n');
            continue;
        }
        
        if(isIdUnique(product, currentCount, product[index].id)){
            printf("\nError: ID already  exist! Enter Unique ID\n");
            continue;
        }

        printf("Product Name: ");

        if(scanf("%s", product[index].name) !=1 || isNameValid(product[index].name)){
            printf("Error: Invalid Input for Product Name \n");
            while(getchar() != '\n');
            continue;
        }

        printf("Product Price: ");

        if(scanf("%f", &product[index].price) != 1 || isPriceValid(product[index].price)){
            printf("Error: Invalid Input for Product Price\n");
            while(getchar() != '\n');
            continue;
        }

        printf("Product Quantity: ");

        if(scanf("%d", &product[index].quantity) != 1 || isQuantityValid(product[index].quantity)){
            printf("Error: Invalid Input for Product Quantity \n");
            while(getchar() != '\n');
            continue;
        }

        break;

    } while(1);

}

int getInputForProductDetails(struct ProductInformation *product, int numberOfProducts){

    for(int index = 0; index < numberOfProducts; index++){

        inputSingleProduct(product, index, index);

    }
    return 0; 
}

void addNewProductToInventory(struct ProductInformation **product, int *numberOfProducts){

    int newIndex = *numberOfProducts;
    struct ProductInformation *newProduct = realloc(*product, (newIndex + 1) * sizeof(struct ProductInformation));

    if (newProduct == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    *product = newProduct;

    inputSingleProduct(*product, *numberOfProducts, newIndex);

    (*numberOfProducts)++;

    printf("Product added successfully! \n");

}

void viewAllProducts(struct ProductInformation *product, int numberOfProducts){

    printf("\n========= PRODUCT LIST =========\n");

    for(int index = 0; index < numberOfProducts; index++){
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", 
        product[index].id, product[index].name, product[index].price, product[index].quantity);
    }

}

void updateQuantity(struct ProductInformation *product, int numberOfProducts){

    printf("\nEnter Product ID to update quantity: ");
    int searchId;
    int found = 0;

    if(scanf("%d", &searchId) != 1 || isIdValid(searchId)){
        printf("Error: Invalid Input for Product ID \n");
        return;
    }

    printf("\nEnter new Quantity: ");
    int newQuantity;
    
    if(scanf("%d", &newQuantity) != 1 || isQuantityValid(newQuantity)){
        printf("Error: Invalid Input for Product Quantity \n");
        return;
    }

    for(int index = 0; index < numberOfProducts; index++){
        if(product[index].id == searchId){
            found = 1;
            product[index].quantity = newQuantity;
            break;
        }
    }

    if(!found){
        printf("\nNo Product Found With ID: %d !",searchId);
    }
    
    else{
        printf("Quantity updated successfully!\n");
    }
    
}

void printProductDetails( struct ProductInformation *product, int indexFoundAt){

    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", 
            product[indexFoundAt].id, product[indexFoundAt].name, product[indexFoundAt].price, product[indexFoundAt].quantity);
}

int searchProductById(struct ProductInformation *product, int numberOfProducts, int searchId){

    int indexFoundAt = -1;

    for(int index = 0; index < numberOfProducts; index++){

        if(product[index].id == searchId){
            indexFoundAt = index;
            break;
        }
    }

    return indexFoundAt;
}

int matchPartialName(const char *nameToSearch, const char *productNames){
    int index = 0, iterator = 0;

    while(productNames[index] != '\0'){

        if(tolower(productNames[index]) == tolower(nameToSearch[0])){
            iterator = 0;

            while(nameToSearch[iterator] != '\0' && tolower(productNames[index + iterator]) == tolower(nameToSearch[iterator])){
                iterator++;
            }

            if(nameToSearch[iterator] == '\0'){
                return 1;
            }
        }

        index++;
    }
    return 0;
}

int searchProductByName(struct ProductInformation *product, int numberOfProducts, int foundIndices[]){

    printf("\nEnter Product Name to search: ");
    char nameToSearch[MAX_NAME_LENGTH];

    int indexCount = 0;

    if(scanf("%s", nameToSearch) != 1 || isNameValid(nameToSearch)){
        printf("\nError: Invalid Input for Product Name \n");
        return 1;
    }

    for(int index = 0; index < numberOfProducts; index++){

        if( matchPartialName (nameToSearch, product[index].name)){
            foundIndices[ indexCount++ ] = index;                              
        }
    }

    return indexCount;
}

int searchProductByPriceRange(struct ProductInformation *product, int numberOfProducts, int foundIndices[]){

    printf("\nEnter Product Price Range (minimum, maximum) to search:");
    float minimumRangeToSearch;
    float maximumRangeToSearch;
    int indexCount = 0;

    if(scanf("%f %f", &minimumRangeToSearch, &maximumRangeToSearch) != 2 || isPriceValid(minimumRangeToSearch) || isPriceValid(maximumRangeToSearch) || 
        minimumRangeToSearch < 0 || maximumRangeToSearch < 0 || maximumRangeToSearch < minimumRangeToSearch){
        printf("\nError: Invalid Input for Price Range \n");
        return 1;
    }

    for(int index = 0; index < numberOfProducts; index++){

        if(product[index].price >= minimumRangeToSearch && product[index].price <= maximumRangeToSearch){
            foundIndices[indexCount++] = index;
        }
    }

    return indexCount;
}

void deleteProductFromInventory(struct ProductInformation **product, int *numberOfProducts){

    if(*product == NULL || *numberOfProducts <= 0){
        printf("\nNo products to delete.\n");
        return;
    }

    printf("\nEnter Product ID to delete: ");
    int idToDelete;

    if(scanf("%d", &idToDelete) != 1 || isIdValid(idToDelete)){
        printf("\nError: Invalid Input for Product ID \n");
        while(getchar() != '\n');
        return;
    }

    int deleteIndex = searchProductById(*product, *numberOfProducts, idToDelete);

    if(deleteIndex == -1){
        printf("\nProduct Not Found!\n");
        return;
    }
     
    for(int iterator = deleteIndex; iterator < *numberOfProducts - 1; iterator++){

        (*product)[iterator] = (*product)[iterator + 1];
    }

    (*numberOfProducts)--;

    if(*numberOfProducts > 0){        

        struct ProductInformation *temporaryProduct = realloc(*product, (*numberOfProducts) * sizeof(struct ProductInformation));

        if(temporaryProduct != NULL){
            *product = temporaryProduct;
        }

        else{
            printf("\nUnable to shrink Memory, Product Deleted! \n");
        }

    }

    else{                          
        free(*product);
        *product = NULL;
    }
    
    printf("\nProduct deleted successfully! \n");
    
}

void inventoryMenu(struct ProductInformation **product, int *numberOfProducts){

    int choice = 0;
    int indexCount = 0;
    int indexFoundAt = -1;
    int foundIndices[MAX_INITIAL_PRODUCTS];
    int searchId;

    while(choice != 8){

        printf("\n=========  INVENTORY MENU =========\n");

        printf("\n1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search product by Name\n");
        printf("6. Search Product by PriceRange\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");

        printf("Enter your choice: ");
        if(scanf("%d", &choice) != 1){
            printf("\nEnter Valid Choice! \n");
            while(getchar() != '\n');
            continue;
        }

        while(getchar() != '\n');

        switch(choice){

            case 1:
                addNewProductToInventory(product, numberOfProducts); 
                break;

            case 2:
                viewAllProducts(*product, *numberOfProducts);
                break;

            case 3:
                updateQuantity(*product, *numberOfProducts);
                break;

            case 4:
                printf("\nEnter Product ID to search: ");
                
                if(scanf("%d", &searchId) != 1 || isIdValid(searchId)){
                    printf("\nError: Invalid Input for Product ID \n");
                    break;
                }

                indexFoundAt = searchProductById(*product, *numberOfProducts, searchId);

                if(indexFoundAt != -1){
                    printf("Product Found: \n");
                    printProductDetails(*product, indexFoundAt);
                }
                else{
                    printf("\nNo Product Found With ID!");
                }

                break;

            case 5:
                indexCount = searchProductByName(*product, *numberOfProducts, foundIndices);

                if(indexCount > 0){
                    printf("Products Found: \n");
                    for(int index = 0; index < indexCount; index++){
                        printProductDetails(*product, foundIndices[index]);
                    }
                }
                else{
                    printf("\nNo Product Found With This Name!");
                }

                break;

            case 6:
                indexCount = searchProductByPriceRange(*product, *numberOfProducts, foundIndices);

                if(indexCount > 0){
                    printf("\nProducts in price range: \n");

                    for(int index = 0; index < indexCount; index++){
                        printProductDetails(*product, foundIndices[index]);
                    }
                    
                }
                else{
                    printf("\nNo Product Found Within this Price Range! \n");
                }
                
                break;

            case 7:
                deleteProductFromInventory(product, numberOfProducts);
                break;
                
            case 8:
                printf("\nExiting program...\n");
                break;
            
            default:
                printf("\nInvalid Choice! \n");

        }
    }

    free(*product);
    *product = NULL;
    printf("\nMemory released successfully.\n");
}

int main(){

    int numberOfProducts;
    printf("\nEnter initial number of products: ");

    if(scanf("%d", &numberOfProducts) != 1 || isProductNumberValid(numberOfProducts)){
        printf("\nError: Invalid Input for Number of Products \n");
        return 1;
    }

    struct ProductInformation *inventory = calloc (numberOfProducts, sizeof(struct ProductInformation));

    if (inventory == NULL) {
        printf("\nMemory allocation failed.\n");
        return 1;
    }

    getInputForProductDetails(inventory, numberOfProducts);

    inventoryMenu(&inventory, &numberOfProducts);

    return 0;

}
