#include "calculator.h"
#include <functional>
#include <stdexcept>

//      double previousNumber;
//     std::function<double(double,double)> operation;
//          int divDecimalPoint;
    
class NewNumber: public CalculatorState {
public:
    virtual bool addDigit(Calculator & calculator, char digit);
    virtual bool execOperation(Calculator & calculator, char symbol);
    virtual bool execute(Calculator & calculator);
};

class NoDecimalPoint: public CalculatorState {
public:
    virtual bool addDigit(Calculator & calculator, char digit);
    virtual bool execOperation(Calculator & calculator, char symbol);
    virtual bool execute(Calculator & calculator);
};

class WithDecimalPoint: public NoDecimalPoint {
public:
    virtual bool addDigit(Calculator & calculator, char digit);
};

NewNumber newNumber;
NoDecimalPoint noDecimalPoint;
WithDecimalPoint withDecimalPoint;

int getNumber(char digit) { return digit - '0'; }

Operation getOperation(char symbol)
{
    switch(symbol) {
        case '+': return std::plus<double>();
        case '-': return std::minus<double>();
        case '*': return std::multiplies<double>();
        case '/': return std::divides<double>();
        default: throw std::logic_error("invalid operation symbol");
    }
}

bool NewNumber::addDigit(Calculator & calculator, char digit)
{
    if( digit == '.' ) {
        calculator.addDecimalPoint();
    } else {
        calculator.state = &noDecimalPoint;
        calculator.currentNumber = getNumber(digit);
    }
    
    return true;
}

bool NoDecimalPoint::addDigit(Calculator & calculator, char digit)
{
    if( digit == '.' ) {
        calculator.addDecimalPoint();
    } else {
        calculator.currentNumber = calculator.currentNumber * 10 + getNumber(digit);
    }
    
    return true;
}

bool WithDecimalPoint::addDigit(Calculator & calculator, char digit)
{
    if( digit == '.' ) return false;
    
    calculator.currentNumber = calculator.currentNumber + double(getNumber(digit)) / double(calculator.divDecimalPoint);
    calculator.divDecimalPoint *= 10;
    
    return true;
}

bool NewNumber::execOperation(Calculator & calculator, char symbol)
{
    if( calculator.currentNumber == 0 ) return false;   // could add another state class to remove this if :)
            
    calculator.pushOperation(getOperation(symbol));    
    return true;
}

bool NoDecimalPoint::execOperation(Calculator & calculator, char symbol)
{
    calculator.executePreviousOperation();
    calculator.pushOperation(getOperation(symbol));    
    calculator.state = &newNumber;
    
    return true;
}

bool NewNumber::execute(Calculator & calculator) 
{ 
    return false;
}

bool NoDecimalPoint::execute(Calculator & calculator)
{
    auto lastNumber = calculator.currentNumber;
    calculator.executePreviousOperation();
    if( lastNumber == calculator.currentNumber ) return false;
    
    calculator.state = &newNumber;
    calculator.resetPrevious();
    return true;
}

auto noOp = [](double previous, double current) { return current; };

Calculator::Calculator():
    state(&newNumber),
    previousOp(noOp),
    previousNumber(0),
    currentNumber(0)
{
}

bool Calculator::addDigit(char n)
{
    return state->addDigit(*this,n);
}

bool Calculator::execOperation(char symbol)
{
    return state->execOperation(*this,symbol);
}

bool Calculator::execute()
{
    return state->execute(*this);
}

void Calculator::reset()
{
    state = &newNumber;
    currentNumber = 0;
    resetPrevious();
}
    
// called by states
    
void Calculator::resetPrevious()
{
    previousOp = noOp;
    previousNumber = 0;
}
    
void Calculator::executePreviousOperation()
{
    currentNumber = previousOp(previousNumber,currentNumber);
}

void Calculator::pushOperation(Operation operation)
{
    previousOp = operation;
    previousNumber = currentNumber;
}

void Calculator::addDecimalPoint()
{
    divDecimalPoint = 10;
    state = &withDecimalPoint;
}

