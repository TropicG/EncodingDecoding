#include <iostream>
using namespace std;

const int helperCode_size = 15;
const int helperMessage_size = 1100;
const int Message_size = 1000;

//finding the size of given string
int sizeOfStr(const char* str) {
	int i = 0;
	while (str[i] != '\0') {// adding to i until we meeat \0
		i++;
	}
	return i;
}

//converting from upper to lowercase
void charToLower(char& ch) {
	if (ch <= 'Z' && ch >= 'A') {
		ch -= ('Z' - 'z');
	}
}

//converting string from lower to uppercase
void stringToLower(char* string) {
	int size = sizeOfStr(string);
	for (int i = 0; i < size; i++) {
		charToLower(string[i]);
	}
}


//copies the characters "from" to "to"
void strcopy(char* to, const int& size ,const char* from) {
	for (int i = 0; i < size; i++) {
		to[i] = from[i];
	}
	//adding turminating null at the end
	to[size] = '\0';
}

//comparing strings
bool strcompare(const char* string1, const char* string2) {
	int size1 = sizeOfStr(string1);
	int size2 = sizeOfStr(string2);
	if (size1 > size2 || size2 > size1) return false;
	if (size1 == size2) {
		for (int i = 0; i < size1; i++) {
			if (string1[i] != string2[i]) return false;
		}
	}
	return true;
}

//returns the crypted code at position
const char* returnCryptedChar(const int& index,  char** allCodes) {
	return allCodes[index];
}

//returns the character at position
char& returnSymbol(const int& index, char* allCharacters) {
	return allCharacters[index];
}

//looking whether the character is in the table for chracters
bool isCharInTable(char* allCharacters, const char& ch, const int& characters) {
	for (int i = 0; i < characters; i++) {
		if (allCharacters[i] == ch) {
			return true;
		}
	}
	return false;
}

//looking whether the code is in the table for codes
bool isCodeInTable(char** allCodes, char* code, const int& characters) {
	for (int i = 0; i < characters; i++) {
		char* string = allCodes[i];
		if (strcompare(code,string)) {
			return true;
		}
	}
	return false;
}

//returns the location of the given code in the table for codes
int returnLocationOfCode(const char* code, char** allCode, const int& characters) {
	int loc = 0;
	for (int i = 0; i < characters; i++) {
		//string points to the code at index i in allCodes, then compare himself with the code we are searching in the table
		char* string = allCode[i];
		if (strcompare(code,string)) {
			loc = i;
			break;
		}
	}
	return loc;
}

//returns the location of the given character in the table for characters
int returnLocationOfCharacter(const char& ch,  char* allCharacters, const int& characters) {
	int loc = 0;
	for (int i = 0; i < characters; i++) {
		if (ch == allCharacters[i]) {
			loc = i;
			break;
		}
	}
	return loc;
}

//appending characters to a given string, useful when we decrypting code
void append(int& size, const char& symbol, char* togheter) {
	togheter[size] = symbol;
	togheter[size +1] = '\0';
	size++;
}


//clearing a given codee (fulling it with nullterminated characters)
void clear(char* code,const int& size) {
	for (int i = 0; i < size; i++) {
		code[i] = '\0';
	}
}

//checks wheter the given code is empty or not, usefull in decrypting
bool empty(const char* code) {
	int size_code = sizeOfStr(code);
	for (int i = 0; i < size_code; i++) {
		if (code[i] != '\0') return true;
	}
	return false;
}

//checking is the character ch a letter from the alphabot or a symbol
bool isNotLetter(const char& ch) {
	if ((int)ch > 122 || (int)ch < 97) {
		return true;
	}
	return false;
}

