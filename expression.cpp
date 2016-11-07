#include "expression.h"
#include "instruction_element.h"
#include <boost/algorithm/string/replace.hpp>
#include <algorithm>
#include <functional>
#include <utility>
#include <regex>
#include <iostream>

namespace Calc
{
    BracketPair::BracketPair(const StringVector& original) noexcept
        : m_original { original }
    {
        m_result.first = std::find(original.cbegin(), original.cend(), "(");
        
        auto rfirst = std::make_reverse_iterator(original.cbegin());
        auto rlast = std::make_reverse_iterator(original.cend());
        
        m_result.second = (std::find(rfirst, rlast, ")") + 1).base();
    }
    
    bool BracketPair::None() const noexcept
    {
        return m_result.first == m_original.cend();
    }
    
    StringVector::const_iterator BracketPair::Start() const noexcept
    {
        return m_result.first;
    }
    
    StringVector::const_iterator BracketPair::Finish() const noexcept
    {
        return m_result.second;
    }
    
    OperatorMap::const_iterator MaxPriorityElement(
        OperatorMap::const_iterator first, 
        OperatorMap::const_iterator last,
        std::function<bool(int, int)> f)
    {
        return std::max_element(first, last,
            [&f](const auto& l, const auto& r)
            {
                return f(l.second.priority, r.second.priority);
            }
        );
    }
    
    bool IsNumberElement(char c) noexcept
    {
        switch(c)
        {
            case '-':
            case '.':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return true;
            default:
                return false;
        }
    }
    
    bool IsOperationElement(char c, const OperatorMap& map)
    {
        auto allElements = OrderedSigns(map);
        allElements.append("()");
        
        return std::find(allElements.cbegin(), allElements.cend(),
            c) != allElements.cend();
    }
    
    void Prepare(std::string& str) noexcept
    {
        if (str.front() == '-')
            str.insert(str.begin(), '0'); // Some edge case
        
        boost::replace_all(str, "(", "1*(");
        boost::replace_all(str, "-", "+-");
    }
    
    std::string OrderedSigns(OperatorMap map)
    {
        std::string result { };
        const auto size = map.size();
        
        for (std::size_t _ = 0; _ < size; ++_)
        {
            auto max = MaxPriorityElement(
                map.cbegin(), map.cend(),
                std::greater<int> { }
            );
            
            result.append(max->first);
            map.erase(max);
        }
        
        return result;
    }
    
    StringVector TranslateString(const std::string& str, 
                                 const OperatorMap& map)
    {
        StringVector result { "" };
        
        for (const auto& letter : str)
        {
            if (IsNumberElement(letter))
            {
                result.back().push_back(letter);
            }
            else if (IsOperationElement(letter, map))
            {
                result.push_back(std::string { letter });
                result.push_back("");
            }
            else
            {
                throw InvalidIdentifierException {
                    "Invalid identifier in expression: "
                    + std::string { letter } + '.'
                };
            }
        }
        
        result.erase(std::remove(result.begin(), result.end(), ""),
                     result.end());
        // The erase at the end is used to fix the cases
        // where we have brackets next to an operator.
        
        return result;
    }
    
    TreeElementPtr CreateTree(
        StringVector svec,
        const OperatorMap& map) // Sign to operator map
    {
        BracketPair brackets { svec };
        
        if (brackets.None()) // No brackets, life is good
        {
            auto lowestOperator = FindLowestOperator(svec, map);
            
            if (lowestOperator != svec.cend())
            {
                return std::make_unique<Operation>(
                    map.at(*lowestOperator).fun,
                    CreateTree({ svec.cbegin(), lowestOperator }, map),
                    CreateTree({ lowestOperator + 1, svec.cend() }, map)
                );
            }
            else
            {
                return std::make_unique<Operand>(
                    ToArithmetic(svec.front())
                );
            }
        }
        else
        { // There are brackets, so we need to handle them.
            auto tree = CreateTree(
                { brackets.Start() + 1, brackets.Finish() }, map);
            // Fetch the bracket result on-the-fly.
            
            Replace(svec, brackets.Start(), brackets.Finish() + 1, 
                    std::to_string(tree->Result())); 
            // Replace the brackets with the actual result.
            
            return CreateTree(std::move(svec), map);
        }
    }
    
    void Replace(StringVector& svec, 
                 StringVector::const_iterator first, 
                 StringVector::const_iterator last, 
                 const std::string& replacement)
    {
        auto dist = std::distance(svec.cbegin(), first);
        // Avoiding UB and iterator invalidation
        
        svec.erase(first, last);
        svec.insert(svec.cbegin() + dist, replacement);
    }
    
    StringVector::const_iterator FindLowestOperator(
        const StringVector& svec,
        const OperatorMap& map)
    {
        auto signs = OrderedSigns(map);
        
        for (auto i = signs.cbegin(); i != signs.cend(); ++i)
        {
            auto found = std::find_if(svec.cbegin(), svec.cend(), 
                [i](const auto& sign)
                {
                    return sign == std::string { *i };
                }
            );
            
            if (found != svec.cend()) // Found it
                return found;
        }
        
        return svec.cend(); // Couldn't find it
    }
}
