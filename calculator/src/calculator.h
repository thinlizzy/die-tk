#ifndef CALCULATOR_H_exampletk_guard_49bnm32gdajafak
#define CALCULATOR_H_exampletk_guard_49bnm32gdajafak

#include <functional>

typedef std::function<double(double,double)> Operation;

class Calculator;

class CalculatorState {
public:
    virtual ~CalculatorState() {}
    virtual bool addDigit(Calculator & calculator, char digit) = 0;
    virtual bool execOperation(Calculator & calculator, char symbol) = 0;
    virtual bool execute(Calculator & calculator) = 0;
};

class Calculator {
    friend class NewNumber;
    friend class NoDecimalPoint;
    friend class WithDecimalPoint;
    
    CalculatorState * state;
    int divDecimalPoint;
    Operation previousOp;
    double previousNumber;
    
    void resetPrevious();
    void executePreviousOperation();
    void pushOperation(Operation operation);
    void addDecimalPoint();
public:    
    double currentNumber;
    
    Calculator();
    bool addDigit(char n);
    bool execOperation(char symbol);
    bool execute();
    void reset();
};

#endif
