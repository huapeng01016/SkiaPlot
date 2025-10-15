# Contributing to SkiaPlot

Thank you for your interest in contributing to SkiaPlot! This document provides guidelines for contributing to the project.

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/your-username/SkiaPlot.git`
3. Create a branch: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Test your changes
6. Commit: `git commit -am 'Add some feature'`
7. Push: `git push origin feature/your-feature-name`
8. Create a Pull Request

## Development Setup

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15 or higher
- Skia library (see README.md for build instructions)

### Building

```bash
export SKIA_DIR=/path/to/skia
mkdir build
cd build
cmake ..
make
```

## Code Style

### C++ Guidelines

- Use C++17 features appropriately
- Follow the existing code style in the project
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and concise

### Formatting

- Indentation: 4 spaces (no tabs)
- Line length: Aim for 80-100 characters, max 120
- Braces: Opening brace on the same line
- Naming conventions:
  - Classes: `PascalCase` (e.g., `DataSeries`)
  - Functions/methods: `camelCase` (e.g., `addPoint`)
  - Variables: `camelCase` or `snake_case` for private members with trailing underscore (e.g., `points_`)
  - Constants: `UPPER_CASE` or `kPascalCase`

### Example

```cpp
class MyClass {
public:
    MyClass(int value);
    
    void doSomething(int parameter);
    int getValue() const { return value_; }
    
private:
    int value_;
    std::string name_;
};

void MyClass::doSomething(int parameter) {
    if (parameter > 0) {
        value_ = parameter;
    }
}
```

## What to Contribute

### Welcome Contributions

- Bug fixes
- New plot types (bar charts, scatter plots, histograms, etc.)
- Performance improvements
- Documentation improvements
- Example programs
- Test coverage
- Platform-specific improvements

### Feature Requests

Before implementing a new feature:

1. Check if an issue exists for it
2. If not, create an issue describing the feature
3. Wait for discussion/approval
4. Implement the feature
5. Submit a pull request

## Testing

### Before Submitting

- Ensure your code compiles without warnings
- Test with example programs
- Verify output images are correct
- Check for memory leaks (use valgrind or similar tools)

### Adding Tests

If adding new functionality:
- Add example programs demonstrating the feature
- Update documentation

## Documentation

### Code Documentation

- Add doc comments for public APIs
- Use clear, concise language
- Include examples where helpful

### README Updates

If your changes affect usage:
- Update README.md
- Update USAGE.md if adding new features
- Add examples if appropriate

## Pull Request Process

1. **Title**: Clear, concise description of changes
2. **Description**: 
   - What changes were made
   - Why they were made
   - Any relevant issue numbers (#123)
3. **Testing**: Describe how you tested the changes
4. **Screenshots**: If UI-related, include before/after images

### PR Checklist

- [ ] Code compiles without warnings
- [ ] Changes are tested
- [ ] Documentation is updated
- [ ] Examples added/updated if needed
- [ ] Commit messages are clear
- [ ] Code follows project style

## Commit Messages

Good commit messages help maintain project history:

### Format

```
Short summary (50 chars or less)

More detailed explanation if necessary. Wrap at 72 characters.
Explain the problem this commit solves and why this approach was
taken.

- Bullet points are okay
- Use imperative mood: "Add feature" not "Added feature"

Fixes #123
```

### Examples

Good:
```
Add histogram plotting support

Implements histogram functionality with configurable bin sizes.
Users can now create histograms from data arrays.

Fixes #45
```

Not ideal:
```
fixed stuff
```

## Issue Reporting

### Bug Reports

When reporting bugs, include:

1. **Description**: Clear description of the bug
2. **Steps to reproduce**: Detailed steps
3. **Expected behavior**: What should happen
4. **Actual behavior**: What actually happens
5. **Environment**: 
   - OS and version
   - Compiler and version
   - Skia version
   - SkiaPlot version
6. **Code sample**: Minimal code to reproduce
7. **Error messages**: Full error output

### Feature Requests

When requesting features:

1. **Use case**: Why is this feature needed?
2. **Description**: Detailed description
3. **Examples**: How would it be used?
4. **Alternatives**: Have you considered alternatives?

## Code Review

All submissions require review. We aim to:

- Respond to PRs within 3-5 days
- Provide constructive feedback
- Maintain code quality

When reviewing:
- Be respectful and constructive
- Explain reasoning for requested changes
- Approve when ready

## Community

### Code of Conduct

- Be respectful and inclusive
- Welcome newcomers
- Focus on constructive criticism
- Help others learn

### Getting Help

- Open an issue for bugs or questions
- Check existing issues and documentation first
- Provide enough context for others to help

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Recognition

Contributors will be acknowledged in:
- Project README
- Release notes
- Git history

Thank you for contributing to SkiaPlot!
