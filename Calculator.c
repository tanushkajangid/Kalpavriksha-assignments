#include <stdio.h>
#include <limits.h>
#define limit 100 

int applyOperation(int a, int b, char op, int *divZero, int *overFlow) {
    long long result=0;
    
    switch (op) {
        case '/':
            if (b == 0) {
                *divZero = 1;
                return 0;
            }
            result = a / b;
            break;

        case '*':
            result = (long long)a * b;
            break;

        case '+':
            result = (long long)a + b;
            break;

        case '-':
            result = (long long)a - b;
            break;

        default: 
            *overFlow = 1;
            return 0;
    }

    if (result > INT_MAX || result < INT_MIN) {
        *overFlow = 1;
        return 0;
    }

    return (int)result;
}

int evaluateExpressions(char expression[], int *divZero, int *invalid, int *overFlow) {
    
    int numberStack[limit];    
    char operatorStack[limit];   
    int topNumber = -1, topOperator = -1;
    int i = 0;
    int lastWasOperator = 1;

    while (expression[i] != '\0') {
        
        if (expression[i] == ' ') {
            i++;
        }

        else if (expression[i] == '-' && lastWasOperator) {
            i++; 
            
            if (expression[i] >= '0' && expression[i] <= '9') {
                int value = 0;
                
                while (expression[i] >= '0' && expression[i] <= '9') {
                    value = value * 10 + (expression[i] - '0');
                    i++;
                }
                numberStack[++topNumber] = -value;
                lastWasOperator = 0;
            } 
                
            else {
                *invalid = 1;
                return 0;
            }
        }
            
        else if (expression[i] >= '0' && expression[i] <= '9') {
            int value = 0;
            
            while (expression[i] >= '0' && expression[i] <= '9') {
                value = value * 10 + (expression[i] - '0');
                i++;
            }
            
            numberStack[++topNumber] = value;

            while (topOperator >= 0 && (operatorStack[topOperator] == '*' || operatorStack[topOperator] == '/')) {
                int b = numberStack[topNumber--];
                int a = numberStack[topNumber--];
                char op = operatorStack[topOperator--];
                
                int answer = applyOperation(a, b, op, divZero,overFlow);
                if (*divZero || *overFlow) {
                    return 0;
                }
                
                numberStack[++topNumber] = answer;
            }
            
            lastWasOperator = 0;
        }
            
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            
            if (lastWasOperator) {
                *invalid = 1;
                return 0;
            }
            
            operatorStack[++topOperator] = expression[i];
            lastWasOperator = 1;
            i++;
        }
            
        else {
            *invalid = 1;
            return 0;
        }
    }

    if (lastWasOperator) {
        *invalid = 1;
        return 0;
    }
    
    int result = numberStack[0];
    int numIndex = 1;
    
    for (int j = 0; j <= topOperator; j++) {
        result = applyOperation(result, numberStack[numIndex++], operatorStack[j], divZero, overFlow);
        if (*divZero || *overFlow) {
            return 0;
        }
    }
    
    return result;
}

int main(){
    
    char expression[limit];
    
    printf("Enter expression: ");
    scanf("%[^\n]", expression);  
    
    int divZero = 0, invalid = 0, overFlow=0;
    
    int result = evaluateExpressions(expression, &divZero, &invalid, &overFlow);
    
    if(divZero) { 
        printf("Error: Division by zero.\n");
    }
    else if(invalid) {
        printf("Error: Invalid expression.\n");
    }
    else if (overFlow) {
        printf("Error: Integer overflow detected.\n");
    }
    else {
        printf("Result = %d\n", result);
    }
    return 0;
}
