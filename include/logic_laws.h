#pragma once

#include "ast.h"
#include <string>
#include <memory>

namespace logixpr {

enum class LogicLaw {
    DOUBLE_NEGATION,
    DE_MORGAN_AND,
    DE_MORGAN_OR,
    DISTRIBUTIVE_AND_OVER_OR,
    DISTRIBUTIVE_OR_OVER_AND,
    ABSORPTION_AND,
    ABSORPTION_OR,
    IDENTITY_AND,
    IDENTITY_OR,
    ANNIHILATION_AND,
    ANNIHILATION_OR,
    COMPLEMENT_AND,
    COMPLEMENT_OR,
    IDEMPOTENT_AND,
    IDEMPOTENT_OR,
    COMMUTATIVE_AND,
    COMMUTATIVE_OR,
    ASSOCIATIVE_AND,
    ASSOCIATIVE_OR,
    IMPLICATION_ELIMINATION,
    BICONDITIONAL_ELIMINATION
};

struct Transformation {
    LogicLaw law;
    std::string description;
    std::unique_ptr<ASTNode> result;
    
    Transformation(LogicLaw law, const std::string& desc, std::unique_ptr<ASTNode> result)
        : law(law), description(desc), result(std::move(result)) {}
};

class LogicLaws {
public:
    static std::string getLawName(LogicLaw law);
    static std::string getLawDescription(LogicLaw law);
    
    static std::unique_ptr<ASTNode> applyDoubleNegation(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyDeMorganAnd(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyDeMorganOr(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyDistributiveAndOverOr(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyDistributiveOrOverAnd(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyAbsorptionAnd(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyAbsorptionOr(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyIdentityAnd(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyIdentityOr(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyAnnihilationAnd(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyAnnihilationOr(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyComplementAnd(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyComplementOr(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyIdempotentAnd(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyIdempotentOr(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyCommutativeAnd(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyCommutativeOr(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyAssociativeAnd(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyAssociativeOr(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyImplicationElimination(const ASTNode& node);
    static std::unique_ptr<ASTNode> applyBiconditionalElimination(const ASTNode& node);

private:
    static bool isConstantTrue(const ASTNode& node);
    static bool isConstantFalse(const ASTNode& node);
    static bool isNegation(const ASTNode& node);
    static std::unique_ptr<ASTNode> createNegation(std::unique_ptr<ASTNode> operand);
    static std::unique_ptr<ASTNode> createAnd(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);
    static std::unique_ptr<ASTNode> createOr(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);
};

}