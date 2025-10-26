#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_PRODUCTS 100
#define MAX_NAME_LENGTH 50
#define MAX_ID 10000
#define MAX_PRICE 100000
#define MAX_QUANTITY 1000000
#define MIN_VALUE 0

struct ProductInformation
{
    int productId;
    char productName[MAX_NAME_LENGTH];
    float productPrice;
    int productQuantity;
};

int isProductNumberValid(int numberOfProducts){

    if(numberOfProducts <= MIN_VALUE || numberOfProducts > MAX_PRODUCTS){
        return 1;
    }

    return 0;
}

int isIdUnique(struct ProductInformation *products, int currentIndexValue, int IdToCheck){

    for(int index = 0; index < currentIndexValue; index++){

        if(products[index].productId == IdToCheck){
            return 1;
        }
    }

    return 0;
}

int isIdValid(int productId){

    if(productId <= MIN_VALUE || productId > MAX_ID){
        return 1;
    }

    return 0;
}

int isNameValid(char productName[]){

    for(int index = 0; productName[index] !='\0'; index++){

        if(!isalpha(productName[index])){
            return 1;
        }
    }

    return 0;
}

int isPriceValid(float productPrice){

    if(productPrice < MIN_VALUE || productPrice > MAX_PRICE){
        return 1;
    }

    return 0;
}

int isQuantityValid(int productQuantity){

    if(productQuantity < MIN_VALUE || productQuantity > MAX_QUANTITY){
        return 1;
    }

    return 0;
}

int inputSingleProduct(struct ProductInformation *product, int currentCount, int index){

    do{

        printf("\nEnter details for product %d", index + 1);

        printf("\nProduct ID: ");

        if(scanf("%d", &product[index].productId) != 1 || product[index].productId <= MIN_VALUE || product[index].productId > MAX_ID){
            printf("Error: Invalid Input for Product ID \n");
            while(getchar() != '\n');
            continue;
        }
        
        if(isIdUnique(product, currentCount, product[index].productId)){
            printf("\nError: ID already  exist! Enter Unique ID\n");
            continue;
        }

        printf("Product Name: ");

        if(scanf("%s", product[index].productName) !=1 || isNameValid(product[index].productName)){
            printf("Error: Invalid Input for Product Name \n");
            while(getchar() != '\n');
            continue;
        }

        printf("Product Price: ");

        if(scanf("%f", &product[index].productPrice) != 1 || isPriceValid(product[index].productPrice)){
            printf("Error: Invalid Input for Product Price\n");
            while(getchar() != '\n');
            continue;
        }

        printf("Product Quantity: ");

        if(scanf("%d", &product[index].productQuantity) != 1 || isQuantityValid(product[index].productQuantity)){
            printf("Error: Invalid Input for Product Quantity \n");
            while(getchar() != '\n');
            continue;
        }

        break;

    } while(1);

    return 0;
}

int getInputForProductDetails(struct ProductInformation *product, int numberOfProducts){

    for(int index = 0; index < numberOfProducts; index++){

        inputSingleProduct(product, index, index);

    }
    return 0; 
}

int addNewProductToInventory(struct ProductInformation **product, int *numberOfProducts){

    int newIndex = *numberOfProducts;
    struct ProductInformation *newProduct = realloc(*product, (newIndex + 1) * sizeof(struct ProductInformation));

    if (newProduct == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    *product = newProduct;

    inputSingleProduct(*product, *numberOfProducts, newIndex);

    (*numberOfProducts)++;

    printf("Product added successfully! \n");
    return 0;

}

int viewAllProducts(struct ProductInformation *product, int numberOfProducts){

    printf("\n========= PRODUCT LIST =========\n");

    for(int index = 0; index < numberOfProducts; index++){
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", 
        product[index].productId, product[index].productName, product[index].productPrice, product[index].productQuantity);
    }

    return 0;
}

int updateQuantity(struct ProductInformation *product, int numberOfProducts){

    printf("\nEnter Product ID to update quantity: ");
    int searchId;

    if(scanf("%d", &searchId) != 1 || isIdValid(searchId)){
        printf("Error: Invalid Input for Product ID \n");
        return 1;
    }

    printf("\nEnter new Quantity: ");
    int newQuantity;
    
    if(scanf("%d", &newQuantity) != 1 || isQuantityValid(newQuantity)){
        printf("Error: Invalid Input for Product Quantity \n");
        return 1;
    }

    for(int index = 0; index < numberOfProducts; index++){
        if(product[index].productId == searchId){
            product[index].productQuantity = newQuantity;
            break;
        }
    }

    printf("Quantity updated successfully!\n");
    return 0;
    
}

