#include <algorithm>
#include <calculator.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>
#include <memory>
#include <string>

TEST_CASE("Untestable code", "[.][Calculator]")
{
    using DependencyInjection::NoDependencyInjection::Calculator;

    Calculator calculator;
    calculator.run();
}

class MockTerminal : public Dependency::ITerminal
{
public:
    MAKE_MOCK1(print, void(const std::string&), override);
    MAKE_MOCK0(read, std::string(), override);
};

TEST_CASE("Dependency Injection with constructor", "[Calculator, DI-Constructor]")
{
    using DependencyInjection::ConstructorDI::Calculator;

    MockTerminal terminal;
    ALLOW_CALL(terminal, print(ANY(std::string)));

    REQUIRE_CALL(terminal, read()).RETURN("1");
    REQUIRE_CALL(terminal, read()).RETURN("2");
    REQUIRE_CALL(terminal, print("The sum is: 3"));

    Calculator calculator(terminal);
    calculator.run();
}

TEST_CASE("Dependency Injection with setter", "[Calculator, DI-Setter]")
{
    using DependencyInjection::SetterDI::Calculator;

    auto terminal = std::make_unique<MockTerminal>();
    ALLOW_CALL(*terminal, print(ANY(std::string)));

    REQUIRE_CALL(*terminal, read()).RETURN("1");
    REQUIRE_CALL(*terminal, read()).RETURN("2");
    REQUIRE_CALL(*terminal, print("The sum is: 3"));

    Calculator calculator;
    calculator.set_terminal(std::move(terminal));
    calculator.run();
}

TEST_CASE("Dependency Injection with template parameter", "[Calculator, DI-Template]")
{
    using DependencyInjection::TemplateParameterDI::Calculator;

    MockTerminal terminal;
    ALLOW_CALL(terminal, print(ANY(std::string)));

    REQUIRE_CALL(terminal, read()).RETURN("1");
    REQUIRE_CALL(terminal, read()).RETURN("2");
    REQUIRE_CALL(terminal, print("The sum is: 3"));

    Calculator<MockTerminal> calculator(terminal);
    calculator.run();
}

class TestableCalculator : public DependencyInjection::FactoryMethodDI::Calculator
{
    MockTerminal terminal_;

public:
    MockTerminal& get_terminal() override
    {
        return terminal_;
    }
};

auto set_expectations(MockTerminal& terminal)
{
    return std::tuple{
        NAMED_ALLOW_CALL(terminal, print(ANY(std::string))),
        NAMED_REQUIRE_CALL(terminal, read()).RETURN("1"),
        NAMED_REQUIRE_CALL(terminal, read()).RETURN("2"),
        NAMED_REQUIRE_CALL(terminal, print("The sum is: 3"))
    };
}

TEST_CASE("Dependency Injection with Factory Method", "[Calculator, DI-FactoryMethod]")
{
    using DependencyInjection::FactoryMethodDI::Calculator;
    
    TestableCalculator calculator;
    auto& mock_terminal = calculator.get_terminal();
    auto mock_expectations = set_expectations(mock_terminal);

    calculator.run();
}