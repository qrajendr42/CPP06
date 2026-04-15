#ifndef SCALARCONVERTER_HPP
#define SCALARCONVERTER_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <limits>
#include <iomanip>
#include <cmath>
#include <cerrno>

class ScalarConverter
{
    private:

        enum LiteralType
        {
            TYPE_INVALID,
            TYPE_CHAR,
            TYPE_INT,
            TYPE_FLOAT,
            TYPE_DOUBLE,
            TYPE_PSEUDO_FLOAT,
            TYPE_PSEUDO_DOUBLE
        };

        struct ParsedValue
        {
            LiteralType type;
            double      number;
            bool        isNanOrInf;
        };

        ScalarConverter();
        ScalarConverter(const ScalarConverter &other);
        ScalarConverter &operator=(const ScalarConverter &other);
        ~ScalarConverter();

        static bool         isPseudoFloat(const std::string &s);
        static bool         isPseudoDouble(const std::string &s);
        static bool         isIntLiteral(const std::string &s);
        static bool         isFloatLiteral(const std::string &s);
        static bool         isDoubleLiteral(const std::string &s);
        static LiteralType  detectType(const std::string &s);

        static ParsedValue  parseLiteral(const std::string &literal);

        static void         printChar(const ParsedValue &v);
        static void         printInt(const ParsedValue &v);
        static void         printFloat(const ParsedValue &v);
        static void         printDouble(const ParsedValue &v);
    
    public:
        static void convert(const std::string &literal);
};

#endif