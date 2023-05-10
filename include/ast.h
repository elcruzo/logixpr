#pragma once

#include <memory>
#include <string>
#include <vector>

namespace logixpr {

enum class NodeType {
    VARIABLE,
    CONSTANT,
    NOT,
    AND,
    OR,
    IMPLIES,
    BICONDITIONAL
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual NodeType getType() const = 0;
    virtual std::string toString() const = 0;
    virtual std::unique_ptr<ASTNode> clone() const = 0;
    virtual bool equals(const ASTNode& other) const = 0;
};

class VariableNode : public ASTNode {
private:
    std::string name;

public:
    explicit VariableNode(const std::string& name);
    NodeType getType() const override;
    std::string toString() const override;
    std::unique_ptr<ASTNode> clone() const override;
    bool equals(const ASTNode& other) const override;
    const std::string& getName() const;
};

class ConstantNode : public ASTNode {
private:
    bool value;

public:
    explicit ConstantNode(bool value);
    NodeType getType() const override;
    std::string toString() const override;
    std::unique_ptr<ASTNode> clone() const override;
    bool equals(const ASTNode& other) const override;
    bool getValue() const;
};

class UnaryOpNode : public ASTNode {
private:
    NodeType op_type;
    std::unique_ptr<ASTNode> operand;

public:
    UnaryOpNode(NodeType op_type, std::unique_ptr<ASTNode> operand);
    NodeType getType() const override;
    std::string toString() const override;
    std::unique_ptr<ASTNode> clone() const override;
    bool equals(const ASTNode& other) const override;
    const ASTNode& getOperand() const;
    std::unique_ptr<ASTNode> takeOperand();
};

class BinaryOpNode : public ASTNode {
private:
    NodeType op_type;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

public:
    BinaryOpNode(NodeType op_type, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);
    NodeType getType() const override;
    std::string toString() const override;
    std::unique_ptr<ASTNode> clone() const override;
    bool equals(const ASTNode& other) const override;
    const ASTNode& getLeft() const;
    const ASTNode& getRight() const;
    std::unique_ptr<ASTNode> takeLeft();
    std::unique_ptr<ASTNode> takeRight();
};

}