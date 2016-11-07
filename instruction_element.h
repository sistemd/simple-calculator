#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace Calc
{
    class TreeElement;
    
    using TreeElementPtr = std::unique_ptr<TreeElement>;
    using ArithmeticType = double;
    using ArithmeticVector = std::vector<ArithmeticType>;
    
    using OperatorFunction = 
        std::function<ArithmeticType(ArithmeticType, ArithmeticType)>;
    
    // TODO Rethink this design and the comment below.
    
    using Plus = std::plus<ArithmeticType>;
    using Minus = std::minus<ArithmeticType>;
    using Multiplies = std::multiplies<ArithmeticType>;
    using Divides = std::divides<ArithmeticType>;
    
    struct Powers
    {
        ArithmeticType operator()(
            ArithmeticType, ArithmeticType) noexcept;
    };
    
    ArithmeticType ToArithmetic(const std::string&);
    
    class TreeElement
    {
    public:
        TreeElement() = default;
        
        TreeElement(const TreeElement&) = delete;
        TreeElement(TreeElement&&) = delete;
        TreeElement& operator=(
            const TreeElement&) = delete;
        TreeElement& operator=(TreeElement&&) = delete;
        
        virtual ~TreeElement() = default;
        
        virtual ArithmeticType Result() const = 0;
        virtual TreeElementPtr Clone() const = 0;
    };
    
    class Operand : public TreeElement
    {
    public:
        explicit Operand(ArithmeticType) noexcept;
        
        ArithmeticType Result() const override;
        TreeElementPtr Clone() const override;
    
    private:
        ArithmeticType m_value { };
    };
    
    class Operation : public TreeElement
    {
    public:
        Operation(const OperatorFunction&,
                  TreeElementPtr, 
                  TreeElementPtr) noexcept;
        
        ArithmeticType Result() const override;
        TreeElementPtr Clone() const override;
        
    private:
        OperatorFunction m_f { };
        TreeElementPtr m_left { nullptr };
        TreeElementPtr m_right { nullptr };
    };
}
