#include "instruction_element.h"
#include <sstream>
#include <cmath>
#include <iostream>

namespace Calc
{
    ArithmeticType Powers::operator()(ArithmeticType left, 
                                      ArithmeticType right) noexcept
    {
        return std::pow(left, right); 
    }
    
    ArithmeticType ToArithmetic(const std::string& str)
    {
        ArithmeticType result { };
        std::stringstream stream { str };
        
        stream >> result;
        
        return result;
    }
    
    Operand::Operand(ArithmeticType value) noexcept
        : m_value { value }
    {
    }
    
    ArithmeticType Operand::Result() const
    {
        return m_value;
    }
    
    TreeElementPtr Operand::Clone() const
    {
        return std::make_unique<Operand>(m_value);
    }
    
    Operation::Operation(const OperatorFunction& f,
                         TreeElementPtr left,
                         TreeElementPtr right) noexcept
        : m_f { f }
        , m_left { std::move(left) }
        , m_right { std::move(right) }
    {
    }
    
    ArithmeticType Operation::Result() const
    {
        
        
        return m_f(m_left->Result(), m_right->Result());
    }
    
    TreeElementPtr Operation::Clone() const
    {
        return std::make_unique<Operation>(m_f,
                                           m_left->Clone(),
                                           m_right->Clone());
    }
}
