#ifndef ACM_BOOKSTORE_UTILS
#define ACM_BOOKSTORE_UTILS

#include <iostream>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <unordered_map>
#include "BookstoreSystem.h"
#include "Exception.h"

void ReadLine(const std::string& str, std::string* buf);

void ValidateKeywordForSearch(const std::string& str);

bool JudgeKeyword(const std::string& str);

bool ValidateUserData1(const std::string& str);//UserID,Password,CurrentPassword,NewPassword

bool ValidateUserData2(const std::string& str);//Username

bool ValidateUserData3(const std::string& str);//Privilege

bool ValidateISBN(const std::string& str);

bool ValidateNameAuthor(const std::string& str);

bool ValidateKeyword(const std::string& str);

bool ValidateQuantity(const std::string& str);

bool ValidatePrice(const std::string& str);

bool ValidateCost(const std::string& str);

bool ValidateCount(const std::string& str);

void BookstoreWork();

void ExitSystem();

#endif