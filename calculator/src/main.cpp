#include "../../die-tk.h"
#include "calculator.h"

using namespace std;
using namespace tk;

// TODO add menu with some options like save history to file or copy number to clipboard or about dialog

int main(int argc, char** argv)
{
    int const margin = 5;
    int const dist = 10;
    int const dim = 20;
    int const tdist = dist + dim;
        
    Calculator calculator;
    
    Window calc(WindowParams()
        .text("Calculator")
        .dims(160,300)
    );
    
    Edit edit(calc, ControlParams()
        .start(margin,calc.height() - 5*tdist)
        .dims(calc.width()-10,dim)
        .text("0")
    );
    edit.setReadOnly(true);
    edit.setTextAlign(hta_right);
    
    Memo history(calc, ControlParams()
        .start(margin,margin)
        .dims(calc.width()-margin*2,edit.y()-margin*2)
    );
    history.setReadOnly(true);
    history.setTextAlign(hta_right);

    vector<Button> numbers;
    vector<Button> operations;
    {
        Button base(calc,ControlParams().dims(dim,dim));
        
        // create numbers
        numbers.emplace_back(calc,ControlParams()
            .start(dist,calc.height() - tdist)
            .dims(tdist + dim,dim)
            .text("0")
        );
        for( int n = 1; n <= 9; ++n ) {
            auto button = base.clone();
            button.setText(die::NativeString::toString(n));
            button.setPos( Point(dist + ((n-1) % 3) * tdist, numbers[0].y() - (((n-1) / 3 + 1) * tdist)) );
            numbers.push_back(move(button));
        }
        
        // anonymous button to define the dot
        Button(calc,ControlParams()
            .start(numbers[0].pos().addX(2*tdist))
            .dims(base.dims())
            .text(".")
        )
        .onClick([&edit,&calculator](){
            if( calculator.addDigit('.') ) {
                if( calculator.currentNumber == 0 ) {
                    edit.setText("0.");
                } else {
                    edit.setText(edit.getText() + ".");
                }
            }
        });
        
        // define the operations
        Button plus(calc,ControlParams()
            .start(numbers[0].pos().addX(3*tdist))
            .dims(base.dims())
            .text("+")
        );
        
        // anonymous cloning for the rest of operations.        
        // I wish I had a way to get rid of this fucking static_cast
        operations.push_back(
            static_cast<Button &&>(
                plus.clone()
                    .setPos(plus.pos().addY(-tdist))
                    .setText("-")
            ));
        operations.push_back(
            static_cast<Button &&>(
                plus.clone()
                    .setPos(plus.pos().addY(-tdist * 2))
                    .setText("*")
            ));
        operations.push_back(
            static_cast<Button &&>(
                plus.clone()
                    .setPos(plus.pos().addY(-tdist * 3))
                    .setText("/")
            ));
        operations.push_back(std::move(plus));
        
        // anonymous button to define the equals
        Button(calc,ControlParams()
            .start(numbers[0].pos() + Point(4*tdist,-2*tdist))
            .dims(base.dims().addHeight(2*tdist))
            .text("=")
        )
        .onClick([&edit,&calculator,&history](){
            if( calculator.execute() ) {
                edit.setText(die::NativeString::toString(calculator.currentNumber));
                history.lines().add(edit.getText()+"=");
                history.scrollTo(history.lines().count());
            }
        });
        
        // anonymous button to define the cancel
        Button(calc,ControlParams()
            .start(numbers[0].pos() + Point(4*tdist,-3*tdist))
            .dims(base.dims())
            .text("C")
        )
        .onClick([&edit,&calculator](){
            calculator.reset();
            edit.setText("0");
        });
        
        
        // destroy control used for cloning
        calc.remove(base);
    }
    
    // set digit events
    for( int n = 0; n != numbers.size(); ++n ) {
        numbers[n].onClick([&numbers,n,&edit,&calculator](){
            if( calculator.addDigit(numbers[n].getText().toUTF8()[0]) ) {
                edit.setText(die::NativeString::toString(calculator.currentNumber));
            }
        });
    }
    
    // set operation events
    for( int n = 0; n != operations.size(); ++n ) {
        operations[n].onClick([&operations,n,&edit,&calculator,&history](){
            if( calculator.execOperation(operations[n].getText().toUTF8()[0]) ) {
                edit.setText(die::NativeString::toString(calculator.currentNumber));
                history.lines().add(edit.getText()+operations[n].getText());
                history.scrollTo(history.lines().count());
            }
        });
    }
    
    
    Application app;
    // process application messages until closing the calc window
    while( calc.visible() ) {
        app.waitForMessages();
        app.processMessages();
    }
    
    return 0;
}

