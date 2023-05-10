#include "parser.h"
#include "proof_search.h"
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>

using namespace logixpr;

void printUsage() {
    std::cout << "LogiXpr - Formal Logic Proof Generator\n\n";
    std::cout << "Usage: logixpr [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help          Show this help message\n";
    std::cout << "  -i, --interactive   Run in interactive mode\n";
    std::cout << "  -p, --prove         Prove equivalence between two expressions\n";
    std::cout << "  -g, --generate      Generate equivalent forms of an expression\n\n";
    std::cout << "Examples:\n";
    std::cout << "  logixpr -i                    # Interactive mode\n";
    std::cout << "  logixpr -p \"A & B\" \"B & A\"    # Prove equivalence\n";
    std::cout << "  logixpr -g \"!(A & B)\"         # Generate equivalent forms\n\n";
    std::cout << "Supported operators:\n";
    std::cout << "  !  ~  ¬     (NOT)\n";
    std::cout << "  &  &&  ∧    (AND)\n";
    std::cout << "  |  ||  ∨    (OR)\n";
    std::cout << "  ->  →       (IMPLIES)\n";
    std::cout << "  <->  ↔      (BICONDITIONAL)\n";
    std::cout << "  T           (TRUE)\n";
    std::cout << "  F           (FALSE)\n";
}

void runInteractiveMode() {
    std::cout << "LogiXpr Interactive Mode\n";
    std::cout << "Enter 'help' for commands, 'quit' to exit\n\n";
    
    ProofSearch searcher;
    std::string input;
    
    while (true) {
        std::cout << "logixpr> ";
        std::getline(std::cin, input);
        
        if (input == "quit" || input == "exit") {
            break;
        }
        
        if (input == "help") {
            std::cout << "Commands:\n";
            std::cout << "  prove <expr1> <expr2>  - Prove equivalence between expressions\n";
            std::cout << "  generate <expr>        - Generate equivalent forms\n";
            std::cout << "  parse <expr>           - Parse and display expression tree\n";
            std::cout << "  quit                   - Exit program\n\n";
            continue;
        }
        
        if (input.empty()) {
            continue;
        }
        
        if (input.substr(0, 5) == "prove") {
            size_t first_space = input.find(' ', 6);
            if (first_space == std::string::npos) {
                std::cout << "Usage: prove <expr1> <expr2>\n";
                continue;
            }
            
            std::string expr1_str = input.substr(6, first_space - 6);
            std::string expr2_str = input.substr(first_space + 1);
            
            try {
                auto expr1 = ExpressionParser::parse(expr1_str);
                auto expr2 = ExpressionParser::parse(expr2_str);
                
                std::cout << "Searching for proof from:\n";
                std::cout << "  " << expr1->toString() << "\n";
                std::cout << "to:\n";
                std::cout << "  " << expr2->toString() << "\n\n";
                
                auto proof = searcher.findProof(*expr1, *expr2);
                
                if (proof.found_target) {
                    ProofFormatter::printProof(proof);
                } else {
                    std::cout << "No proof found within search limits.\n";
                }
                
                ProofFormatter::printProofStatistics(proof);
                
            } catch (const ParseError& e) {
                std::cout << "Parse error: " << e.what() << " at position " << e.getPosition() << "\n";
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            
        } else if (input.substr(0, 8) == "generate") {
            std::string expr_str = input.substr(9);
            
            try {
                auto expr = ExpressionParser::parse(expr_str);
                
                std::cout << "Generating equivalent forms of: " << expr->toString() << "\n\n";
                
                auto equivalent_forms = searcher.generateEquivalentForms(*expr, 3);
                
                std::cout << "Found " << equivalent_forms.size() << " equivalent forms:\n";
                for (size_t i = 0; i < equivalent_forms.size() && i < 20; ++i) {
                    std::cout << std::setw(2) << (i + 1) << ". " << equivalent_forms[i]->toString() << "\n";
                }
                
                if (equivalent_forms.size() > 20) {
                    std::cout << "... and " << (equivalent_forms.size() - 20) << " more forms\n";
                }
                
            } catch (const ParseError& e) {
                std::cout << "Parse error: " << e.what() << " at position " << e.getPosition() << "\n";
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            
        } else if (input.substr(0, 5) == "parse") {
            std::string expr_str = input.substr(6);
            
            try {
                auto expr = ExpressionParser::parse(expr_str);
                std::cout << "Parsed expression: " << expr->toString() << "\n";
                
            } catch (const ParseError& e) {
                std::cout << "Parse error: " << e.what() << " at position " << e.getPosition() << "\n";
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            
        } else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
        
        std::cout << "\n";
    }
    
    std::cout << "Goodbye!\n";
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        runInteractiveMode();
        return 0;
    }
    
    std::string command = argv[1];
    
    if (command == "-h" || command == "--help") {
        printUsage();
        return 0;
    }
    
    if (command == "-i" || command == "--interactive") {
        runInteractiveMode();
        return 0;
    }
    
    if (command == "-p" || command == "--prove") {
        if (argc != 4) {
            std::cout << "Usage: " << argv[0] << " -p <expr1> <expr2>\n";
            return 1;
        }
        
        try {
            auto expr1 = ExpressionParser::parse(argv[2]);
            auto expr2 = ExpressionParser::parse(argv[3]);
            
            ProofSearch searcher;
            auto proof = searcher.findProof(*expr1, *expr2);
            
            ProofFormatter::printProof(proof);
            ProofFormatter::printProofStatistics(proof);
            
            return proof.found_target ? 0 : 1;
            
        } catch (const ParseError& e) {
            std::cout << "Parse error: " << e.what() << " at position " << e.getPosition() << "\n";
            return 1;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            return 1;
        }
    }
    
    if (command == "-g" || command == "--generate") {
        if (argc != 3) {
            std::cout << "Usage: " << argv[0] << " -g <expr>\n";
            return 1;
        }
        
        try {
            auto expr = ExpressionParser::parse(argv[2]);
            
            ProofSearch searcher;
            auto equivalent_forms = searcher.generateEquivalentForms(*expr, 3);
            
            std::cout << "Equivalent forms of: " << expr->toString() << "\n\n";
            for (size_t i = 0; i < equivalent_forms.size() && i < 20; ++i) {
                std::cout << std::setw(2) << (i + 1) << ". " << equivalent_forms[i]->toString() << "\n";
            }
            
            return 0;
            
        } catch (const ParseError& e) {
            std::cout << "Parse error: " << e.what() << " at position " << e.getPosition() << "\n";
            return 1;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            return 1;
        }
    }
    
    std::cout << "Unknown option: " << command << "\n";
    printUsage();
    return 1;
}