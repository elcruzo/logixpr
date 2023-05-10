#pragma once

#include "ast.h"
#include "equivalence_engine.h"
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <queue>

namespace logixpr {

struct ProofStep {
    std::unique_ptr<ASTNode> expression;
    LogicLaw law_applied;
    std::string description;
    int step_number;
    
    ProofStep(std::unique_ptr<ASTNode> expr, LogicLaw law, const std::string& desc, int step)
        : expression(std::move(expr)), law_applied(law), description(desc), step_number(step) {}
};

struct Proof {
    std::vector<ProofStep> steps;
    bool found_target;
    int total_steps;
    
    Proof() : found_target(false), total_steps(0) {}
};

class ProofSearchNode {
public:
    std::unique_ptr<ASTNode> expression;
    int depth;
    std::vector<ProofStep> path;
    
    ProofSearchNode(std::unique_ptr<ASTNode> expr, int d, std::vector<ProofStep> p)
        : expression(std::move(expr)), depth(d), path(std::move(p)) {}
};

class ProofSearch {
private:
    EquivalenceEngine equivalence_engine;
    std::unordered_set<std::string> visited_expressions;
    int max_depth;
    int max_transformations;
    
public:
    explicit ProofSearch(int max_depth = 10, int max_transformations = 10000);
    
    Proof findProof(const ASTNode& start_expression, const ASTNode& target_expression);
    
    Proof findShortestProof(const ASTNode& start_expression, const ASTNode& target_expression);
    
    std::vector<std::unique_ptr<ASTNode>> generateEquivalentForms(const ASTNode& expression, int max_steps = 5);
    
    void setMaxDepth(int depth);
    void setMaxTransformations(int transformations);

private:
    bool isVisited(const ASTNode& expression);
    void markVisited(const ASTNode& expression);
    void clearVisited();
    
    std::string expressionToString(const ASTNode& expression);
    
    bool shouldPrune(const ProofSearchNode& node);
    
    std::vector<ProofSearchNode> expandNode(const ProofSearchNode& node);
    
    Proof reconstructProof(const std::vector<ProofStep>& path, bool found_target);
    
    int estimateDistance(const ASTNode& current, const ASTNode& target);
};

class ProofFormatter {
public:
    static std::string formatProof(const Proof& proof);
    static std::string formatProofStep(const ProofStep& step);
    static void printProof(const Proof& proof);
    static void printProofStatistics(const Proof& proof);
};

}