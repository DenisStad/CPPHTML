//
//  StringUtil.c
//  Musee
//
//  Created by Denis Stadniczuk on 29/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#include "StringUtil.h"

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>

std::vector<std::string> StringSplit(const std::string &source, const char *delimiter, bool keepEmpty)
{
   std::vector<std::string> results;

   size_t prev = 0;
   size_t next = 0;

   while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
   {
      if (keepEmpty || (next - prev != 0))
      {
         results.push_back(source.substr(prev, next - prev));
      }
      prev = next + 1;
   }

   if (prev < source.size())
   {
      results.push_back(source.substr(prev));
   }

   return results;
}

std::string toUpper(std::string string)
{
   struct convert {
      void operator()(char& c) { c = toupper((unsigned char)c); }
   };

   for_each(string.begin(), string.end(), convert());

   return string;
}