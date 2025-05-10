#include <gtest/gtest.h>
#include "logic_laws.h"
#include "parser.h"

namespace logixpr {
namespace test {

class LogicLawsTest : public ::testing::Test {
protected:
    LogicLawsTest() : laws() {}
    LogicLaws laws;

    std::unique_ptr<ASTNode> parse(const std::string& expr) {
        return ExpressionParser::parse(expr);
    }

    void assertTransformation(const std::string& input, const std::string& expected, LogicLaw law) {
        auto inputNode = parse(input);
        auto expectedNode = parse(expected);
        std::unique_ptr<ASTNode> result;
        
        switch (law) {
            case LogicLaw::DOUBLE_NEGATION:
                result = LogicLaws::applyDoubleNegation(*inputNode);
                break;
            case LogicLaw::DE_MORGAN_AND:
                result = LogicLaws::applyDeMorganAnd(*inputNode);
                break;
            case LogicLaw::DE_MORGAN_OR:
                result = LogicLaws::applyDeMorganOr(*inputNode);
                break;
            case LogicLaw::DISTRIBUTIVE_AND_OVER_OR:
                result = LogicLaws::applyDistributiveAndOverOr(*inputNode);
                break;
            case LogicLaw::DISTRIBUTIVE_OR_OVER_AND:
                result = LogicLaws::applyDistributiveOrOverAnd(*inputNode);
                break;
            case LogicLaw::ABSORPTION_AND:
                result = LogicLaws::applyAbsorptionAnd(*inputNode);
                break;
            case LogicLaw::ABSORPTION_OR:
                result = LogicLaws::applyAbsorptionOr(*inputNode);
                break;
            case LogicLaw::COMPLEMENT_AND:
                result = LogicLaws::applyComplementAnd(*inputNode);
                break;
            case LogicLaw::COMPLEMENT_OR:
                result = LogicLaws::applyComplementOr(*inputNode);
                break;
            case LogicLaw::IDEMPOTENT_AND:
                result = LogicLaws::applyIdempotentAnd(*inputNode);
                break;
            case LogicLaw::IDEMPOTENT_OR:
                result = LogicLaws::applyIdempotentOr(*inputNode);
                break;
            case LogicLaw::IMPLICATION_ELIMINATION:
                result = LogicLaws::applyImplicationElimination(*inputNode);
                break;
            case LogicLaw::BICONDITIONAL_ELIMINATION:
                result = LogicLaws::applyBiconditionalElimination(*inputNode);
                break;
            default:
                FAIL() << "Unsupported logic law";
        }
        
        ASSERT_TRUE(result != nullptr);
        EXPECT_TRUE(result->equals(*expectedNode));
    }

