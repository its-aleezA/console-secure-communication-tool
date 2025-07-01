//header files
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <cmath>
#include <limits>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

//function to compute modular inverse of a number
int modInverse(int a, int m)
{
    //looping over all possible values of x from 1 to m-1
    for (int x = 1; x < m; ++x)
        if ((a * x) % m == 1)
            return x;
    return -1; //returning -1 if no modular inverse is found
}

//function to save a string to a file
void saveToFile(const string& filename, const string& data)
{
    ofstream file(filename);
    if (file.is_open())
    {
        file << data; //writing data to file
        file.close(); //closing the file
    }
    else
    {
        cout << "Error: Unable to save to file " << filename << endl;
    }
}

//function to read a string from file
string readFromFile(const string& filename)
{
    ifstream file(filename);
    string data, line;
    if (file.is_open())
    {
        //reading the file line-by-line
        while (getline(file, line)) 
            //appending each line to the data string
            data += line + "\n"; 
        //closing file
        file.close(); 
    }
    else
    {
        cout << "Error: Unable to read from file " << filename << endl;
    }
    //returning data string
    return data; 
}

//function to save a vector to a file
void saveVectorToFile(const string& filename, const vector<int>& data)
{
    ofstream file(filename);
    if (file.is_open())
    {
        for (int val : data)
        {
            //saving integers separated by spaces
            file << val << " "; 
        }
        file.close();
    }
}

//function to read a vector from file
vector<int> readVectorFromFile(const string& filename)
{
    ifstream file(filename);
    vector<int> data;
    int val;
    if (file.is_open())
    {
        while (file >> val)
        { 
            //reading integers
            data.push_back(val); 
        }
        file.close();
    }
    //returning the data vector
    return data; 
}

//caesar cipher encryption with given shift
string caesarEncrypt(const string& text, int a, int b)
{
    string result;
    for (char ch : text)
    {
        if (isprint(ch))
        {
            result += static_cast<char>(((a * (ch - 32) + b) % 95) + 32);
        }
        else
        {
            result += ch;
        }
    }
    return result;
}

//caesar cipher encryption with given shift
string caesarEncryptBreak(const string& text, int b)
{
    string result;
    //looping through each character in the input string
    for (char ch : text) 
    {
        //checking if the character is alphabetic
        if (isalpha(ch)) 
        {
            //determining base
            char base = islower(ch) ? 'a' : 'A'; 
            //applying shift
            result += static_cast<char>((ch - base + b) % 26 + base); 
        }
        else
        {
            //leaving non-alphabetic characters unchanged
            result += ch; 
        }
    }
    //returning encrypted text
    return result; 
}

//caesar cipher decryption with given shift
string caesarDecrypt(const string& cipher, int a, int b)
{
    int aInv = modInverse(a, 95);
    if (aInv == -1) throw invalid_argument("Invalid 'a': Not invertible in mod 95.");
    string result;
    for (char ch : cipher)
    {
        if (isprint(ch))
        {
            result += static_cast<char>((aInv * ((ch - 32) - b + 95)) % 95 + 32);
        } else {
            result += ch;
        }
    }
    return result;
}

//calculating the frequency of each letter in the text
map<char, double> calculateFrequency(const string& text)
{
    map<char, double> freqMap;
    int totalLetters = 0;

    //counting occurrences of each letter (case insensitive)
    for (char ch : text)
    {
        if (isalpha(ch))
        {
            freqMap[tolower(ch)]++;
            totalLetters++;
        }
    }

    //converting counts into percentages
    for (auto& pair : freqMap)
    {
        pair.second = (pair.second / totalLetters) * 100.0;
    }

    //returning the frequency map
    return freqMap; 
}

//reference frequencies of each English letter
const map<char, double> englishFrequencies =
{
    {'a', 8.2}, {'b', 1.5}, {'c', 2.8}, {'d', 4.3}, {'e', 13.0}, {'f', 2.2}, {'g', 2.0},
    {'h', 6.1}, {'i', 7.0}, {'j', 0.15}, {'k', 0.77}, {'l', 4.0}, {'m', 2.4}, {'n', 6.7},
    {'o', 7.5}, {'p', 1.9}, {'q', 0.095}, {'r', 6.0}, {'s', 6.3}, {'t', 9.1}, {'u', 2.8},
    {'v', 0.98}, {'w', 2.4}, {'x', 0.15}, {'y', 2.0}, {'z', 0.074}
};

