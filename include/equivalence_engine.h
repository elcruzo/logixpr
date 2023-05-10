#pragma once

#include "ast.h"
#include "logic_laws.h"
#include <vector>
#include <memory>
#include <unordered_set>
#include <functional>

namespace logixpr {

class EquivalenceEngine {
private:
    struct ASTHasher {
        std::size_t operator()(const std::unique_ptr<ASTNode>& node) const;
    };
    
    struct ASTEqual {
        bool operator()(const std::unique_ptr<ASTNode>& a, const std::unique_ptr<ASTNode>& b) const;
    };
    
    using VisitedSet = std::unordered_set<std::unique_ptr<ASTNode>, ASTHasher, ASTEqual>;
    
public:
    std::vector<Transformation> generateAllTransformations(const ASTNode& expression);
    
    std::vector<Transformation> applyAllLaws(const ASTNode& expression);
    
    std::vector<Transformation> applyLawRecursively(const ASTNode& expression, LogicLaw law);
    
    bool areEquivalent(const ASTNode& expr1, const ASTNode& expr2);

private:
    std::vector<Transformation> applyLawToSubexpressions(const ASTNode& expression, LogicLaw law);
    
    std::unique_ptr<ASTNode> applyLawToNode(const ASTNode& node, LogicLaw law);
    
    std::vector<std::unique_ptr<ASTNode>> generateSubstitutions(const ASTNode& expression, 
                                                               const ASTNode& original_subexpr,
                                                               const ASTNode& new_subexpr);
    
    std::size_t computeHash(const ASTNode& node) const;
    
    std::size_t combineHashes(std::size_t h1, std::size_t h2) const;
};

}