int searchProductById(struct ProductInformation *product, int numberOfProducts){

    printf("\nEnter Product ID to search: ");
    int searchId;
    int found = 0;

    if(scanf("%d", &searchId) != 1 || isIdValid(searchId)){
        printf("\nError: Invalid Input for Product ID \n");
        return 1;
    }

    for(int index = 0; index < numberOfProducts; index++){
        if(product[index].productId == searchId){
            found = 1;
            printf("Products Found: ");
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", 
            product[index].productId, product[index].productName, product[index].productPrice, product[index].productQuantity);
        }
    }

    if(!found){
        printf("\nNo Product Found With ID: %d !",searchId);
    }

    return 0;
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

int searchProductByName(struct ProductInformation *product, int numberOfProducts){

    printf("\nEnter Product Name to search: ");
    char nameToSearch[MAX_NAME_LENGTH];
    int found = 0;

    if(scanf("%s", nameToSearch) != 1 || isNameValid(nameToSearch)){
        printf("\nError: Invalid Input for Product Name \n");
        return 1;
    }

    for(int index = 0; index < numberOfProducts; index++){

        if( matchPartialName (nameToSearch, product[index].productName)){

            found = 1;
            printf("\nProducts Found: ");

            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", 
            product[index].productId, product[index].productName, product[index].productPrice, product[index].productQuantity);
        }
    }

    if(!found){
        printf("\nNo Product Found With Name: %s !", nameToSearch);
    }

    return 0;
}

int searchProductByPriceRange(struct ProductInformation *product, int numberOfProducts){

    printf("\nEnter Product Price Range (minimum, maximum) to search:");
    float minimumRangeToSearch;
    float maximumRangeToSearch;
    int found = 0;

    if(scanf("%f %f", &minimumRangeToSearch, &maximumRangeToSearch) != 2 || isPriceValid(minimumRangeToSearch) || isPriceValid(maximumRangeToSearch) || 
        minimumRangeToSearch < 0 || maximumRangeToSearch < 0 || maximumRangeToSearch < minimumRangeToSearch){
        printf("\nError: Invalid Input for Price Range \n");
        return 1;
    }

    for(int index = 0; index < numberOfProducts; index++){

        if(product[index].productPrice >= minimumRangeToSearch && product[index].productPrice <= maximumRangeToSearch){
            
            found = 1;
            printf("\nProducts in price range: ");
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", 
            product[index].productId, product[index].productName, product[index].productPrice, product[index].productQuantity);
        }
    }

    if(!found){
        printf("\nNo Product Found Within this Price Range! \n");
    }

    return 0;
}

int deleteProductFromInventory(struct ProductInformation **product, int *numberOfProducts){

    if(*product == NULL || *numberOfProducts <= 0){
        printf("\nNo products to delete.\n");
        return 1;
    }

    printf("\nEnter Product ID to delete: ");
    int idToDelete;

    if(scanf("%d", &idToDelete) != 1 || isIdValid(idToDelete)){
        printf("\nError: Invalid Input for Product ID \n");
        while(getchar() != '\n');
        return 1;
    }

    int found = 0;
    int deleteIndex = -1;

    for(int index = 0; index < *numberOfProducts; index++){

        if((*product)[index].productId == idToDelete){
            found = 1;
            deleteIndex = index;
            break;
        }
    }

    if(!found){
        printf("\nProduct Not Found!\n");
        return 0;
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
            printf("\nUnable to shrik Memeory, Product Deleted! \n");
        }

    }

    else{
        free(*product);
        *product = NULL;
    }
    
    printf("\nProduct deleted successfully! \n");
    return 0;
    
}

void inventoryMenu(struct ProductInformation **product, int *numberOfProducts){

    int choice = 0;

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
                searchProductById(*product, *numberOfProducts);
                break;

            case 5:
                searchProductByName(*product, *numberOfProducts);
                break;

            case 6:
                searchProductByPriceRange(*product, *numberOfProducts);
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