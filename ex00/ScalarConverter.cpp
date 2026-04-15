#include "ScalarConverter.hpp"

bool    ScalarConverter::isPseudoFloat(const std::string &s)
{
    return (s == "nanf" || s == "+inff" || s == "-inff");
}

bool    ScalarConverter::isPseudoDouble(const std::string &s)
{
    return (s == "nan" || s == "+inf" || s == "-inf");
}

bool    ScalarConverter::isIntLiteral(const std::string &s)
{
    if (s.empty())
        return false;

    size_t i = 0;
    if (s[i] == '+' || s[i] == '-')
        i++;
    if (i >= s.size())
        return (false);

    while (i < s.size())
    {
        if (std::isdigit(static_cast<unsigned char>(s[i])) == 0)
            return (false);
        i++;
    }
    return (true);
}

bool    ScalarConverter::isFloatLiteral(const std::string &s)
{
    if (s.size() < 2 || s[s.size() - 1] != 'f')
        return (false);
    
    std::string core = s.substr(0, s.size() - 1);
    size_t i = 0;
    bool hasDot = false;
    bool hasDigit = false;

    if (core[i] == '+' || core[i] == '-')
        i++;

    if (i >=core.size())
        return (false);

    while (i < core.size())
    {
        if (core[i] == '.')
        {
            if (hasDot)
                return (false);
            hasDot = true;
        }
        else if (std::isdigit(static_cast<unsigned char>(core[i])))
            hasDigit = true;
        else
            return (false);
        i++;
    }
    return (hasDot && hasDigit);
}

bool ScalarConverter::isDoubleLiteral(const std::string &s)
{
    if (s.empty())
        return (false);
    
    size_t i = 0;
    bool hasDot = false;
    bool hasDigit = false;

    if (s[i] == '+' || s[i] == '-')
        i++;
    
    if (i >= s.size())
        return (false);
    
    while (i < s.size())
    {
        if (s[i] == '.')
        {
            if (hasDot)
                return (false);
            hasDot = true;
        }
        else if (std::isdigit(static_cast<unsigned char>(s[i])))
            hasDigit = true;
        else
            return (false);
        i++;
    }
    return (hasDot && hasDigit);
}

ScalarConverter::LiteralType ScalarConverter::detectType(const std::string &s)
{
    if (isPseudoFloat(s))
        return TYPE_PSEUDO_FLOAT;
    if (isPseudoDouble(s))
        return TYPE_PSEUDO_DOUBLE;
    if (s.size() == 1 && std::isdigit(static_cast<unsigned char>(s[0])) == 0)
        return TYPE_CHAR;
    if (isIntLiteral(s))
        return TYPE_INT;
    if (isFloatLiteral(s))
        return TYPE_FLOAT;
    if (isDoubleLiteral(s))
        return TYPE_DOUBLE;
    return TYPE_INVALID;
}

ScalarConverter::ParsedValue ScalarConverter::parseLiteral(const std::string &literal)
{
    ParsedValue out;
    out.type = detectType(literal);
    out.number = 0.0;
    out.isNanOrInf = false;

    if (out.type == TYPE_CHAR)
        out.number = static_cast<double>(literal[0]);
    else if (out.type == TYPE_INT)
    {
        errno = 0;
        long value = std::strtol(literal.c_str(), NULL, 10);
        if (errno == ERANGE
            || value < std::numeric_limits<int>::min()
            || value > std::numeric_limits<int>::max())
            out.type = TYPE_INVALID;
        else
            out.number = static_cast<double>(value);
    }
    else if (out.type == TYPE_FLOAT)
    {
        std::string core = literal.substr(0, literal.size() - 1);
        out.number = std::strtod(core.c_str(), NULL);
    }
    else if (out.type == TYPE_DOUBLE)
        out.number = std::strtod(literal.c_str(), NULL);
    else if (out.type == TYPE_PSEUDO_FLOAT)
    {
        if (literal == "nanf")
            out.number = std::numeric_limits<double>::quiet_NaN();
        else if (literal == "+inff")
            out.number = std::numeric_limits<double>::infinity();
        else
            out.number = -std::numeric_limits<double>::infinity();
        out.isNanOrInf = true;
    }
    else if (out.type == TYPE_PSEUDO_DOUBLE)
    {
        if (literal == "nan")
            out.number = std::numeric_limits<double>::quiet_NaN();
        else if (literal == "+inf")
            out.number = std::numeric_limits<double>::infinity();
        else
            out.number = -std::numeric_limits<double>::infinity();
        out.isNanOrInf = true;
    }

    if (std::isnan(out.number) || std::isinf(out.number))
        out.isNanOrInf = true;

    return (out);
}

void    ScalarConverter::printChar(const ParsedValue &v)
{
    std::cout << "char: ";
    if (v.type == TYPE_INVALID || v.isNanOrInf || v.number < 0.0 || v.number > 127.0)
    {
        std::cout << "impossible\n";
        return ;
    }
    char c = static_cast<char>(v.number);
    if (!std::isprint(static_cast<unsigned char>(c)))
        std::cout << "Non displayable\n";
    else
        std::cout << "'" << c << "'\n";
}

void    ScalarConverter::printInt(const ParsedValue &v)
{
    std::cout << "int: ";
    if (v.type == TYPE_INVALID || v.isNanOrInf
        || v.number < static_cast<double>(std::numeric_limits<int>::min())
        || v.number > static_cast<double>(std::numeric_limits<int>::max()))
    {
        std::cout << "impossible\n";
        return ;
    }
    std::cout << static_cast<int>(v.number) << "\n";
}

void    ScalarConverter::printFloat(const ParsedValue &v)
{
    std::cout << "float: ";

    if (v.type == TYPE_INVALID)
    {
        std::cout << "impossible\n";
        return ;
    }

    if (std::isnan(v.number))
    {
        std::cout << "nanf\n";
        return ;
    }
    if (std::isinf(v.number))
    {
        if (v.number < 0)
            std::cout << "-inff\n";
        else
            std::cout << "+inff\n";
        return ;
    }

    float f = static_cast<float>(v.number);
    if (f == static_cast<int>(f))
        std::cout << std::fixed << std::setprecision(1) << f << "f\n";
    else
        std::cout << f << "f\n";
}

void    ScalarConverter::printDouble(const ParsedValue &v)
{
    std::cout << "double: ";

    if (v.type == TYPE_INVALID)
    {
        std::cout << "impossible\n";
        return ;
    }

    if (std::isnan(v.number))
    {
        std::cout << "nan\n";
        return ;
    }
    if (std::isinf(v.number))
    {
        if (v.number < 0)
            std::cout << "-inf\n";
        else
            std::cout << "+inf\n";
        return ;
    }

    double d = v.number;
    if (d == static_cast<int>(d))
        std::cout << std::fixed << std::setprecision(1) << d << "\n";
    else
        std::cout << d << "\n";
}

void    ScalarConverter::convert(const std::string &literal)
{
    ParsedValue parsed = parseLiteral(literal);

    printChar(parsed);
    printInt(parsed);
    printFloat(parsed);
    printDouble(parsed);
}

ScalarConverter::ScalarConverter()
{
}

ScalarConverter::ScalarConverter(const ScalarConverter &other)
{
    (void)other;
}

ScalarConverter &ScalarConverter::operator=(const ScalarConverter &other)
{
    (void)other;
    return *this;
}

ScalarConverter::~ScalarConverter()
{
}