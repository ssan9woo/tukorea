#include <Arduino.h>

class myButton
{
private:
    int pin;
    int button_state;
    time_t debounce_time;
    time_t before;

    int is_set;
public:
    enum
    {
        NPUSHED,
        PUSHED
    };
    myButton(){};
    myButton(int pin) : pin(pin){};
    void init();
    void attach(int pin) { this->pin = pin; };
    void set_debounce(int debounce_time);
    int get();
    void loop();
};

void myButton::init()
{
    pinMode(pin, INPUT_PULLUP);
    button_state = NPUSHED;
    debounce_time = 50;
}

void myButton::set_debounce(int debounce_time)
{
    this->debounce_time = debounce_time;
}

int myButton::get()
{

    int val = !digitalRead(pin);
/*
    if(val == button_state) return NPUSHED;
    button_state =(button_state == NPUSHED && val == PUSHED);
    return button_state;
*/
    yield();
    if(is_set)
    {
        yield();
        if(val == HIGH && millis() - before > debounce_time)
        {
            is_set = false;
            button_state = PUSHED;
            return PUSHED;
        }
        else 
        {
            return NPUSHED;
        }
    }
    else
    {
        yield();
        if(val == HIGH && button_state == PUSHED);
        else if(val == HIGH && button_state == NPUSHED)
        {
            before = millis();
            is_set = true;
        }
        else if(val == LOW) button_state = NPUSHED;
        return NPUSHED;
    }
    return NPUSHED;
}