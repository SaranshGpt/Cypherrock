Link to resume: https://drive.google.com/file/d/1c5MT7dGFQvYkqypSQOX3GTmKywDRN38C/view?usp=drive_link

Instructions to use the code:

    Compile the code and add two numbers as arguments
    Output will either give verification succeeded or verification failed, with the expected and actual sum of shares

The codebase turned out more haphazard than I would have liked due to numerous iterations and project structure changes as my understanding of the problem evolved. If I were to implement this again, these are the changes I would make:

 - Separating the processing of individual agents into separate threads to better emulate real-world environments. I avoided doing this here for the sake of simplicity.
 - Creating wrapper functions around trezor-firmware functions to improve code clarity
 - Use the bignum-based prime value used in the ECC curve for custom. I used my prime in my custom code as much as possible to make debugging easier and code less cumbersome.
 - Cleaner state management.
 - Removing some redundant code. There is not too much of it, but removing some that is there would still be nice
 - More structured memory management.
 - Creating an MPC-based verification for the shares generated. I know how that might be done, but it would've taken too long to implement.

Test cases are failing, as the earlier email indicated they might. Still, I tried my best to ensure that my logic is correct. I am assuming the failure is either due to a bug in the library itself or the result of me misunderstanding its functionality.

A copy of the above message is present in the repo's readme file
