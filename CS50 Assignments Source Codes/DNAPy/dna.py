import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py database.csv sequence.txt")

    dbFile = sys.argv[1]
    seqFile = sys.argv[2]

    # Read database file into a variable
    database = []

    with open(dbFile, "r") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            database.append(row)

    # Read DNA sequence file into a variable

    with open(seqFile, "r") as textfile:
        sequence = textfile.read()

    # Find longest match of each STR in DNA sequence

    # Store subsequences as a list, ignoring "name" and storing only everything after that
    subseq = list(database[0])[1:]

    totals = {}
    for sub in subseq:
        totals[sub] = longest_match(sequence, sub)

    # Check database for matching profiles

    for person in database:
        match = 0
        for sub in subseq:
            if int(person[sub]) == totals[sub]:
                match += 1

        # If the number of matches equals all of the subsequences then there is a match
        if match == len(subseq):
            print(person["name"])
            return

    # If it leaves the loop without finding a match, then there is no match
    print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()