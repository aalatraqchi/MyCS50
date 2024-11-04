from cs50 import get_int


def main():
    # Prompt user for card number
    num = getNum()
    length = getLength(num)
    type = cardType(num)

    # Print out correct card type
    if length != 13 and length != 15 and length != 16:
        print("INVALID")
    else:
        print(f"{type}")


def getNum():
    while True:
        num = get_int("Number: ")

        # This will ensure the card isn't a negative number
        if num > 0:
            break
    return num


def getLength(cardNumber):
    length = 0
    while cardNumber >= 1:
        cardNumber //= 10
        length += 1
    return length


def luhn(n):

    # Define varialbles: last digit, second to last digit, first and second sums, individual digits for sum one, and total
    m1 = 0
    m2 = 0
    sum1 = 0
    sum2 = 0
    d1 = 0
    d2 = 0
    total = 0

    # Luhn's algorithm by getting last and second to last, applying them to their respective sums, then
    # shortening the card number and repeat
    while n > 1:
        m1 = n % 10  # Get the last digit
        n //= 10  # Shorten card number
        sum2 += m1  # Add last digit to sum2

        m2 = n % 10  # Get the last digit of the new shortened number (second to last digit of the original card number)
        n //= 10
        m2 *= 2  # Multiply by 2 then add its 2 individual digits to sum1
        d2 = m2 % 10
        d1 = m2 // 10
        sum1 += d1 + d2
    total = sum1 + sum2
    return total


def cardType(n):

    # Set variables for first and second digits, and the total returned from luhn's
    fnum = 0
    snum = 0
    l = luhn(n)

    # Shorten number until we are left with the first two digits
    while n > 100:
        n //= 10

    fnum = n // 10
    snum = n % 10

    # Determine card type by starting 2 digits
    if l % 10 == 0 and fnum == 4:
        return "VISA"
    elif l % 10 == 0 and fnum == 5 and (snum > 0 and snum < 6):
        return "MASTERCARD"
    elif l % 10 == 0 and fnum == 3 and (snum == 4 or snum == 7):
        return "AMEX"
    else:
        return "INVALID"


if __name__ == "__main__":
    main()