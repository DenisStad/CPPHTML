//
//  StringUtil.h
//  Musee
//
//  Created by Denis Stadniczuk on 29/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef Musee_StringUtil_h
#define Musee_StringUtil_h

#include <vector>

std::vector<std::string> StringSplit(const std::string &source, const char *delimiter = " ", bool keepEmpty = false);
std::string toUpper(std::string string);

#endif
