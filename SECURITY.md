# Security Policy

## Supported Versions

| Version | Supported |
| ------- | --------- |
| 1.0.x   | ✅        |
| < 1.0   | ❌        |

## Reporting a Vulnerability

If you discover a security vulnerability in LogiXpr, please report it responsibly:

### How to Report

1. **DO NOT** create a public GitHub issue
2. Email security details to: [dev.elcruzo@gmail.com]
3. Include the following information:
   - Description of the vulnerability
   - Steps to reproduce
   - Potential impact
   - Suggested fix (if you have one)

### What to Expect

- We will acknowledge receipt within 48 hours
- We will provide regular updates on our progress
- We will credit you in our security advisories (unless you prefer to remain anonymous)
- We will work with you to coordinate disclosure

### Scope

This security policy applies to:

- The LogiXpr codebase
- Dependencies and build system
- Documentation and examples

### Out of Scope

- Issues in third-party dependencies (please report directly to their maintainers)
- Social engineering attacks
- Physical attacks
- Denial of service attacks that don't involve code vulnerabilities

## Security Best Practices

When using LogiXpr:

1. **Input Validation**: Always validate user input before passing it to LogiXpr
2. **Resource Limits**: Be aware that complex expressions may consume significant memory/CPU
3. **Sandboxing**: Consider running LogiXpr in a sandboxed environment for untrusted input
4. **Updates**: Keep your LogiXpr installation updated to the latest version

## Security Considerations

- LogiXpr processes logical expressions and may be vulnerable to:
  - Stack overflow from deeply nested expressions
  - Memory exhaustion from very large expressions
  - Infinite loops in certain edge cases
- The tool is designed for trusted input and should not be used with untrusted user input without proper validation

## Disclosure Timeline

- **Day 0**: Vulnerability reported
- **Day 1-2**: Initial response and triage
- **Day 3-7**: Investigation and fix development
- **Day 8-14**: Testing and validation
- **Day 15+**: Coordinated disclosure and release

## Credits

We thank the security researchers and community members who help keep LogiXpr secure.