//calculating the chi-squared statistic to compare letter frequencies
double calculateChiSquared(const map<char, double>& textFreq, const map<char, double>& referenceFreq)
{
    double chiSquared = 0.0;

    // comparing observed frequencies to expected frequencies
    for (auto& pair : referenceFreq)
    {
        char letter = pair.first;
        double expectedFreq = pair.second;
        double observedFreq = textFreq.count(letter) ? textFreq.at(letter) : 0.0;

        // if the letter is not in the text, it has 0 frequency
        chiSquared += pow(observedFreq - expectedFreq, 2) / expectedFreq;
    }
    //returning the chi-squared value
    return chiSquared; 
}

//decrypting a Caesar cipher using a specific shift
string decryptWithShift(const string& cipher, int shift)
{
    string result;
    for (char ch : cipher)
    {
        if (isalpha(ch))
        {
            //determining base
            char base = islower(ch) ? 'a' : 'A'; 
            //reversing the shift
            result += static_cast<char>((ch - base - shift + 26) % 26 + base); 
        }
        else
        {
            //leaving non-alphabetic characters unchanged
            result += ch; 
        }
    }
    //returning the decrypted text
    return result; 
}

//breaking a Caesar cipher using frequency analysis
string breakCaesarCipher(const string& cipher)
{
    //initializing minimum chi-squared value
    double minChiSquared = numeric_limits<double>::max(); 
    int bestShift = 0;

    //testing all possible shifts (0 to 25)
    for (int shift = 0; shift < 26; ++shift)
    {
        //decrypting with the current shift
        string decryptedText = decryptWithShift(cipher, shift); 
        //calculating frequency
        map<char, double> textFreq = calculateFrequency(decryptedText); 
        //computing chi-squared
        double chiSquared = calculateChiSquared(textFreq, englishFrequencies); 

        //updating the best shift if a lower chi-squared is found
        if (chiSquared < minChiSquared)
        {
            minChiSquared = chiSquared;
            bestShift = shift;
        }
    }

    // decrypt with the best shift found
    return decryptWithShift(cipher, bestShift);
}

//function to check if a number is a prime number
bool isPrime(int num)
{
    //for zero or negative numbers
    if (num <= 1) return false; 

    //checking divisiblity by any number from 2 to sqrt(num)
    for (int i = 2; i <= sqrt(num); ++i)
        //if number is divisible
        if (num % i == 0) 
            //not prime
            return false; 
    //if no divisors are found
    return true; 
}

//function to compute the greatest common divisor of two numbers
int gcd(int a, int b)
{
    return b == 0 ? a : gcd(b, a % b);
}

//function to calculate public and private key for the RSA algorithm
pair<int, int> generateKeys(int& privateKey, int& publicKey, int& n)
{
    //seeding the random number generator with current time
    srand(time(nullptr)); 

    int p, q;
    //generating prime numbers between 50 and 150
    do {
        //random prime number 
        p = rand() % 100 + 50; 
    } 
    //until p is prime
    while (!isPrime(p)); 

    do {
        //random prime number 
        q = rand() % 100 + 50;
    } 
    //until q is prime and q != p
    while (!isPrime(q) || p == q); 
    
    //product of the two prime numbers
    n = p * q; 
    
    //product of one less than each of the two prime numbers
    int phi = (p - 1) * (q - 1); 

    do {
        //random number for public key
        publicKey = rand() % phi;   
    } 
    //until the key is coprime with Ï†(n)
    while (gcd(publicKey, phi) != 1);

    privateKey = modInverse(publicKey, phi);

    //returning p and q
    return {p, q};
}

//function to perform modular exponentiation
int modularExponentiation(int base, int exp, int mod)
{
    int result = 1;

    //ensuring base is within modulo range
    base = base % mod; 
    
    //performing exponentiation using squaring
    while (exp > 0)
    {
        //if exponent is odd
        if (exp % 2 == 1)
            result = (result * base) % mod;
        //dividing exponent by 2
        exp /= 2;
        //squaring the base
        base = (base * base) % mod;
    }
    //returning the final result
    return result;
}

//function to encrypt a string using RSA encryption
vector<int> rsaEncrypt(const string& text, int publicKey, int n)
{
    vector<int> cipher;
    
    //encrypt each character of the text using modular exponentiation
    for (char ch : text)
    {
        //encrypting ans storing result
        cipher.push_back(modularExponentiation(ch, publicKey, n));
    }
    //returning the encrypted text as a vector of integers
    return cipher;
}

//function to decrypt an RSA-encrypted cipher text
string rsaDecrypt(const vector<int>& cipher, int privateKey, int n)
{
    string result;
    
    //decrypt each value in the cipher using the private key and modulus
    for (int val : cipher)
    {
        //decrypt and append to result
        result += static_cast<char>(modularExponentiation(val, privateKey, n));
    }
    //retrunig the decrypted text
    return result;
}