char* decryptedMessage(const int& characters, char* allCharacters, char** allCodes, const char* cryptedMessage) {

	//here we will store the Decrypted message with maximum memory allocation
	int size = sizeOfStr(cryptedMessage);
	char* decryptedMessage_fornow = new char[size+1];
	if (decryptedMessage_fornow == nullptr) {
		cout << "Unsuccsesfull allocation";
		delete[] decryptedMessage_fornow;
		return nullptr;
	}

	//the size we will be using to follow how many characters are inputted in DecryptedMessage
	int sizeOfDecrypted = 0;

	//here will be store array that we will append to to see whether this code is in the table
	char* code = new char[helperMessage_size+1];
	if (code == nullptr) {
		cout << "Unsuccsesfull allocation";
		delete[] decryptedMessage_fornow;
		delete[] code;
		return nullptr;
	}

	int size_code = 0;
	code[size_code] = '\0';

	//starts the cycle with the first character in cryptedMessage
	for (int i = 0; i < size; i++) {
		//if the i character is whitespace add it to decryptedMessage_fornow and increase its size
		if (cryptedMessage[i] == ' ') {
			decryptedMessage_fornow[sizeOfDecrypted] = cryptedMessage[i];
			sizeOfDecrypted++;
		}

		//checks the code if is it not in the table and its not a letter
		//by not a letter i mean a symbol of sort of # # @ ! ? ><<>
		if (!isCodeInTable(allCodes, code, characters) && isNotLetter(cryptedMessage[i])) {
			decryptedMessage_fornow[sizeOfDecrypted] = cryptedMessage[i];
			sizeOfDecrypted++;
		}

	    //checks the code is it in allCodes table and is character at index i is not whitespace nor different symbol
		//then appends to code the i index character
		if (!isCodeInTable(allCodes, code, characters) && cryptedMessage[i]!= ' ' && !isNotLetter(cryptedMessage[i])) {
			append(size_code, cryptedMessage[i], code);
		}

		//if code is found in allCodes then search the character for this code and adds it into decryptedMessage_fornow
		if (isCodeInTable(allCodes, code, characters)) {
			int loc = returnLocationOfCode(code, allCodes, characters);
			char symbol = returnSymbol(loc, allCharacters);

			decryptedMessage_fornow[sizeOfDecrypted] = symbol;
			sizeOfDecrypted++;

		//clears the code so we can store one by one until we meet another code in 
			clear(code,size_code);
			size_code = 0;
		}

		//when code has over 10 characters mean there was no code found in allCodes table
		//and outputs bad string to the screen
		if (sizeOfStr(code) > 10) {
			char badstring[] = "bad string";
			char* decryptedMessage = new char[sizeOfStr(badstring) + 1];
			if (decryptedMessage == nullptr) {
				cout << "Unsuccsesfull allocation";
				delete[] decryptedMessage_fornow;
				delete[] decryptedMessage;
				delete[] code;
				return nullptr;
			}
			strcopy(decryptedMessage, sizeOfStr(badstring) + 1, badstring);

			delete[] code;
			delete[] decryptedMessage_fornow;
			return decryptedMessage;
		}
	}
	//if the upper cycle is done checks is code is empty
	//if not empty means that in cryptedMessage there were no codes met in allCodes
	if (empty(code)) {
		const char* badstring = "bad string";
		char* decryptedMessage = new char[sizeOfStr(badstring)+1];
		if (decryptedMessage == nullptr) {
			cout << "Unsuccsesfull allocation";
			delete[] decryptedMessage_fornow;
			delete[] decryptedMessage;
			delete[] code;
			return nullptr;
		}

		strcopy(decryptedMessage, sizeOfStr(badstring), badstring);

		delete[] code;
		delete[] decryptedMessage_fornow;
		return decryptedMessage;
	}

	//creating new array with accurate size and copy the characters in decryptedMessage_fornow to decryptedMessage
	char* decryptedMessage = new char[sizeOfDecrypted + 1];
	if (decryptedMessage == nullptr) {
		cout << "Unsuccsesfull allocation";
		delete[] decryptedMessage_fornow;
		delete[] decryptedMessage;
		delete[] code;
		return nullptr;
	}

	strcopy(decryptedMessage, sizeOfDecrypted, decryptedMessage_fornow);
	delete[] decryptedMessage_fornow;
	delete[] code;
	return decryptedMessage;

}

