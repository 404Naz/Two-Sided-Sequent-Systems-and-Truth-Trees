//
// Created by Andrew Nazareth on 3/21/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_LEAFLOGICEXPRESSION_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_LEAFLOGICEXPRESSION_HPP
#include "LogicExpression.hpp"
#include <string>

namespace Logic_Project {
template <typename DerivedT>
class LeafLogicExpression : public LogicExpression {
public:
    [[nodiscard]] std::unique_ptr<LogicExpression> Copy() const final
    {
        return std::make_unique<DerivedT>(*static_cast<const DerivedT*>(this));
    }

    EXPRESSION_CATEGORY(LeafExpression)
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_LEAFLOGICEXPRESSION_HPP
