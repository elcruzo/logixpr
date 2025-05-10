# Contributing to LogiXpr

Thank you for your interest in contributing to LogiXpr! This document provides guidelines and information for contributors.

## Getting Started

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.16 or later
- Git

### Building from Source

1. Clone the repository:

   ```bash
   git clone https://github.com/elcruzo/logixpr.git
   cd logixpr
   ```
2. Create a build directory:

   ```bash
   mkdir build && cd build
   ```
3. Configure and build:

   ```bash
   cmake ..
   make
   ```
4. Run tests:

   ```bash
   ./logixpr_test
   ```

## Development Guidelines

### Code Style

- Follow the existing code style in the project
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and reasonably sized
- Use `const` where appropriate

### C++ Standards

- Use C++17 features
- Prefer modern C++ idioms
- Use smart pointers when appropriate
- Follow RAII principles

### Testing

- All new features must include tests
- Tests should be in the `tests/` directory
- Use Google Test framework (already set up)
- Aim for good test coverage

### Documentation

- Update README.md if adding new features
- Add Doxygen comments for public APIs
- Update this CONTRIBUTING.md if needed

## Submitting Changes

### Pull Request Process

1. Fork the repository
2. Create a feature branch from `main`:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. Make your changes
4. Add tests for your changes
5. Ensure all tests pass:
   ```bash
   cd build
   make
   ./logixpr_test
   ```
6. Commit your changes with a clear message:
   ```bash
   git commit -m "Add feature: brief description"
   ```
7. Push to your fork:
   ```bash
   git push origin feature/your-feature-name
   ```
8. Create a Pull Request

### Commit Message Format

Use clear, descriptive commit messages:

- Start with a verb in imperative mood
- Keep the first line under 50 characters
- Add more details in the body if needed

Examples:

- `Add support for additional logic operators`
- `Fix memory leak in AST destructor`
- `Improve proof search algorithm performance`

### Pull Request Guidelines

- Provide a clear description of changes
- Reference any related issues
- Ensure CI passes
- Request review from maintainers
- Respond to feedback promptly

## Areas for Contribution

### High Priority

- Additional logic laws and transformations
- Performance optimizations
- Better error handling and user feedback
- More comprehensive test coverage

### Medium Priority

- Additional input/output formats
- GUI interface
- Integration with other tools
- Documentation improvements

### Low Priority

- Additional language bindings
- Plugin system
- Advanced proof visualization

## Reporting Issues

### Bug Reports

When reporting bugs, please include:

- Clear description of the issue
- Steps to reproduce
- Expected vs actual behavior
- System information (OS, compiler, etc.)
- Minimal example if possible

### Feature Requests

For feature requests, please include:

- Clear description of the feature
- Use case and motivation
- Potential implementation approach (if you have ideas)
- Any relevant examples

## Code of Conduct

- Be respectful and inclusive
- Focus on constructive feedback
- Help others learn and grow
- Follow the golden rule

## Questions?

Feel free to open an issue for questions about contributing or the codebase. We're here to help!

## License

By contributing to LogiXpr, you agree that your contributions will be licensed under the same MIT License that covers the project.
