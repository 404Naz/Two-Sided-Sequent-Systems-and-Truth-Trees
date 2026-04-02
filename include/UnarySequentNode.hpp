//
// Created by Andrew Nazareth on 3/31/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_UNARYSEQUENTNODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_UNARYSEQUENTNODE_HPP

#include "SequentNode.hpp"

namespace Logic_Project {
class UnarySequentNode final : public SequentNode {
    public:
    UnarySequentNode() = default;
    UnarySequentNode(const SequentNode&);

    std::unique_ptr<SequentNode> parent;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_UNARYSEQUENTNODE_HPP
