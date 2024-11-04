-- I started off by getting the description from crime_scene_reports that occured in July of 2021 on Humphrey Street
SELECT description
FROM crime_scene_reports
    WHERE month = 7
    AND year = 2021
    AND street = "Humphrey Street";

-- The descriptions mentioned it happening at the bakery with three witnesses, so I checked the interview transcripts
SELECT name, transcript
FROM interviews
    WHERE year = 2021
    AND month = 7
    AND day = 28;

/* All three witnesses gave different information, I decided to search
the atm data first to get the information of the people who withdrew money
on the day of the crime, and match their license plates to the ones in the bakery security logs*/
SELECT name, phone_number, passport_number, license_plate
FROM people, bank_accounts
    WHERE people.id = bank_accounts.person_id
    AND account_number IN (SELECT account_number
        FROM atm_transactions
            WHERE year = 2021
            AND month = 7
            AND day = 28
            AND atm_location = "Leggett Street"
            AND transaction_type = "withdraw")
    AND license_plate IN (SELECT license_plate
        FROM bakery_security_logs
            WHERE year = 2021
            AND month = 7
            AND day = 28
            AND hour LIKE 10
            AND minute <= 25
            AND activity = "exit");

/* I then decided to search for the earliest flight from Fiftyville on July 29th then
get its id to try and match its passengers with the potential suspects*/
SELECT flights.id, destination_airport_id, hour
FROM flights, airports
    WHERE flights.origin_airport_id = airports.id
    AND origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville")
    AND year = 2021
    AND month = 7
    AND day = 29
    ORDER BY hour ASC, minute ASC

-- Flight id 36 is the earliest and it is headed to destination airport 4. Now get the airport info.
SELECT abbreviation, full_name, city
FROM airports, flights
    WHERE airports.id = flights.destination_airport_id
    AND destination_airport_id = 4

/* Now we know the suspect is heading to LaGuardia Airport (LGA) in New York City.
Search for the passengers that flight and try to match their passports with the potential suspects */
SELECT DISTINCT(passport_number), seat
FROM passengers, flights
    WHERE flight_id = 36
    AND passport_number IN (SELECT passport_number
        FROM people, bank_accounts
            WHERE people.id = bank_accounts.person_id
            AND account_number IN (SELECT account_number
                FROM atm_transactions
                WHERE year = 2021
                AND month = 7
                AND day = 28
                AND atm_location = "Leggett Street"
                AND transaction_type = "withdraw")
            AND license_plate IN (SELECT license_plate
                FROM bakery_security_logs
                WHERE year = 2021
                AND month = 7
                AND day = 28
                AND hour LIKE 10
                AND minute <= 25
                AND activity = "exit"));

/* That has narrowed our search to Bruce and Luca now we can check
phone_calls to see if any receiver got phone calls lasting less than a minute
from any of the suspects */
SELECT caller, receiver, duration
FROM phone_calls
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60
    ORDER BY duration ASC;

/* The suspect is Bruce clearly based on the phone calls with a duration of less than
a minute, so now we can get his receiver's information */
SELECT *
FROM people
    WHERE phone_number = "(375) 555-8161"