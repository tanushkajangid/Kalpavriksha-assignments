#include <stdio.h>
#define limit 100 

int applyOperation(int a, int b, char op, int *divZero) {
    if (op == '/') {
        if (b == 0) {
            *divZero = 1;
            return 0;
        }
        return a / b;
    }
    if (op == '*') return a * b;
    if (op == '+') return a + b;
    return a - b;
}
int evaluateExpressions(char expression[], int *divZero, int *invalid) {
    int numbers[limit];
    char operators[limit];
    int operatorCount = 0, numberCount = 0;
    int i = 0;
    int lastWasOperator = 1;

    while (expression[i] != '\0') {
        if (expression[i] == ' ') {
            i++; 
        } 
        else if (expression[i] >= '0' && expression[i] <= '9') {
            int value = 0;
            while (expression[i] >= '0' && expression[i] <= '9') {
                value = value * 10 + (expression[i] - '0');
                i++;
            }
            numbers[numberCount++] = value;
            lastWasOperator = 0;
        } 
        else if (expression[i] == '+' || expression[i] == '-' || 
                 expression[i] == '*' || expression[i] == '/') {
        // check for invalid cases: if 2 operators together
            if (lastWasOperator) {
                *invalid = 1;
                return 0;
            }
            operators[operatorCount++] = expression[i];
            lastWasOperator = 1;
            i++;
        } 
        else {
            *invalid = 1;   
            return 0;
        }
    }
    
    if (lastWasOperator ||operatorCount >= numberCount) {
        *invalid = 1;
        return 0;
    }
    
    for (i = 0; i < operatorCount; i++) {           
        if (operators[i] == '*' || operators[i] == '/') {
            numbers[i] = applyOperation(numbers[i], numbers[i + 1], operators[i], divZero);
            if (*divZero) return 0;
            
            for (int j = i + 1; j < numberCount - 1; j++) numbers[j] = numbers[j + 1];
            for (int j = i; j < operatorCount - 1; j++) operators[j] = operators[j + 1];
            numberCount--;
            operatorCount--;
            i--;
        }
    }
    int result = numbers[0];
    for (i = 0; i < operatorCount; i++) {
        result = applyOperation(result, numbers[i + 1], operators[i], divZero);
        if (*divZero) return 0;
    }
    return result;
}

// for single pass stack based approach

// int evaluateExpressions(char expression[], int *divZero, int *invalid) {
//     int numberStack[limit];    
//     char operatorStack[limit];   
//     int topNumber = -1, topOperator = -1;
//     int i = 0;
//     int lastWasOperator = 1;

//     while (expression[i] != '\0') {
//         if (expression[i] == ' ') {
//             i++;
//         }
//         else if (expression[i] >= '0' && expression[i] <= '9') {
//             int value = 0;
//             while (expression[i] >= '0' && expression[i] <= '9') {
//                 value = value * 10 + (expression[i] - '0');
//                 i++;
//             }
//             numberStack[++topNumber] = value;

//             while (topOperator >= 0 && (operatorStack[topOperator] == '*' || operatorStack[topOperator] == '/')) {
//                 int b = numberStack[topNumber--];
//                 int a = numberStack[topNumber--];
//                 char op = operatorStack[topOperator--];
//                 int answer = applyOperation(a, b, op, divZero);
//                 if (*divZero) return 0;
//                 numberStack[++topNumber] = answer;
//             }
//             lastWasOperator = 0;
//         }
//         else if (expression[i] == '+' || expression[i] == '-' || 
//                  expression[i] == '*' || expression[i] == '/') {
//             if (lastWasOperator) {  // consecutive operators
//                 *invalid = 1;
//                 return 0;
//             }
//             operatorStack[++topOperator] = expression[i];
//             lastWasOperator = 1;
//             i++;
//         }
//         else {
//             *invalid = 1;
//             return 0;
//         }
//     }

//     if (lastWasOperator) { // expression ending with operator
//         *invalid = 1;
//         return 0;
//     }

//     // process remaining + and - operators
//     int result = numberStack[0];
//     int numIndex = 1;
//     for (int j = 0; j <= topOperator; j++) {
//         result = applyOperation(result, numberStack[numIndex++], operatorStack[j], divZero);
//         if (*divZero) return 0;
//     }
//     return result;
// }

int main(){
    char expression[limit];
    printf("Enter expression: ");
    scanf("%[^\n]", expression);   
    int divZero = 0, invalid = 0;
    int result = evaluateExpressions(expression, &divZero, &invalid);
    if(divZero) printf("Error: Division by zero.\n");
    else if(invalid) printf("Error: Invalid expression.\n");
    else printf("Result = %d\n", result);
    return 0;
}
