#include "proof_search.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace logixpr {

ProofSearch::ProofSearch(int max_depth, int max_transformations) 
    : max_depth(max_depth), max_transformations(max_transformations) {}

Proof ProofSearch::findProof(const ASTNode& start_expression, const ASTNode& target_expression) {
    return findShortestProof(start_expression, target_expression);
}

Proof ProofSearch::findShortestProof(const ASTNode& start_expression, const ASTNode& target_expression) {
    clearVisited();
    
    std::queue<ProofSearchNode> queue;
    queue.emplace(start_expression.clone(), 0, std::vector<ProofStep>());
    
    markVisited(start_expression);
    
    int transformations_explored = 0;
    
    while (!queue.empty() && transformations_explored < max_transformations) {
        ProofSearchNode current = std::move(queue.front());
        queue.pop();
        
        if (current.depth > max_depth) {
            continue;
        }
        
        if (equivalence_engine.areEquivalent(*current.expression, target_expression)) {
            return reconstructProof(current.path, true);
        }
        
        if (shouldPrune(current)) {
            continue;
        }
        
        auto expanded_nodes = expandNode(current);
        transformations_explored += expanded_nodes.size();
        
        for (auto& node : expanded_nodes) {
            if (!isVisited(*node.expression)) {
                markVisited(*node.expression);
                queue.push(std::move(node));
            }
        }
    }
    
    return reconstructProof({}, false);
}

std::vector<std::unique_ptr<ASTNode>> ProofSearch::generateEquivalentForms(const ASTNode& expression, int max_steps) {
    std::vector<std::unique_ptr<ASTNode>> equivalent_forms;
    clearVisited();
    
    std::queue<ProofSearchNode> queue;
    queue.emplace(expression.clone(), 0, std::vector<ProofStep>());
    markVisited(expression);
    
    int transformations_explored = 0;
    
    while (!queue.empty() && transformations_explored < max_transformations && equivalent_forms.size() < 50) {
        ProofSearchNode current = std::move(queue.front());
        queue.pop();
        
        if (current.depth > max_steps) {
            continue;
        }
        
        equivalent_forms.push_back(current.expression->clone());
        
        auto expanded_nodes = expandNode(current);
        transformations_explored += expanded_nodes.size();
        
        for (auto& node : expanded_nodes) {
            if (!isVisited(*node.expression)) {
                markVisited(*node.expression);
                queue.push(std::move(node));
            }
        }
    }
    
    return equivalent_forms;
}

void ProofSearch::setMaxDepth(int depth) {
    max_depth = depth;
}

void ProofSearch::setMaxTransformations(int transformations) {
    max_transformations = transformations;
}

bool ProofSearch::isVisited(const ASTNode& expression) {
    std::string expr_str = expressionToString(expression);
    return visited_expressions.find(expr_str) != visited_expressions.end();
}

void ProofSearch::markVisited(const ASTNode& expression) {
    std::string expr_str = expressionToString(expression);
    visited_expressions.insert(expr_str);
}

void ProofSearch::clearVisited() {
    visited_expressions.clear();
}

std::string ProofSearch::expressionToString(const ASTNode& expression) {
    return expression.toString();
}

bool ProofSearch::shouldPrune(const ProofSearchNode& node) {
    if (node.depth >= max_depth) {
        return true;
    }
    
    std::string expr_str = expressionToString(*node.expression);
    if (expr_str.length() > 200) {
        return true;
    }
    
    return false;
}

