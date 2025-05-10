#include <gtest/gtest.h>
#include "proof_search.h"
#include "parser.h"
#include "equivalence_engine.h"

namespace logixpr {
namespace test {

class ProofSearchTest : public ::testing::Test {
protected:
    ProofSearchTest() : proofSearch() {}
    ProofSearch proofSearch;
};

TEST_F(ProofSearchTest, DoubleNegation) {
    auto expr1 = ExpressionParser::parse("!!p");
    auto expr2 = ExpressionParser::parse("p");
    auto proof = proofSearch.findProof(*expr1, *expr2);
    ASSERT_TRUE(proof.found_target);
    EXPECT_EQ(proof.steps.size(), 1);
    EXPECT_EQ(proof.steps[0].law_applied, LogicLaw::DOUBLE_NEGATION);
}

TEST_F(ProofSearchTest, DeMorganAnd) {
    auto expr1 = ExpressionParser::parse("!(p & q)");
    auto expr2 = ExpressionParser::parse("!p | !q");
    auto proof = proofSearch.findProof(*expr1, *expr2);
    ASSERT_TRUE(proof.found_target);
    EXPECT_EQ(proof.steps[0].law_applied, LogicLaw::DE_MORGAN_AND);
}

TEST_F(ProofSearchTest, DeMorganOr) {
    auto expr1 = ExpressionParser::parse("!(p | q)");
    auto expr2 = ExpressionParser::parse("!p & !q");
    auto proof = proofSearch.findProof(*expr1, *expr2);
    ASSERT_TRUE(proof.found_target);
    EXPECT_EQ(proof.steps[0].law_applied, LogicLaw::DE_MORGAN_OR);
}

TEST_F(ProofSearchTest, DistributiveAndOverOr) {
    auto expr1 = ExpressionParser::parse("p & (q | r)");
    auto expr2 = ExpressionParser::parse("(p & q) | (p & r)");
    auto proof = proofSearch.findProof(*expr1, *expr2);
    ASSERT_TRUE(proof.found_target);
    EXPECT_EQ(proof.steps[0].law_applied, LogicLaw::DISTRIBUTIVE_AND_OVER_OR);
}

TEST_F(ProofSearchTest, ImplicationElimination) {
    auto expr1 = ExpressionParser::parse("p -> q");
    auto expr2 = ExpressionParser::parse("!p | q");
    auto proof = proofSearch.findProof(*expr1, *expr2);
    ASSERT_TRUE(proof.found_target);
    EXPECT_EQ(proof.steps[0].law_applied, LogicLaw::IMPLICATION_ELIMINATION);
}

TEST_F(ProofSearchTest, BiconditionalElimination) {
    auto expr1 = ExpressionParser::parse("p <-> q");
    auto expr2 = ExpressionParser::parse("(p -> q) & (q -> p)");
    auto proof = proofSearch.findProof(*expr1, *expr2);
    ASSERT_TRUE(proof.found_target);
    EXPECT_EQ(proof.steps[0].law_applied, LogicLaw::BICONDITIONAL_ELIMINATION);
}

TEST_F(ProofSearchTest, ComplexTransformation) {
    auto expr1 = ExpressionParser::parse("!!(p & q) -> r");
    auto expr2 = ExpressionParser::parse("(p & q) -> r");
    auto proof = proofSearch.findProof(*expr1, *expr2);
    ASSERT_TRUE(proof.found_target);
    EXPECT_GT(proof.steps.size(), 1);
}

TEST_F(ProofSearchTest, NoProofExists) {
    auto expr1 = ExpressionParser::parse("p & q");
    auto expr2 = ExpressionParser::parse("p | q");
    auto proof = proofSearch.findProof(*expr1, *expr2);
    ASSERT_FALSE(proof.found_target);
}

TEST_F(ProofSearchTest, ShortestProof) {
    auto expr1 = ExpressionParser::parse("!!(p & q)");
    auto expr2 = ExpressionParser::parse("p & q");
    auto proof = proofSearch.findShortestProof(*expr1, *expr2);
    ASSERT_TRUE(proof.found_target);
    EXPECT_EQ(proof.steps.size(), 1);
}

} // namespace test
} // namespace logixpr