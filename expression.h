#pragma once

#include "instruction_element.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <algorithm>

namespace Calc
{
    struct Operator;
    
    using OperatorVector = std::vector<Operator>;
    using StringVector = std::vector<std::string>;
    
    using OperatorMap = std::map<std::string, Operator>;
    
    struct Operator
    {
        int priority { }; // Operator priority
        OperatorFunction fun { }; // A binary operator function
    };
    
    class BracketPair
    {
    public:
        BracketPair(const StringVector&) noexcept;
        
        bool None() const noexcept;
        
        StringVector::const_iterator Start() const noexcept;
        StringVector::const_iterator Finish() const noexcept;
        
    private:
        using CIPair = std::pair<StringVector::const_iterator,
                                 StringVector::const_iterator>;
        
        const StringVector& m_original;
        CIPair m_result { };
    };
    
    class InvalidIdentifierException : public std::runtime_error
    {
        using runtime_error::runtime_error;
    };
    
    OperatorMap::const_iterator MaxPriorityElement(
        OperatorMap::const_iterator, OperatorMap::const_iterator,
        std::function<bool(int, int)>);
    // Returns the maximum element. Uses the given function
    // as a predicate, but compares by priority
    
    bool IsNumberElement(char) noexcept; // TODO give me a better name
    bool IsOperationElement(char, const OperatorMap&);
    
    void Prepare(std::string&) noexcept;
    // Do some preparation for the string to be translated
    
    std::string OrderedSigns(OperatorMap); // Intentionally copying
    // Creates a string from a map's operators signs (i.e., keys).
    // The signs are ordered by priority in reverse.
    
    StringVector TranslateString(const std::string&, const OperatorMap&);
    // Translates the input string into an Pieces object.
    // Like you described. I can reuse the code for 
    // "two vectors" I had, except this is better and simpler
    
    TreeElementPtr CreateTree(StringVector, // Intentionally copying
                              const OperatorMap&);
    // Finally creates the tree to be parsed
    
    void Replace(StringVector&, 
                 StringVector::const_iterator, 
                 StringVector::const_iterator, 
                 const std::string&);
    // Replace a range of the StringVector
    // with a different string.
    
    StringVector::const_iterator FindLowestOperator(
        const StringVector&, const OperatorMap&);
    // Finds the currently lowest operator in the StringVector.
}