    void assertNoTransformation(const std::string& input, LogicLaw law) {
        auto inputNode = parse(input);
        std::unique_ptr<ASTNode> result;
        
        switch (law) {
            case LogicLaw::DOUBLE_NEGATION:
                result = LogicLaws::applyDoubleNegation(*inputNode);
                break;
            case LogicLaw::DE_MORGAN_AND:
                result = LogicLaws::applyDeMorganAnd(*inputNode);
                break;
            case LogicLaw::DE_MORGAN_OR:
                result = LogicLaws::applyDeMorganOr(*inputNode);
                break;
            case LogicLaw::DISTRIBUTIVE_AND_OVER_OR:
                result = LogicLaws::applyDistributiveAndOverOr(*inputNode);
                break;
            case LogicLaw::DISTRIBUTIVE_OR_OVER_AND:
                result = LogicLaws::applyDistributiveOrOverAnd(*inputNode);
                break;
            case LogicLaw::ABSORPTION_AND:
                result = LogicLaws::applyAbsorptionAnd(*inputNode);
                break;
            case LogicLaw::ABSORPTION_OR:
                result = LogicLaws::applyAbsorptionOr(*inputNode);
                break;
            case LogicLaw::COMPLEMENT_AND:
                result = LogicLaws::applyComplementAnd(*inputNode);
                break;
            case LogicLaw::COMPLEMENT_OR:
                result = LogicLaws::applyComplementOr(*inputNode);
                break;
            case LogicLaw::IDEMPOTENT_AND:
                result = LogicLaws::applyIdempotentAnd(*inputNode);
                break;
            case LogicLaw::IDEMPOTENT_OR:
                result = LogicLaws::applyIdempotentOr(*inputNode);
                break;
            case LogicLaw::IMPLICATION_ELIMINATION:
                result = LogicLaws::applyImplicationElimination(*inputNode);
                break;
            case LogicLaw::BICONDITIONAL_ELIMINATION:
                result = LogicLaws::applyBiconditionalElimination(*inputNode);
                break;
            default:
                FAIL() << "Unsupported logic law";
        }
        
        EXPECT_TRUE(result == nullptr);
    }
};

TEST_F(LogicLawsTest, DoubleNegation) {
    assertTransformation("!!p", "p", LogicLaw::DOUBLE_NEGATION);
    assertTransformation("!!q", "q", LogicLaw::DOUBLE_NEGATION);
    assertNoTransformation("!p", LogicLaw::DOUBLE_NEGATION);
    assertNoTransformation("p", LogicLaw::DOUBLE_NEGATION);
}

TEST_F(LogicLawsTest, DeMorganAnd) {
    assertTransformation("!(p & q)", "!p | !q", LogicLaw::DE_MORGAN_AND);
    assertTransformation("!(r & s)", "!r | !s", LogicLaw::DE_MORGAN_AND);
    assertNoTransformation("!(p | q)", LogicLaw::DE_MORGAN_AND);
    assertNoTransformation("p & q", LogicLaw::DE_MORGAN_AND);
}

TEST_F(LogicLawsTest, DeMorganOr) {
    assertTransformation("!(p | q)", "!p & !q", LogicLaw::DE_MORGAN_OR);
    assertTransformation("!(r | s)", "!r & !s", LogicLaw::DE_MORGAN_OR);
    assertNoTransformation("!(p & q)", LogicLaw::DE_MORGAN_OR);
    assertNoTransformation("p | q", LogicLaw::DE_MORGAN_OR);
}

TEST_F(LogicLawsTest, DistributiveAndOverOr) {
    assertTransformation("p & (q | r)", "(p & q) | (p & r)", LogicLaw::DISTRIBUTIVE_AND_OVER_OR);
    assertTransformation("(q | r) & p", "(q & p) | (r & p)", LogicLaw::DISTRIBUTIVE_AND_OVER_OR);
    assertNoTransformation("p | (q & r)", LogicLaw::DISTRIBUTIVE_AND_OVER_OR);
}

TEST_F(LogicLawsTest, DistributiveOrOverAnd) {
    assertTransformation("p | (q & r)", "(p | q) & (p | r)", LogicLaw::DISTRIBUTIVE_OR_OVER_AND);
    assertTransformation("(q & r) | p", "(q | p) & (r | p)", LogicLaw::DISTRIBUTIVE_OR_OVER_AND);
    assertNoTransformation("p & (q | r)", LogicLaw::DISTRIBUTIVE_OR_OVER_AND);
}

TEST_F(LogicLawsTest, AbsorptionAnd) {
    assertTransformation("p & (p | q)", "p", LogicLaw::ABSORPTION_AND);
    assertTransformation("(p | q) & p", "p", LogicLaw::ABSORPTION_AND);
    assertNoTransformation("p | (p & q)", LogicLaw::ABSORPTION_AND);
}

TEST_F(LogicLawsTest, AbsorptionOr) {
    assertTransformation("p | (p & q)", "p", LogicLaw::ABSORPTION_OR);
    assertTransformation("(p & q) | p", "p", LogicLaw::ABSORPTION_OR);
    assertNoTransformation("p & (p | q)", LogicLaw::ABSORPTION_OR);
}

TEST_F(LogicLawsTest, ImplicationElimination) {
    assertTransformation("p -> q", "!p | q", LogicLaw::IMPLICATION_ELIMINATION);
    assertTransformation("r -> s", "!r | s", LogicLaw::IMPLICATION_ELIMINATION);
    assertNoTransformation("p & q", LogicLaw::IMPLICATION_ELIMINATION);
}

TEST_F(LogicLawsTest, BiconditionalElimination) {
    assertTransformation("p <-> q", "(p -> q) & (q -> p)", LogicLaw::BICONDITIONAL_ELIMINATION);
    assertTransformation("r <-> s", "(r -> s) & (s -> r)", LogicLaw::BICONDITIONAL_ELIMINATION);
    assertNoTransformation("p -> q", LogicLaw::BICONDITIONAL_ELIMINATION);
}

TEST_F(LogicLawsTest, ComplementAnd) {
    assertTransformation("p & !p", "F", LogicLaw::COMPLEMENT_AND);
    assertTransformation("!p & p", "F", LogicLaw::COMPLEMENT_AND);
    assertNoTransformation("p & q", LogicLaw::COMPLEMENT_AND);
}

TEST_F(LogicLawsTest, ComplementOr) {
    assertTransformation("p | !p", "T", LogicLaw::COMPLEMENT_OR);
    assertTransformation("!p | p", "T", LogicLaw::COMPLEMENT_OR);
    assertNoTransformation("p | q", LogicLaw::COMPLEMENT_OR);
}

TEST_F(LogicLawsTest, IdempotentAnd) {
    assertTransformation("p & p", "p", LogicLaw::IDEMPOTENT_AND);
    assertTransformation("q & q", "q", LogicLaw::IDEMPOTENT_AND);
    assertNoTransformation("p & q", LogicLaw::IDEMPOTENT_AND);
}

TEST_F(LogicLawsTest, IdempotentOr) {
    assertTransformation("p | p", "p", LogicLaw::IDEMPOTENT_OR);
    assertTransformation("q | q", "q", LogicLaw::IDEMPOTENT_OR);
    assertNoTransformation("p | q", LogicLaw::IDEMPOTENT_OR);
}

} // namespace test
} // namespace logixpr