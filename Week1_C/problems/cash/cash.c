#include <cs50.h>
#include <stdio.h>

int get_cents(void);
// int calc_quarters(int cents);
// int calc_nickels(int cents);
// int calc_dimes(int cents);
// int calc_pennies(int cents);

int main(void)
{
    // first study the problem and study the algorithm
    int cents = get_cents();

    int quarters = calc_quarters(cents); 
    cents %= 25; // give me the remainder after i delete every 25 from cents, and save it to cents.
    // calc dimes
    int dimes = cents / 10;
    cents %= 10;
    // calc nickels
    int nickels = cents / 5;
    cents %= 5;
    // calc pennies
    int pennies = cents / 1;


    printf("%i\n", (quarters + dimes + nickels + pennies));
}
int get_cents(void)
{
    int cents;
    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 1);
    return cents;
}
int calc_quarters(int cents)
{
    int quarters = 0;
    while (cents >= 25)
    {
        cents -= 25;
        quarters++;
    }
    return quarters;
}
int calc_dimes(int cents)
{
    int dimes = 0;
    while (cents >= 10)
    {
        cents -= 10;
        dimes++;
    }
    return dimes;
}
int calc_nickels(int cents)
{
    int nickels = 0;
    while (cents >= 5)
    {
        cents -= 5;
        nickels++;
    }
    return nickels;
}
int calc_pennies(int cents)
{
    int pennies = 0;
    while (cents >= 1)
    {
        cents -= 1;
        pennies++;
    }
    return pennies;
}