//algorithm for crypting messeage
char* cryptedMessage(const int& characters, char* allCharacters, char** allCodes, const char* message) {

	//here we will store the crypted message with maximum memory allocation
	//the message may containg characters each of them whom might have 10 characters code
	int size = sizeOfStr(message);
	char* CryptedMessage_fornow = new char[(size * 10)+1];
	if (CryptedMessage_fornow == nullptr) {
		cout << "Unsuccsesfull allocation";
		delete[] CryptedMessage_fornow;
		return nullptr;
	}

	//the size we will be using to follow how many characters are inputted in cryptedMessage
	int sizeOfCrypted = 0;
	
	//starts the cycle with the first character in message
	for (int i = 0; i < size; i++) {

		//if the first character is whitespace it just adds whitespace to the cryptedMessage and increases size
		if (message[i] == ' ') {
			CryptedMessage_fornow[sizeOfCrypted] = message[i];
			sizeOfCrypted++;
		}

		//looks the character at i index whether is in the table allCharacters and if false just adds this character
		//to the cryptedMessage and increases size
		if (!isCharInTable(allCharacters, message[i], characters)) {
			CryptedMessage_fornow[sizeOfCrypted] = message[i];
			sizeOfCrypted++;

		}

		//looks the character at i index whether is in the table allCharacters and if true searches the number of the character
		//in allCharacters and returns the code with the same index in allCodes
		if (isCharInTable(allCharacters, message[i], characters)) {
			int loc = returnLocationOfCharacter(message[i], allCharacters, characters);
			//here we store the found cide
			const char* helper = returnCryptedChar(loc, allCodes);
			int size_helper = strlen(helper);
			//cycle to input the given code until it ends
			for (int j = 0; j < size_helper;j++) {
				CryptedMessage_fornow[sizeOfCrypted] = helper[j];
				sizeOfCrypted++;
			}
		}
	}

	//after the cycle is done creates new dynamic array with accurate memory for the crypted message
	char* CryptedMessage = new char[sizeOfCrypted+1];
	if (CryptedMessage == nullptr) {
		cout << "Unsuccsesfull allocation";
		delete[] CryptedMessage_fornow;
		delete[] CryptedMessage;
		return nullptr;
	}
	//copies the characters from cryptedMessage_fornow to cryptedMessage
	strcopy(CryptedMessage, sizeOfCrypted, CryptedMessage_fornow);

	delete[] CryptedMessage_fornow;

	return CryptedMessage;
}

