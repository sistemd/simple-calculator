#include "instruction_element.h"
#include "expression.h"
#include <iostream>

template <class Istream>
std::string Read(Istream& input)
{
    std::string result { };
    input >> result;
    
    return result;
}

int main() 
{
    using namespace Calc;
    
    try
    {
        const OperatorMap signToOperator
        {
            { "+", { 0, Plus { } } },
            { "*", { 1, Multiplies { } } },
            { "/", { 1, Divides { } } },
            { "^", { 2, Powers { } } }
        }; // Maybe rooting? Syntax? Add brackets first.
        
        // TODO Bracket support!
        // TODO Use regexes for stripping whitespace and checking digits
        // TODO Strip whitespace!
        
        // NOTE Fix your kdevelop launches
        
        // NOTE -1-1 results in 0, as well as 0-1-1
        // What might fix this is adding brackets, and
        // after that for every negative number -x translating 
        // -x into +(0-x)
        
        // Every open bracket increases the priority level addition
        // by the maximum that exists plus 1.
        // Every closed bracket decreases the priority level
        // addition by the same amount
        
        while (true)
        {
            try
            {
                auto exprStr = Read(std::cin);
                Prepare(exprStr);
                
                auto translated = TranslateString(exprStr, signToOperator);
                auto root = CreateTree(translated,
                                    signToOperator);
                
                std::cout << " = " << root->Result() << '\n';
            }
            catch (InvalidIdentifierException& e)
            {
                std::cout << e.what() << '\n';
            }
            catch (...)
            {
                throw;
            }
        }
    }
    catch (std::bad_alloc&)
    {
        std::cout << "Error: unhandleable bad allocation!\n";
        return 1;
    }
    catch (...)
    {
        std::cout << "Error: an error has occured.\n";
        return 1;
    }
    
    return 0;
}
