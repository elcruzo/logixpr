#include "equivalence_engine.h"
#include <algorithm>
#include <queue>

namespace logixpr {

std::string LogicLaws::getLawName(LogicLaw law) {
    switch (law) {
        case LogicLaw::DOUBLE_NEGATION: return "Double Negation";
        case LogicLaw::DE_MORGAN_AND: return "De Morgan's Law (AND)";
        case LogicLaw::DE_MORGAN_OR: return "De Morgan's Law (OR)";
        case LogicLaw::DISTRIBUTIVE_AND_OVER_OR: return "Distributive Law (AND over OR)";
        case LogicLaw::DISTRIBUTIVE_OR_OVER_AND: return "Distributive Law (OR over AND)";
        case LogicLaw::ABSORPTION_AND: return "Absorption Law (AND)";
        case LogicLaw::ABSORPTION_OR: return "Absorption Law (OR)";
        case LogicLaw::IDENTITY_AND: return "Identity Law (AND)";
        case LogicLaw::IDENTITY_OR: return "Identity Law (OR)";
        case LogicLaw::ANNIHILATION_AND: return "Annihilation Law (AND)";
        case LogicLaw::ANNIHILATION_OR: return "Annihilation Law (OR)";
        case LogicLaw::COMPLEMENT_AND: return "Complement Law (AND)";
        case LogicLaw::COMPLEMENT_OR: return "Complement Law (OR)";
        case LogicLaw::IDEMPOTENT_AND: return "Idempotent Law (AND)";
        case LogicLaw::IDEMPOTENT_OR: return "Idempotent Law (OR)";
        case LogicLaw::COMMUTATIVE_AND: return "Commutative Law (AND)";
        case LogicLaw::COMMUTATIVE_OR: return "Commutative Law (OR)";
        case LogicLaw::ASSOCIATIVE_AND: return "Associative Law (AND)";
        case LogicLaw::ASSOCIATIVE_OR: return "Associative Law (OR)";
        case LogicLaw::IMPLICATION_ELIMINATION: return "Implication Elimination";
        case LogicLaw::BICONDITIONAL_ELIMINATION: return "Biconditional Elimination";
        default: return "Unknown Law";
    }
}

std::unique_ptr<ASTNode> LogicLaws::applyDoubleNegation(const ASTNode& node) {
    if (node.getType() == NodeType::NOT) {
        const auto& unary = static_cast<const UnaryOpNode&>(node);
        const auto& operand = unary.getOperand();
        if (operand.getType() == NodeType::NOT) {
            const auto& inner_unary = static_cast<const UnaryOpNode&>(operand);
            return inner_unary.getOperand().clone();
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyDeMorganAnd(const ASTNode& node) {
    if (node.getType() == NodeType::NOT) {
        const auto& unary = static_cast<const UnaryOpNode&>(node);
        const auto& operand = unary.getOperand();
        if (operand.getType() == NodeType::AND) {
            const auto& binary = static_cast<const BinaryOpNode&>(operand);
            auto left_neg = createNegation(binary.getLeft().clone());
            auto right_neg = createNegation(binary.getRight().clone());
            return createOr(std::move(left_neg), std::move(right_neg));
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyDeMorganOr(const ASTNode& node) {
    if (node.getType() == NodeType::NOT) {
        const auto& unary = static_cast<const UnaryOpNode&>(node);
        const auto& operand = unary.getOperand();
        if (operand.getType() == NodeType::OR) {
            const auto& binary = static_cast<const BinaryOpNode&>(operand);
            auto left_neg = createNegation(binary.getLeft().clone());
            auto right_neg = createNegation(binary.getRight().clone());
            return createAnd(std::move(left_neg), std::move(right_neg));
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyImplicationElimination(const ASTNode& node) {
    if (node.getType() == NodeType::IMPLIES) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        auto left_neg = createNegation(binary.getLeft().clone());
        return createOr(std::move(left_neg), binary.getRight().clone());
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyBiconditionalElimination(const ASTNode& node) {
    if (node.getType() == NodeType::BICONDITIONAL) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        auto left_implies_right = std::make_unique<BinaryOpNode>(
            NodeType::IMPLIES, binary.getLeft().clone(), binary.getRight().clone());
        auto right_implies_left = std::make_unique<BinaryOpNode>(
            NodeType::IMPLIES, binary.getRight().clone(), binary.getLeft().clone());
        return createAnd(std::move(left_implies_right), std::move(right_implies_left));
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyDistributiveAndOverOr(const ASTNode& node) {
    if (node.getType() == NodeType::AND) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (binary.getRight().getType() == NodeType::OR) {
            const auto& right_or = static_cast<const BinaryOpNode&>(binary.getRight());
            auto left_and_b = createAnd(binary.getLeft().clone(), right_or.getLeft().clone());
            auto left_and_c = createAnd(binary.getLeft().clone(), right_or.getRight().clone());
            return createOr(std::move(left_and_b), std::move(left_and_c));
        }
        if (binary.getLeft().getType() == NodeType::OR) {
            const auto& left_or = static_cast<const BinaryOpNode&>(binary.getLeft());
            auto b_and_right = createAnd(left_or.getLeft().clone(), binary.getRight().clone());
            auto c_and_right = createAnd(left_or.getRight().clone(), binary.getRight().clone());
            return createOr(std::move(b_and_right), std::move(c_and_right));
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyDistributiveOrOverAnd(const ASTNode& node) {
    if (node.getType() == NodeType::OR) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (binary.getRight().getType() == NodeType::AND) {
            const auto& right_and = static_cast<const BinaryOpNode&>(binary.getRight());
            auto left_or_b = createOr(binary.getLeft().clone(), right_and.getLeft().clone());
            auto left_or_c = createOr(binary.getLeft().clone(), right_and.getRight().clone());
            return createAnd(std::move(left_or_b), std::move(left_or_c));
        }
        if (binary.getLeft().getType() == NodeType::AND) {
            const auto& left_and = static_cast<const BinaryOpNode&>(binary.getLeft());
            auto b_or_right = createOr(left_and.getLeft().clone(), binary.getRight().clone());
            auto c_or_right = createOr(left_and.getRight().clone(), binary.getRight().clone());
            return createAnd(std::move(b_or_right), std::move(c_or_right));
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyAbsorptionAnd(const ASTNode& node) {
    if (node.getType() == NodeType::AND) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (binary.getRight().getType() == NodeType::OR) {
            const auto& right_or = static_cast<const BinaryOpNode&>(binary.getRight());
            if (binary.getLeft().equals(right_or.getLeft()) || binary.getLeft().equals(right_or.getRight())) {
                return binary.getLeft().clone();
            }
        }
        if (binary.getLeft().getType() == NodeType::OR) {
            const auto& left_or = static_cast<const BinaryOpNode&>(binary.getLeft());
            if (binary.getRight().equals(left_or.getLeft()) || binary.getRight().equals(left_or.getRight())) {
                return binary.getRight().clone();
            }
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyAbsorptionOr(const ASTNode& node) {
    if (node.getType() == NodeType::OR) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (binary.getRight().getType() == NodeType::AND) {
            const auto& right_and = static_cast<const BinaryOpNode&>(binary.getRight());
            if (binary.getLeft().equals(right_and.getLeft()) || binary.getLeft().equals(right_and.getRight())) {
                return binary.getLeft().clone();
            }
        }
        if (binary.getLeft().getType() == NodeType::AND) {
            const auto& left_and = static_cast<const BinaryOpNode&>(binary.getLeft());
            if (binary.getRight().equals(left_and.getLeft()) || binary.getRight().equals(left_and.getRight())) {
                return binary.getRight().clone();
            }
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyIdentityAnd(const ASTNode& node) {
    if (node.getType() == NodeType::AND) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (isConstantTrue(binary.getLeft())) {
            return binary.getRight().clone();
        }
        if (isConstantTrue(binary.getRight())) {
            return binary.getLeft().clone();
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyIdentityOr(const ASTNode& node) {
    if (node.getType() == NodeType::OR) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (isConstantFalse(binary.getLeft())) {
            return binary.getRight().clone();
        }
        if (isConstantFalse(binary.getRight())) {
            return binary.getLeft().clone();
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyAnnihilationAnd(const ASTNode& node) {
    if (node.getType() == NodeType::AND) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (isConstantFalse(binary.getLeft()) || isConstantFalse(binary.getRight())) {
            return std::make_unique<ConstantNode>(false);
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyAnnihilationOr(const ASTNode& node) {
    if (node.getType() == NodeType::OR) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (isConstantTrue(binary.getLeft()) || isConstantTrue(binary.getRight())) {
            return std::make_unique<ConstantNode>(true);
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyComplementAnd(const ASTNode& node) {
    if (node.getType() == NodeType::AND) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (binary.getLeft().getType() == NodeType::NOT) {
            const auto& left_not = static_cast<const UnaryOpNode&>(binary.getLeft());
            if (left_not.getOperand().equals(binary.getRight())) {
                return std::make_unique<ConstantNode>(false);
            }
        }
        if (binary.getRight().getType() == NodeType::NOT) {
            const auto& right_not = static_cast<const UnaryOpNode&>(binary.getRight());
            if (right_not.getOperand().equals(binary.getLeft())) {
                return std::make_unique<ConstantNode>(false);
            }
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyComplementOr(const ASTNode& node) {
    if (node.getType() == NodeType::OR) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (binary.getLeft().getType() == NodeType::NOT) {
            const auto& left_not = static_cast<const UnaryOpNode&>(binary.getLeft());
            if (left_not.getOperand().equals(binary.getRight())) {
                return std::make_unique<ConstantNode>(true);
            }
        }
        if (binary.getRight().getType() == NodeType::NOT) {
            const auto& right_not = static_cast<const UnaryOpNode&>(binary.getRight());
            if (right_not.getOperand().equals(binary.getLeft())) {
                return std::make_unique<ConstantNode>(true);
            }
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyIdempotentAnd(const ASTNode& node) {
    if (node.getType() == NodeType::AND) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (binary.getLeft().equals(binary.getRight())) {
            return binary.getLeft().clone();
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyIdempotentOr(const ASTNode& node) {
    if (node.getType() == NodeType::OR) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (binary.getLeft().equals(binary.getRight())) {
            return binary.getLeft().clone();
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyCommutativeAnd(const ASTNode& node) {
    if (node.getType() == NodeType::AND) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        return createAnd(binary.getRight().clone(), binary.getLeft().clone());
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyCommutativeOr(const ASTNode& node) {
    if (node.getType() == NodeType::OR) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        return createOr(binary.getRight().clone(), binary.getLeft().clone());
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyAssociativeAnd(const ASTNode& node) {
    if (node.getType() == NodeType::AND) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (binary.getLeft().getType() == NodeType::AND) {
            const auto& left_and = static_cast<const BinaryOpNode&>(binary.getLeft());
            auto b_and_c = createAnd(left_and.getRight().clone(), binary.getRight().clone());
            return createAnd(left_and.getLeft().clone(), std::move(b_and_c));
        }
        if (binary.getRight().getType() == NodeType::AND) {
            const auto& right_and = static_cast<const BinaryOpNode&>(binary.getRight());
            auto a_and_b = createAnd(binary.getLeft().clone(), right_and.getLeft().clone());
            return createAnd(std::move(a_and_b), right_and.getRight().clone());
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> LogicLaws::applyAssociativeOr(const ASTNode& node) {
    if (node.getType() == NodeType::OR) {
        const auto& binary = static_cast<const BinaryOpNode&>(node);
        if (binary.getLeft().getType() == NodeType::OR) {
            const auto& left_or = static_cast<const BinaryOpNode&>(binary.getLeft());
            auto b_or_c = createOr(left_or.getRight().clone(), binary.getRight().clone());
            return createOr(left_or.getLeft().clone(), std::move(b_or_c));
        }
        if (binary.getRight().getType() == NodeType::OR) {
            const auto& right_or = static_cast<const BinaryOpNode&>(binary.getRight());
            auto a_or_b = createOr(binary.getLeft().clone(), right_or.getLeft().clone());
            return createOr(std::move(a_or_b), right_or.getRight().clone());
        }
    }
    return nullptr;
}

bool LogicLaws::isConstantTrue(const ASTNode& node) {
    return node.getType() == NodeType::CONSTANT && 
           static_cast<const ConstantNode&>(node).getValue();
}

bool LogicLaws::isConstantFalse(const ASTNode& node) {
    return node.getType() == NodeType::CONSTANT && 
           !static_cast<const ConstantNode&>(node).getValue();
}

bool LogicLaws::isNegation(const ASTNode& node) {
    return node.getType() == NodeType::NOT;
}

std::unique_ptr<ASTNode> LogicLaws::createNegation(std::unique_ptr<ASTNode> operand) {
    return std::make_unique<UnaryOpNode>(NodeType::NOT, std::move(operand));
}

std::unique_ptr<ASTNode> LogicLaws::createAnd(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) {
    return std::make_unique<BinaryOpNode>(NodeType::AND, std::move(left), std::move(right));
}

std::unique_ptr<ASTNode> LogicLaws::createOr(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) {
    return std::make_unique<BinaryOpNode>(NodeType::OR, std::move(left), std::move(right));
}

std::size_t EquivalenceEngine::ASTHasher::operator()(const std::unique_ptr<ASTNode>& node) const {
    EquivalenceEngine engine;
    return engine.computeHash(*node);
}

bool EquivalenceEngine::ASTEqual::operator()(const std::unique_ptr<ASTNode>& a, const std::unique_ptr<ASTNode>& b) const {
    return a->equals(*b);
}

std::vector<Transformation> EquivalenceEngine::generateAllTransformations(const ASTNode& expression) {
    std::vector<Transformation> transformations;
    
    static const std::vector<LogicLaw> all_laws = {
        LogicLaw::DOUBLE_NEGATION,
        LogicLaw::DE_MORGAN_AND,
        LogicLaw::DE_MORGAN_OR,
        LogicLaw::DISTRIBUTIVE_AND_OVER_OR,
        LogicLaw::DISTRIBUTIVE_OR_OVER_AND,
        LogicLaw::ABSORPTION_AND,
        LogicLaw::ABSORPTION_OR,
        LogicLaw::IDENTITY_AND,
        LogicLaw::IDENTITY_OR,
        LogicLaw::ANNIHILATION_AND,
        LogicLaw::ANNIHILATION_OR,
        LogicLaw::COMPLEMENT_AND,
        LogicLaw::COMPLEMENT_OR,
        LogicLaw::IDEMPOTENT_AND,
        LogicLaw::IDEMPOTENT_OR,
        LogicLaw::COMMUTATIVE_AND,
        LogicLaw::COMMUTATIVE_OR,
        LogicLaw::ASSOCIATIVE_AND,
        LogicLaw::ASSOCIATIVE_OR,
        LogicLaw::IMPLICATION_ELIMINATION,
        LogicLaw::BICONDITIONAL_ELIMINATION
    };
    
    for (LogicLaw law : all_laws) {
        auto law_transformations = applyLawRecursively(expression, law);
        for (auto& trans : law_transformations) {
            transformations.push_back(std::move(trans));
        }
    }
    
    return transformations;
}

std::vector<Transformation> EquivalenceEngine::applyLawRecursively(const ASTNode& expression, LogicLaw law) {
    std::vector<Transformation> transformations;
    
    auto direct_result = applyLawToNode(expression, law);
    if (direct_result) {
        transformations.emplace_back(law, LogicLaws::getLawName(law), std::move(direct_result));
    }
    
    auto subexpr_transformations = applyLawToSubexpressions(expression, law);
    for (auto& trans : subexpr_transformations) {
        transformations.push_back(std::move(trans));
    }
    
    return transformations;
}

std::vector<Transformation> EquivalenceEngine::applyLawToSubexpressions(const ASTNode& expression, LogicLaw law) {
    std::vector<Transformation> transformations;
    
    if (expression.getType() == NodeType::NOT) {
        const auto& unary = static_cast<const UnaryOpNode&>(expression);
        auto sub_transformations = applyLawRecursively(unary.getOperand(), law);
        
        for (auto& trans : sub_transformations) {
            auto new_expr = std::make_unique<UnaryOpNode>(NodeType::NOT, std::move(trans.result));
            transformations.emplace_back(trans.law, trans.description, std::move(new_expr));
        }
    } else if (expression.getType() == NodeType::AND || 
               expression.getType() == NodeType::OR ||
               expression.getType() == NodeType::IMPLIES ||
               expression.getType() == NodeType::BICONDITIONAL) {
        const auto& binary = static_cast<const BinaryOpNode&>(expression);
        
        // Try transforming left subexpression
        auto left_transformations = applyLawRecursively(binary.getLeft(), law);
        for (auto& trans : left_transformations) {
            auto new_expr = std::make_unique<BinaryOpNode>(
                expression.getType(), 
                std::move(trans.result), 
                binary.getRight().clone()
            );
            transformations.emplace_back(trans.law, trans.description, std::move(new_expr));
        }
        
        // Try transforming right subexpression
        auto right_transformations = applyLawRecursively(binary.getRight(), law);
        for (auto& trans : right_transformations) {
            auto new_expr = std::make_unique<BinaryOpNode>(
                expression.getType(), 
                binary.getLeft().clone(),
                std::move(trans.result)
            );
            transformations.emplace_back(trans.law, trans.description, std::move(new_expr));
        }
        
        // Try transforming both subexpressions
        for (auto& left_trans : left_transformations) {
            for (auto& right_trans : right_transformations) {
                auto new_expr = std::make_unique<BinaryOpNode>(
                    expression.getType(),
                    left_trans.result->clone(),
                    right_trans.result->clone()
                );
                transformations.emplace_back(
                    left_trans.law,
                    left_trans.description + " and " + right_trans.description,
                    std::move(new_expr)
                );
            }
        }
    }
    
    return transformations;
}

std::unique_ptr<ASTNode> EquivalenceEngine::applyLawToNode(const ASTNode& node, LogicLaw law) {
    switch (law) {
        case LogicLaw::DOUBLE_NEGATION:
            return LogicLaws::applyDoubleNegation(node);
        case LogicLaw::DE_MORGAN_AND:
            return LogicLaws::applyDeMorganAnd(node);
        case LogicLaw::DE_MORGAN_OR:
            return LogicLaws::applyDeMorganOr(node);
        case LogicLaw::DISTRIBUTIVE_AND_OVER_OR:
            return LogicLaws::applyDistributiveAndOverOr(node);
        case LogicLaw::DISTRIBUTIVE_OR_OVER_AND:
            return LogicLaws::applyDistributiveOrOverAnd(node);
        case LogicLaw::ABSORPTION_AND:
            return LogicLaws::applyAbsorptionAnd(node);
        case LogicLaw::ABSORPTION_OR:
            return LogicLaws::applyAbsorptionOr(node);
        case LogicLaw::IDENTITY_AND:
            return LogicLaws::applyIdentityAnd(node);
        case LogicLaw::IDENTITY_OR:
            return LogicLaws::applyIdentityOr(node);
        case LogicLaw::ANNIHILATION_AND:
            return LogicLaws::applyAnnihilationAnd(node);
        case LogicLaw::ANNIHILATION_OR:
            return LogicLaws::applyAnnihilationOr(node);
        case LogicLaw::COMPLEMENT_AND:
            return LogicLaws::applyComplementAnd(node);
        case LogicLaw::COMPLEMENT_OR:
            return LogicLaws::applyComplementOr(node);
        case LogicLaw::IDEMPOTENT_AND:
            return LogicLaws::applyIdempotentAnd(node);
        case LogicLaw::IDEMPOTENT_OR:
            return LogicLaws::applyIdempotentOr(node);
        case LogicLaw::COMMUTATIVE_AND:
            return LogicLaws::applyCommutativeAnd(node);
        case LogicLaw::COMMUTATIVE_OR:
            return LogicLaws::applyCommutativeOr(node);
        case LogicLaw::ASSOCIATIVE_AND:
            return LogicLaws::applyAssociativeAnd(node);
        case LogicLaw::ASSOCIATIVE_OR:
            return LogicLaws::applyAssociativeOr(node);
        case LogicLaw::IMPLICATION_ELIMINATION:
            return LogicLaws::applyImplicationElimination(node);
        case LogicLaw::BICONDITIONAL_ELIMINATION:
            return LogicLaws::applyBiconditionalElimination(node);
        default:
            return nullptr;
    }
}

bool EquivalenceEngine::areEquivalent(const ASTNode& expr1, const ASTNode& expr2) {
    return expr1.equals(expr2);
}

std::size_t EquivalenceEngine::computeHash(const ASTNode& node) const {
    std::hash<int> int_hasher;
    std::hash<std::string> string_hasher;
    std::hash<bool> bool_hasher;
    
    switch (node.getType()) {
        case NodeType::VARIABLE: {
            const auto& var = static_cast<const VariableNode&>(node);
            return combineHashes(int_hasher(static_cast<int>(NodeType::VARIABLE)), 
                               string_hasher(var.getName()));
        }
        case NodeType::CONSTANT: {
            const auto& constant = static_cast<const ConstantNode&>(node);
            return combineHashes(int_hasher(static_cast<int>(NodeType::CONSTANT)), 
                               bool_hasher(constant.getValue()));
        }
        case NodeType::NOT: {
            const auto& unary = static_cast<const UnaryOpNode&>(node);
            return combineHashes(int_hasher(static_cast<int>(NodeType::NOT)), 
                               computeHash(unary.getOperand()));
        }
        case NodeType::AND:
        case NodeType::OR:
        case NodeType::IMPLIES:
        case NodeType::BICONDITIONAL: {
            const auto& binary = static_cast<const BinaryOpNode&>(node);
            std::size_t type_hash = int_hasher(static_cast<int>(node.getType()));
            std::size_t left_hash = computeHash(binary.getLeft());
            std::size_t right_hash = computeHash(binary.getRight());
            return combineHashes(combineHashes(type_hash, left_hash), right_hash);
        }
    }
    return 0;
}

std::size_t EquivalenceEngine::combineHashes(std::size_t h1, std::size_t h2) const {
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}

}