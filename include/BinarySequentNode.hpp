//
// Created by Andrew Nazareth on 3/31/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_BINARYSEQUENTNODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_BINARYSEQUENTNODE_HPP

#include "SequentNode.hpp"

namespace Logic_Project {
class BinarySequentNode final: public SequentNode {
public:
    BinarySequentNode() = default;
    BinarySequentNode(const SequentNode& left, const SequentNode& right);

    std::unique_ptr<SequentNode> leftParent;
    std::unique_ptr<SequentNode> rightParent;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_BINARYSEQUENTNODE_HPP
