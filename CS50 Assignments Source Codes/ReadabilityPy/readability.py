from cs50 import get_string


def main():

    # Prompt user for text
    text = get_string("Text: ")

    # Get letters, words, and sentences from functions
    letters = getLetters(text)
    words = getWords(text)
    sentences = getSentences(text)

    # Calculate Coleman-Liau index
    l = letters / words * 100
    s = sentences / words * 100

    cindex = round((.0588 * l) - (.296 * s) - 15.8)

    # Print out appropriate reading level based on cindex value
    if cindex >= 16:
        print("Grade 16+")
    elif cindex < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {cindex}")


def getLetters(text):

    # Iterate through every character of the text and count the letters
    letters = 0
    for c in text:
        if c.isalpha():
            letters += 1
    return letters


def getWords(text):

    # Iterate through every character and count is a word after every space or when it reaches the end of the sentence
    words = 0
    for c in text:
        if c == " ":
            words += 1
    words += 1 #  This accounts for the last word in the sentence
    return words


def getSentences(text):

    # Iterate through ever character and mark sentences when there are periods, exclamation marks, or question marks
    sentences = 0
    for c in text:
        if c == "!" or c == "?" or c == ".":
            sentences += 1
    return sentences


if __name__ == "__main__":
    main()