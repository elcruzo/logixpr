#include "ast.h"
#include <sstream>

namespace logixpr {

VariableNode::VariableNode(const std::string& name) : name(name) {}

NodeType VariableNode::getType() const {
    return NodeType::VARIABLE;
}

std::string VariableNode::toString() const {
    return name;
}

std::unique_ptr<ASTNode> VariableNode::clone() const {
    return std::make_unique<VariableNode>(name);
}

bool VariableNode::equals(const ASTNode& other) const {
    if (other.getType() != NodeType::VARIABLE) return false;
    const auto& var = static_cast<const VariableNode&>(other);
    return name == var.name;
}

const std::string& VariableNode::getName() const {
    return name;
}

ConstantNode::ConstantNode(bool value) : value(value) {}

NodeType ConstantNode::getType() const {
    return NodeType::CONSTANT;
}

std::string ConstantNode::toString() const {
    return value ? "T" : "F";
}

std::unique_ptr<ASTNode> ConstantNode::clone() const {
    return std::make_unique<ConstantNode>(value);
}

bool ConstantNode::equals(const ASTNode& other) const {
    if (other.getType() != NodeType::CONSTANT) return false;
    const auto& constant = static_cast<const ConstantNode&>(other);
    return value == constant.value;
}

bool ConstantNode::getValue() const {
    return value;
}

UnaryOpNode::UnaryOpNode(NodeType op_type, std::unique_ptr<ASTNode> operand)
    : op_type(op_type), operand(std::move(operand)) {}

NodeType UnaryOpNode::getType() const {
    return op_type;
}

std::string UnaryOpNode::toString() const {
    std::ostringstream oss;
    switch (op_type) {
        case NodeType::NOT:
            oss << "!" << operand->toString();
            break;
        default:
            oss << "UNKNOWN_UNARY(" << operand->toString() << ")";
    }
    return oss.str();
}

std::unique_ptr<ASTNode> UnaryOpNode::clone() const {
    return std::make_unique<UnaryOpNode>(op_type, operand->clone());
}

bool UnaryOpNode::equals(const ASTNode& other) const {
    if (other.getType() != op_type) return false;
    const auto& unary = static_cast<const UnaryOpNode&>(other);
    return operand->equals(*unary.operand);
}

const ASTNode& UnaryOpNode::getOperand() const {
    return *operand;
}

std::unique_ptr<ASTNode> UnaryOpNode::takeOperand() {
    return std::move(operand);
}

BinaryOpNode::BinaryOpNode(NodeType op_type, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
    : op_type(op_type), left(std::move(left)), right(std::move(right)) {}

NodeType BinaryOpNode::getType() const {
    return op_type;
}

std::string BinaryOpNode::toString() const {
    std::ostringstream oss;
    oss << "(" << left->toString();
    
    switch (op_type) {
        case NodeType::AND:
            oss << " & ";
            break;
        case NodeType::OR:
            oss << " | ";
            break;
        case NodeType::IMPLIES:
            oss << " -> ";
            break;
        case NodeType::BICONDITIONAL:
            oss << " <-> ";
            break;
        default:
            oss << " UNKNOWN_BINARY ";
    }
    
    oss << right->toString() << ")";
    return oss.str();
}

std::unique_ptr<ASTNode> BinaryOpNode::clone() const {
    return std::make_unique<BinaryOpNode>(op_type, left->clone(), right->clone());
}

bool BinaryOpNode::equals(const ASTNode& other) const {
    if (other.getType() != op_type) return false;
    const auto& binary = static_cast<const BinaryOpNode&>(other);
    
    // For commutative operations (AND and OR), check both orderings
    if (op_type == NodeType::AND || op_type == NodeType::OR) {
        return (left->equals(*binary.left) && right->equals(*binary.right)) ||
               (left->equals(*binary.right) && right->equals(*binary.left));
    }
    
    // For non-commutative operations, order matters
    return left->equals(*binary.left) && right->equals(*binary.right);
}

const ASTNode& BinaryOpNode::getLeft() const {
    return *left;
}

const ASTNode& BinaryOpNode::getRight() const {
    return *right;
}

std::unique_ptr<ASTNode> BinaryOpNode::takeLeft() {
    return std::move(left);
}

std::unique_ptr<ASTNode> BinaryOpNode::takeRight() {
    return std::move(right);
}

}