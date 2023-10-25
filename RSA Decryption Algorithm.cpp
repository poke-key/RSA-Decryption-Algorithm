#include <iostream>
#include <cmath>
#include <cctype>
#include <vector>
using namespace std;

/*declaration of functions used.*/
int largestPrimeFactor(int);
bool isPrime(int);
int gcd(int, int);
bool isRelativelyPrime(int, int);
bool isValidKey(int, int, int, int);
int calcModInverse(int, int);
int decryptMsg(int, int, int);
char decodetoChar(int);

int main() {
    //List of variables needed for the RSA
    int length;
    int msg;
    int e;
    int n;
    int q;
    int p;
    int phi;
    int d;

    cin >> e >> n >> length;

    //Populating the user input into the vectors
    if (!cin.good()) {
        cout << "Public key is not valid!";
        return 0;
    }
    /*use vectors to read and store the integers in the encrypted message and output the new decrypted message.*/
    vector<int> encryptedMsgs;
    vector<int> decryptedMsgs;

    for (int i = 0; i < length; ++i) {
        cin >> msg;
        if (!cin.good()) {
            cout << "Public key is not valid!";
            return 0;
        }
        encryptedMsgs.push_back(msg);
    }

    //Now we calculate p,q, phi(n), and d
    q = largestPrimeFactor(n);
    p = n / q;
    phi = (p - 1) * (q - 1);

    //Check if the public key is valid
    if (!isValidKey(q, p, e, phi)) {
        cout << "Public key is not valid!";
        return 0;
    }

    d = calcModInverse(e, phi);

    for (int i = 0; i < length; ++i) {
        decryptedMsgs.push_back(decryptMsg(encryptedMsgs.at(i), d, n));
    }

    //Print everything
    cout << p << " " << q << " " << phi << " " << d << "\n";
    for (int i = 0; i < length; ++i) {
        cout << decryptedMsgs.at(i) << " ";
    }
    cout << "\n";

    for (int i = 0; i < length; ++i) {
        cout << decodetoChar(decryptedMsgs.at(i));
    }
    return 0;
}

/// <summary>
/// calculates the largest prime factors, q and p
/// </summary>
/// <param name="n"></param>
/// <returns></returns>
int largestPrimeFactor(int n) {
    int largest_prime = -1;
    //if even, keep dividing until it becomes odd
    while (n % 2 == 0) {
        largest_prime = 2;
        n /= 2;
    }
    //brute force all numbers until a divisible number is found
    for (int i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            largest_prime = i;
            n /= i;
        }
    }
    if (n > 1) largest_prime = n;
    return largest_prime;
}

/*checks if number is prime*/
bool isPrime(int n) {
    if (n <= 1) return false;

    // Check from 2 to n-1
    for (int i = 2; i <= n / 2; i++)
        if (n % i == 0) return false;
    return true;
}

/*checks for whether gcd of parameters are 1, meaning relatively prime, thus returning a boolean value.*/
bool isRelativelyPrime(int a, int b) {
    if (gcd(a, b) == 1) return true;
    return false;
}
/*basic function that uses recursion to calculate the greatest common divisor.*/
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

/// <summary>
/// the values q and p should both be prime, and the values e and phi should have no gcd other than 1.
/// </summary>
/// <param name="q"></param>
/// <param name="p"></param>
/// <param name="e"></param>
/// <param name="phi"></param>
/// <returns></returns>
bool isValidKey(int q, int p, int e, int phi) {
    if (!isPrime(p)) 
        return false;
    if (!isPrime(q)) return false;
    if (!isRelativelyPrime(e, phi)) return false;
    if (p == q) 
        return false;
    return true;
}
/*function that is used to calculate d.*/
int calcModInverse(int e, int phi) {
    int m0 = phi, x = 1, y = 0;

    while (e > 1) {

        int quotient = e / phi; //find quotient of the numbers
        int tempValue = phi;
        phi = e % phi;      // make remainder m, swap
        e = tempValue;

        tempValue = y;
        y = x - (quotient * y);
        x = tempValue;
    }
    return (x % m0 + m0) % m0;
}

/*decrypts the message using the power rule*/
int decryptMsg(int x, int n, int m) {
    if (n == 0) return 1;   //base case

    if (n % 2 == 0) {       //if even
        int new_power = (int)pow(decryptMsg(x, n / 2, m), 2) % m; //recursively call but divide exponent by 2, then square with pow() function
        return new_power;
    }
    else {  //if odd
        int new_power = (int)pow(decryptMsg(x, (n - 1) / 2, m), 2) % m;    //same logic but subtract 1 from exponent before dividing by 2
        return (x * new_power) % m;
    }
}
/*takes cipher value as parameter, decodes decrypted value into a charr and returns it based on switch case statements for the parameter. Return breaks out of the switch-case statements*/
char decodetoChar(int c) {
    char new_c = c + 60;

    switch (c) {
        case 31:
            return ' ';
        case 32:
            return '"';
        case 33:
            return ',';
        case 34:
            return '.';
        case 35:
            return '\'';
        default:
            if (c >= 5 && c <= 30) return new_c;
            return '=';
        }
}