//menu function to interact with the user
void menu()
{
    int choice;
    int lastShift = 0;
    int lastCoprime = 0;

    //starting a loop
    while (true)
    {
        //displaying menu options
        cout << "\nSecure Communication System\n";
        cout << "1. Encrypt using Caesar Cipher\n";
        cout << "2. Decrypt using Caesar Cipher with key\n";
        cout << "3. Break Caesar Cipher without key (Frequency Analysis)\n";
        cout << "4. Encrypt using RSA algorithm\n";
        cout << "5. Decrypt using RSA algorithm\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        //exiting program
        if (choice == 6) break; 

        //declaring input and output strings
        string input, output, output_break; 
        //declaring shift value 
        int a, b; 
        //declaring public key, private key and n 
        int publicKey, privateKey, n; 
        vector<int> rsaCipher;

        switch (choice)
        {
            case 1:
                //taking text to be encrypted as input
                cout << "Enter text to encrypt: "; 
                cin.ignore();
                getline(cin, input);

                //taking coprime to 95 as input
                cout << "Enter a (coprime to 95): "; 
                cin >> a;
                //checking whether input is a coprime of 95 or not
                while (gcd(a, 95) != 1)
                {
                    cout << "Invalid 'a'. Enter again: ";
                    cin >> a;
                }

                //taking shift as input
                cout << "Enter b: "; 
                cin >> b;

                lastShift = b;
                lastCoprime = a;

                //encrypting text using Caesar cipher method
                output = caesarEncrypt(input, a, b); 
                    
                //saving the encrypted string to text file
                saveToFile("encrypted_caesar.txt", output); 
                cout << "Encrypted text saved to file.\n";

                //encrypting text for frequency analysis
                output_break = caesarEncryptBreak(input, b);

                //saving encrypted text to file
                saveToFile("encrypted_caesar_break.txt", output_break); 
                
                break;

            case 2:
                //read encrypted text from file
                input = readFromFile("encrypted_caesar.txt"); 

                char useSameShift, useSameCoprime;
                
                //checking whether to use the previous coprime or not
                cout << "Do you want to use the same coprime (a) as encryption? (y/n): ";
                cin >> useSameCoprime;
                
                //to use previous coprime
                if ((useSameCoprime == 'y' || useSameCoprime == 'Y')) 
                {
                    //using the coprime stored during encryption
                    a = lastCoprime;                                        
                }
                //to use new coprime
                else 
                {
                    //taking new coprime as input
                    cout << "Enter a new a (coprime to 95): "; 
                    cin >> a;
                }
                
                //checking whether to use the previous shift or not
                cout << "Do you want to use the same shift (b) as encryption? (y/n): "; 
                cin >> useSameShift;
                //to use previous shift
                if ((useSameShift == 'y' || useSameShift == 'Y')) 
                {
                    //using the shift stored during encryption
                    b = lastShift;                                        
                }
                //to use new shift
                else 
                {
                    //taking new shift as input
                    cout << "Enter a new shift (b): "; 
                    cin >> b;
                }
                
                //decrypting using the same shift
                output = caesarDecrypt(input, a, b);  
                
                //displaying decrypted text
                cout << "Decrypted text using symmetric decryption: " << output << endl; 
                break;

            case 3:
                //reading encrypted text from the file
                input = readFromFile("encrypted_caesar_break.txt");
                
                //decrypting encrypted text using frequency analysis
                output = breakCaesarCipher(input);
                
                //displaying decrypted text
                cout << "Decrypted text (frequency analysis): " << output << endl;
                break;

            case 4:
                //taking text to be encrypted as input
                cout << "Enter text to encrypt: "; 
                cin.ignore();
                getline(cin, input);

                //generating keys for RSA encryption
                generateKeys(privateKey, publicKey, n); 

                //encrypting using RSA algorithm
                rsaCipher = rsaEncrypt(input, publicKey, n); 

                //saving the full cipher to file
                saveVectorToFile("encrypted_rsa.txt", rsaCipher); 
                cout << "Encrypted data saved to file.\n";

                //display private key and n
                cout << "Private key: " << privateKey << ", n: " << n << endl; 
                break;
            
            case 5:
                //reading the full cipher
                rsaCipher = readVectorFromFile("encrypted_rsa.txt"); 
                //if the file is empty
                if (rsaCipher.empty()) 
                {
                    cout << "Error: No cipher data found in file.\n";
                    break;
                }

                //taking private key as input
                cout << "Enter private key: "; 
                cin >> privateKey;
                //taking n as input
                cout << "Enter n: "; 
                cin >> n;

                //decrypting the vector
                output = rsaDecrypt(rsaCipher, privateKey, n); 
                //displaying decrypted text
                cout << "Decrypted text: " << output << endl; 
                break;

            default:
                //default output
                cout << "Invalid choice. Try again.\n"; 
        }
    }
}

//main function
int main()
{
    //calling menu function
    menu(); 
    
    return 0;
}
