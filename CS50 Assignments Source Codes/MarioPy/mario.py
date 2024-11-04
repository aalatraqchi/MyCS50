from cs50 import get_int

# Prompt user for height using modified while loop, equivalent to do while in C

while True:
    height = get_int("Height: ")
    if height >= 1 and height <= 8:
        break

# Iterate through every row
for i in range(height):

    # For each row, create the spaces for the left side pyramid
    for j in range(height - i, 1, -1):
        print(" ", end="")

    # For each row, add a hash
    for k in range(i + 1):
        print("#", end="")

    # Print the spaces in middle that will split the pyramids
    for l in range(2):
        print(" ", end="")

    # Print hashes on right side of spaces
    for m in range(i + 1):
        print("#", end="")
    print("")