std::vector<ProofSearchNode> ProofSearch::expandNode(const ProofSearchNode& node) {
    std::vector<ProofSearchNode> expanded_nodes;
    
    // Try transformations on the entire expression
    auto transformations = equivalence_engine.generateAllTransformations(*node.expression);
    
    for (auto& transformation : transformations) {
        std::vector<ProofStep> new_path;
        for (auto& step : node.path) {
            new_path.emplace_back(step.expression->clone(), step.law_applied, step.description, step.step_number);
        }
        new_path.emplace_back(
            transformation.result->clone(),
            transformation.law,
            transformation.description,
            node.depth + 1
        );
        
        expanded_nodes.emplace_back(
            std::move(transformation.result),
            node.depth + 1,
            std::move(new_path)
        );
    }
    
    // Try transformations on subexpressions
    if (node.expression->getType() == NodeType::IMPLIES || 
        node.expression->getType() == NodeType::BICONDITIONAL ||
        node.expression->getType() == NodeType::AND ||
        node.expression->getType() == NodeType::OR) {
        const auto& binary = static_cast<const BinaryOpNode&>(*node.expression);
        
        // Try transforming left subexpression
        auto left_transformations = equivalence_engine.generateAllTransformations(binary.getLeft());
        for (auto& transformation : left_transformations) {
            std::vector<ProofStep> new_path;
            for (auto& step : node.path) {
                new_path.emplace_back(step.expression->clone(), step.law_applied, step.description, step.step_number);
            }
            
            auto new_expr = std::make_unique<BinaryOpNode>(
                node.expression->getType(),
                std::move(transformation.result),
                binary.getRight().clone()
            );
            
            new_path.emplace_back(
                new_expr->clone(),
                transformation.law,
                transformation.description,
                node.depth + 1
            );
            
            expanded_nodes.emplace_back(
                std::move(new_expr),
                node.depth + 1,
                std::move(new_path)
            );
        }
        
        // Try transforming right subexpression
        auto right_transformations = equivalence_engine.generateAllTransformations(binary.getRight());
        for (auto& transformation : right_transformations) {
            std::vector<ProofStep> new_path;
            for (auto& step : node.path) {
                new_path.emplace_back(step.expression->clone(), step.law_applied, step.description, step.step_number);
            }
            
            auto new_expr = std::make_unique<BinaryOpNode>(
                node.expression->getType(),
                binary.getLeft().clone(),
                std::move(transformation.result)
            );
            
            new_path.emplace_back(
                new_expr->clone(),
                transformation.law,
                transformation.description,
                node.depth + 1
            );
            
            expanded_nodes.emplace_back(
                std::move(new_expr),
                node.depth + 1,
                std::move(new_path)
            );
        }
    } else if (node.expression->getType() == NodeType::NOT) {
        const auto& unary = static_cast<const UnaryOpNode&>(*node.expression);
        
        // Try transforming the operand
        auto operand_transformations = equivalence_engine.generateAllTransformations(unary.getOperand());
        for (auto& transformation : operand_transformations) {
            std::vector<ProofStep> new_path;
            for (auto& step : node.path) {
                new_path.emplace_back(step.expression->clone(), step.law_applied, step.description, step.step_number);
            }
            
            auto new_expr = std::make_unique<UnaryOpNode>(
                NodeType::NOT,
                std::move(transformation.result)
            );
            
            new_path.emplace_back(
                new_expr->clone(),
                transformation.law,
                transformation.description,
                node.depth + 1
            );
            
            expanded_nodes.emplace_back(
                std::move(new_expr),
                node.depth + 1,
                std::move(new_path)
            );
        }
    }
    
    return expanded_nodes;
}

Proof ProofSearch::reconstructProof(const std::vector<ProofStep>& path, bool found_target) {
    Proof proof;
    proof.found_target = found_target;
    proof.total_steps = path.size();
    
    for (auto& step : path) {
        proof.steps.emplace_back(step.expression->clone(), step.law_applied, step.description, step.step_number);
    }
    
    for (size_t i = 0; i < proof.steps.size(); ++i) {
        proof.steps[i].step_number = i + 1;
    }
    
    return proof;
}

int ProofSearch::estimateDistance(const ASTNode& current, const ASTNode& target) {
    std::string current_str = expressionToString(current);
    std::string target_str = expressionToString(target);
    
    if (current_str == target_str) {
        return 0;
    }
    
    int differences = 0;
    size_t min_length = std::min(current_str.length(), target_str.length());
    
    for (size_t i = 0; i < min_length; ++i) {
        if (current_str[i] != target_str[i]) {
            differences++;
        }
    }
    
    differences += std::abs(static_cast<int>(current_str.length() - target_str.length()));
    
    return (differences + 4) / 5;
}

std::string logixpr::ProofFormatter::formatProof(const Proof& proof) {
    std::ostringstream oss;
    
    if (!proof.found_target) {
        oss << "No proof found within the search limits.\n";
        return oss.str();
    }

    oss << "Proof found in " << proof.total_steps << " steps:\n\n";
    
    for (const auto& step : proof.steps) {
        oss << "Step " << step.step_number << ": " << step.expression->toString() << "\n";
        if (!step.description.empty()) {
            oss << "  Using: " << LogicLaws::getLawName(step.law_applied) << "\n";
            oss << "  " << step.description << "\n";
        }
        oss << "\n";
    }
    
    return oss.str();
}

void logixpr::ProofFormatter::printProof(const Proof& proof) {
    std::cout << formatProof(proof);
}

void logixpr::ProofFormatter::printProofStatistics(const Proof& proof) {
    std::cout << "\nProof Statistics:\n";
    std::cout << "  Total steps: " << proof.total_steps << "\n";
    std::cout << "  Proof found: " << (proof.found_target ? "Yes" : "No") << "\n";
    
    if (!proof.steps.empty()) {
        std::cout << "  Initial expression: " << proof.steps.front().expression->toString() << "\n";
        std::cout << "  Final expression: " << proof.steps.back().expression->toString() << "\n";
    }
    
    std::cout << "\n";
}

}