int main() {

	int characters=0, decryptedMessages=0, cryptedMessages=0;
	//if user inputs character program ends
	if (!(cin >> characters)) {
		std::cout << "You have to enter number not character";
		return -1;
	}

	//allocating memory for all the codes and characters
	char* allCharacters = new char[characters+1];
	char** allCodes= new char*[characters+1];
	if (allCharacters == nullptr || allCodes == nullptr) {
		cout << "Unsuccsseful allocation of memory!";
		delete[] allCharacters;
		delete[] allCodes;
		return -1;
	}



	//cycle to input all the characters and their codes
	for (int i = 0; i < characters; i++) {

		//using helper_code to store a code and check its validity
	    char helper_code[helperCode_size];
		cin >> allCharacters[i] >> helper_code;

		//converting char and string
		charToLower(allCharacters[i]);
		stringToLower(helper_code);

		//if it typed something besides letter
		if (isNotLetter(allCharacters[i])) {
			std::cout << "You have to enter a letter from the alphabet not another character";
			for (int i = 0; i < characters; i++) {
				delete[] allCodes[i];
			}
			delete[] allCodes;
			delete[] allCharacters;
			return -1;
		}
        
		//error when the size helper_code is above 10 
		if (sizeOfStr(helper_code) > 10) { 
			std::cout << "Your code was too long";
			for (int i = 0; i < characters; i++) {
				delete[] allCodes[i];
			}
			delete[] allCodes;
			delete[] allCharacters;
			return -1;

		}

		//initialzing
		allCodes[i] = new char[sizeOfStr(helper_code) + 1];
		if (allCodes[i] == nullptr) {
			std::cout << "Unsuccsessful allocation";
			for (int i = 0; i < characters; i++) {
				delete[] allCodes[i];
			}
			delete[] allCodes;
			delete[] allCharacters;
			return -1;
		}
		strcopy(allCodes[i],sizeOfStr(helper_code), helper_code);
	}

	//if typed some character other than number
	if (!(cin >> decryptedMessages)) {
		std::cout << "You have to enter number not character";
		for (int i = 0; i < characters; i++) {
			delete[] allCodes[i];
		}
		delete[] allCodes;
		delete[] allCharacters;
		return -1;
	}

	//declaring memory for all the decryptedmessages
	char** allDecryptedMessages = new char*[decryptedMessages];
	char* helperMessage = new char[helperMessage_size];
	if (allDecryptedMessages == nullptr || helperMessage == nullptr) {
		std::cout << "Unsuccsessful allocation";
		for (int i = 0; i < characters; i++) {
			delete[] allCodes[i];
		}
		delete[] allCodes;
		delete[] allCharacters;
		delete[] helperMessage;
		delete[] allDecryptedMessages;
		return -1;
	}

	//using this to suck the incoming new line from entering question
	char garbage[3] = { '\0' };
	cin.getline(garbage, 2, '\n');

	for (int i = 0; i < decryptedMessages; i++) {
		cin.getline(helperMessage, 1100);
		stringToLower(helperMessage);
		//if message is above 1000 characters is error
		if (sizeOfStr(helperMessage) > 1000) {

			std::cout << "You have entered to long sentence";
			for (int i = 0; i < characters; i++) {
				delete[] allCodes[i];
			}
			delete[] allCodes;
			delete[] allCharacters;
			
			for (int i = 0; i < decryptedMessages; i++) {
				delete[] allDecryptedMessages[i];
			}
			delete[] allDecryptedMessages;
			delete[] helperMessage;
			return -1;
		}

		//initializng
		allDecryptedMessages[i] = new char[sizeOfStr(helperMessage)+1];
		if (allDecryptedMessages[i] == nullptr) {
			std::cout << "Unsuccssesful allocation";
			for (int i = 0; i < characters; i++) {
				delete[] allCodes[i];
			}
			delete[] allCodes;
			delete[] allCharacters;

			for (int i = 0; i < decryptedMessages; i++) {
				delete[] allDecryptedMessages[i];
			}
			delete[] allDecryptedMessages;
			delete[] helperMessage;
			return -1;
		}
		strcopy(allDecryptedMessages[i], sizeOfStr(helperMessage), helperMessage);

		//clearing helperMessage to use it again
		clear(helperMessage,helperMessage_size);
	}
	
	//if typed some character other than number
	if (!(cin >> cryptedMessages)) {
		std::cout << "You have to enter a number not character";
		for (int i = 0; i < characters; i++) {
			delete[] allCodes[i];
		}
		delete[] allCodes;
		delete[] allCharacters;

		for (int i = 0; i < decryptedMessages; i++) {
			delete[] allDecryptedMessages[i];
		}
		delete[] allDecryptedMessages;
		delete[] helperMessage;
		return -1;
	}

	//initializng meemory for cryptedMessages
	char** allCryptedMessages = new char* [cryptedMessages];
	if (allCryptedMessages == nullptr) {
		std::cout << "Unssucsessful allocation";
		for (int i = 0; i < characters; i++) {
			delete[] allCodes[i];
		}
		delete[] allCodes;
		delete[] allCharacters;

		for (int i = 0; i < decryptedMessages; i++) {
			delete[] allDecryptedMessages[i];
		}
		delete[] allCryptedMessages;
		delete[] allDecryptedMessages;
		delete[] helperMessage;
		return -1;
	}
	cin.getline(garbage, 2, '\n');

	for (int i = 0; i < cryptedMessages; i++) {
		cin.getline(helperMessage, 1100);
		stringToLower(helperMessage);
		//if message is above 1000 characters is error
		if (sizeOfStr(helperMessage) > 1000) {

			std::cout << "Your sentece was too long";
			for (int i = 0; i < characters; i++) {
				delete[] allCodes[i];
			}
			delete[] allCodes;
			delete[] allCharacters;

			for (int i = 0; i < decryptedMessages; i++) {
				delete[] allDecryptedMessages[i];
			}
			delete[] allDecryptedMessages;

			for (int i = 0; i < decryptedMessages; i++) {
				delete[] allCryptedMessages[i];
			}
			delete[] allCryptedMessages;
			delete[] helperMessage;
			return -1;
		}
		//initializng
		allCryptedMessages[i] = new char[sizeOfStr(helperMessage) + 1];
		if (allCryptedMessages[i] == nullptr) {
			std::cout << "Unssucsesful allocation";
			for (int i = 0; i < characters; i++) {
				delete[] allCodes[i];
			}
			delete[] allCodes;
			delete[] allCharacters;

			for (int i = 0; i < decryptedMessages; i++) {
				delete[] allDecryptedMessages[i];
			}
			delete[] allDecryptedMessages;

			for (int i = 0; i < decryptedMessages; i++) {
				delete[] allCryptedMessages[i];
			}
			delete[] allCryptedMessages;
			delete[] helperMessage;
			return -1;

		}
		strcopy(allCryptedMessages[i], sizeOfStr(helperMessage), helperMessage);

		//clearing helperMessage to use it again
		clear(helperMessage,helperMessage_size);
	}

	//outputting crypted messages
	std::cout << endl;
	std::cout << "Crypted messeges:" << endl;
	for (int i = 0; i < decryptedMessages; i++) {
		const char* p_crypted = cryptedMessage(characters, allCharacters, allCodes, allDecryptedMessages[i]);
		std::cout << p_crypted <<endl;

		if (p_crypted == nullptr) {
			for (int i = 0; i < characters; i++) {
				delete[] allCodes[i];
			}
			delete[] allCodes;
			delete[] allCharacters;

			for (int i = 0; i < decryptedMessages; i++) {
				delete[] allDecryptedMessages[i];
			}
			delete[] allDecryptedMessages;

			for (int i = 0; i < cryptedMessages; i++) {
				delete[] allCryptedMessages[i];
			}
			delete[] allCryptedMessages;
			delete[] helperMessage;
			delete p_crypted;
			return 0;
		}

		//deletes memory allocated array returned from function
		delete[] p_crypted;
	}

	//outputting decrypted messages
	std::cout << endl;
	std::cout << "Decrypted messeges: " << endl;
	for (int i = 0; i < cryptedMessages; i++) {
		const char* p_decrypted = decryptedMessage(characters, allCharacters, allCodes, allCryptedMessages[i]);
		std::cout << p_decrypted << endl;

		if (p_decrypted==nullptr) {
			for (int i = 0; i < characters; i++) {
				delete[] allCodes[i];
			}
			delete[] allCodes;
			delete[] allCharacters;

			for (int i = 0; i < decryptedMessages; i++) {
				delete[] allDecryptedMessages[i];
			}
			delete[] allDecryptedMessages;

			for (int i = 0; i < cryptedMessages; i++) {
				delete[] allCryptedMessages[i];
			}
			delete[] allCryptedMessages;
			delete[] helperMessage;
			delete p_decrypted;
			return 0;
		}

		//deletes memory allocated array returned from function
		delete[] p_decrypted;
	}


	//deleting everything
	for (int i = 0; i < characters; i++) {
		delete[] allCodes[i];
	}
	delete[] allCodes;
	delete[] allCharacters;

	for (int i = 0; i < decryptedMessages; i++) {
		delete[] allDecryptedMessages[i];
	}
	delete[] allDecryptedMessages;

	for (int i = 0; i < cryptedMessages; i++) {
		delete[] allCryptedMessages[i];
	}
	delete[] allCryptedMessages;
	delete[] helperMessage;
	return 0;
}