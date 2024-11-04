#include <cs50.h>
#include <stdio.h>

long get_cardnum(void);
int cardLength(long num);
int doluhn(long num);
string determineCardType(long num);

int main(void)
{
    // Obtain card number from user
    long num = get_cardnum();
    int length = cardLength(num);
    string type = determineCardType(num);

    // If the card isn't 13, 15, or 16 digits long then it's invalid
    if (length != 13 && length != 15 && length != 16)
    {
        printf("INVALID\n");
    }

    // If it's not invalid, it will print the card type
    else
    {
        printf("%s\n", type);
    }
}

long get_cardnum(void)
{
    // Prompt user for credit card number
    long num;
    do
    {
        num = get_long("Card Number: ");
    }
    while (num < 0);

    return num;
}

int cardLength(long num)
{
    int length = 0;
    do
    {
        num /= 10; // Dividing by 10 shrinks the number by getting rid of the last digit
        length++;
    }
    while (num > 0);
    return length;
}

int doluhn(long num)
{
    int m1 = 0; // This will be the first %10 to get the last digit
    int m2 = 0; // Second %10 to get second to last digit
    int sum1 = 0; // Sum of of the products' digits after multiplying second to last digits by 2
    int sum2 = 0; // Sum of the digits that don't get multiplied by 2
    int d1 = 0; // Digit 1 after multiplying by 2
    int d2 = 0; // Digit 2 after muliplying by 2
    int total = 0; // Sum of both sums

    do
    {
        m1 = num % 10; // Store last digit in m1
        num /= 10; // Divide number by 10 to get rid of last digit and shorten the number
        sum2 += m1; // Add last digit to sum2

        m2 = num % 10; // Get the last digit of the shortened number (second to last digit of the original number)
        num /= 10; // Divide number by 10 to shorten number again
        m2 *= 2; // Multiply second to last digit by 2
        d2 = m2 % 10; // Get the last digit of the products' digit
        d1 = m2 / 10; // Get first digit of the products' digit
        sum1 += d1 + d2; // Add the digits together and to sum1
    }
    while (num > 0);

    total = sum1 + sum2; // Get the total of the two sums
    return total;
}

string determineCardType(long num)
{
    // Set the first and second numbers to 0 and obtain total from Luhn's algorithm
    int firstnum = 0;
    int secondnum = 0;
    int l = doluhn(num);

    // Reduce the number to its first 2 digits to determine which numbers it starts with to determine the type of card
    do
    {
        num /= 10;
    }
    while (num > 100); // This will ensure the number doesn't divide by 10 once the number is reduced to 2 digits

    firstnum = num / 10; // Dividing by 10 gets rid of the last digit and leaves the first digit
    secondnum = num % 10; // Getting modulo 10 gets the last digit

    // These conditional statements check the first digits and make sure the total from Luhn's Algorithm has 0 as its last digit
    if (firstnum == 4 && l % 10 == 0)
    {
        return "VISA";
    }
    else if (firstnum == 5 && (1 <= secondnum && secondnum <= 5) && l % 10 == 0)
    {
        return "MASTERCARD";
    }
    else if (firstnum == 3 && (secondnum == 4 || secondnum == 7) && l % 10 == 0)
    {
        return "AMEX";
    }
    else
    {
        return "INVALID";
    }
}
