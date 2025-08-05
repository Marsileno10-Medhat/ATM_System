# ATM System Project

This project is a C++ console-based ATM system designed to allow clients to perform basic banking operations such as withdrawals, deposits, and balance checks. The system authenticates users via account number and PIN code, with client data stored in a text file (`Clients_data_file.txt`). Below is an overview of the system's functionality and implementation details for version 1.0.

## Version 1.0

The initial version of the ATM system provides a client-facing interface for performing transactions. Key features include:

- **Login System**:
  - Clients log in using their account number and PIN code.
  - Authentication is validated against data stored in `Clients_data_file.txt`.
  - Failed login attempts prompt the user to retry until successful.

- **Main Menu**:
  - **Quick Withdraw**: Allows clients to withdraw predefined amounts (20, 50, 100, 200, 400, 600, 800, 1000) with balance validation.
  - **Normal Withdraw**: Enables withdrawal of any amount (in multiples of 5) that does not exceed the account balance.
  - **Deposit**: Allows clients to deposit a positive amount into their account.
  - **Check Balance**: Displays the current account balance.
  - **Logout**: Returns to the login screen.

- **Data Storage**:
  - Client data is stored in `Clients_data_file.txt` with fields (Account Number, PIN Code, Client Name, Phone Number, Account Balance) separated by a delimiter (`#//#`).
  - Data is loaded into a `vector<stClient>` for in-memory operations and saved back to the file after transactions.

- **Transaction Features**:
  - All transactions (withdrawals and deposits) require confirmation (Y/N) to proceed.
  - Quick withdrawals are restricted to predefined amounts, with balance checks to prevent overdrafts.
  - Normal withdrawals enforce amounts in multiples of 5 and validate against the account balance.
  - Deposits accept any positive amount and update the balance immediately.
  - Transaction results are saved to the file, ensuring data persistence.

- **Implementation Details**:
  - Uses a `struct stClient` to store client information (AccountNumber, PinCode, ClientName, PhoneNumber, AccountBalance).
  - Employs standard C++ libraries (`iostream`, `string`, `iomanip`, `vector`, `fstream`, `limits`) for input/output, file handling, and formatting.
  - Input validation ensures robust handling of user inputs (e.g., checking for valid numeric inputs and correct menu choices).
  - The system uses enums (`enATM` and `enQuickWithdraw`) to manage menu options and quick withdrawal amounts.
  - A global `CurrentClient` variable tracks the logged-in client's data.

### Usage
- **Prerequisites**:
  - A C++ compiler (e.g., g++).
  - Ensure `Clients_data_file.txt` is in the same directory as the executable, with data in the format: `AccountNumber#//#PinCode#//#ClientName#//#PhoneNumber#//#AccountBalance`.
- **Steps**:
  1. Compile the code: `g++ ATM_System1.0.cpp -o ATM_System`.
  2. Run the executable: `./ATM_System`.
  3. Log in using a valid account number and PIN code.
  4. Navigate the main menu to perform transactions.
  5. Select "Logout" to return to the login screen.

### File Format
- **Clients_data_file.txt**: Stores client data with fields separated by `#//#`. Example:
