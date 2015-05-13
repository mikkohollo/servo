//define states either as enum or constants
#define KEY_RELEASED 0
#define POSSIBLE_KEYPRESS 1
#define KEY_PRESSED 2
#define POSSIBLE_KEY_RELEASE 3

#include <Servo.h>

unsigned char keypress_detected = 0;
int hold_count = 0;
class Button
{
  private:
    unsigned char detector_state = KEY_RELEASED;
    unsigned long entry_time;
    unsigned long exit_time;
    unsigned long autorepeat_time;
    int button_pin;

  public:
    Button(int pin)
    {
      button_pin = pin;
      pinMode(pin, INPUT_PULLUP);
    }

    unsigned char detect_key_state()
    {

      switch (detector_state) {

        case KEY_RELEASED:
          if (digitalRead(button_pin) == LOW)
          {
            entry_time = millis();
            detector_state = POSSIBLE_KEYPRESS;
          }
          break;

        case POSSIBLE_KEYPRESS:
          if (digitalRead(button_pin) == HIGH)
          {
            detector_state = KEY_RELEASED;
          }
          else if (millis() - entry_time > 30)  // PRESS DETECTED
          {
            autorepeat_time = millis();         // initialize autorepeat timer
            keypress_detected++;
            detector_state = KEY_PRESSED;
            hold_count = 0;
          }
          break;

        case KEY_PRESSED:
          if (digitalRead(button_pin) == HIGH)
          {
            detector_state = POSSIBLE_KEY_RELEASE;
            exit_time = millis();
          }
          else if (millis() - autorepeat_time > 500) // REPEAT KEYPRESS EVERY 0,5s
          {
            autorepeat_time = millis();
            keypress_detected++;
            hold_count++;
          }
          break;

        case POSSIBLE_KEY_RELEASE:
          if (digitalRead(button_pin) == LOW)
          {
            detector_state = KEY_PRESSED;
          }
          else if (millis() - exit_time > 30) // RELEASE DETECTED
          {
            detector_state = KEY_RELEASED;
          }
          break;
      }
      return detector_state;
    }
};

Button b1(6);
Button b2(7);
Servo myservo;  // create servo object to control a servo

int velocity = 1500;
const int counter = 67;
int acceleration[counter] = {
  1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0,
  -1, -2, -3, -4, -5, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -5, -4, -3, -2, -1, 0
};
int servo_position = 0;

long button_timer = 0;
long function_timer = 0;
long servo_timer = 0;
bool stopping = false;
bool starting = false;

void setup() {
  Serial.begin(9600);
  button_timer = millis();
  function_timer = millis();
  servo_timer = millis();
  myservo.attach(9);
}
void loop() {
  int button1_state;
  int button2_state;

  if ( millis() - button_timer > 10) {

    Serial.print(" Presses: ");
    Serial.println(keypress_detected);

    Serial.print("     Hold count: ");
    Serial.print(hold_count);

    function_timer = millis();
    button1_state = b1.detect_key_state();
    button2_state = b2.detect_key_state();
    Serial.print("      Execution time: ");
    Serial.print(millis() - function_timer);
    Serial.print("      servo position: ");
    Serial.print(servo_position);

    button_timer = millis();
  }

  if (button1_state == KEY_PRESSED) {
    if (stopping == false) {
      starting = true;
    }
  }
  else if (button1_state == KEY_RELEASED) {
    if (starting == true) {
      starting = false;
      stopping = true;
    }
  }

  if ( millis() - servo_timer > 100  && ( starting == true || stopping == true ) ) {
    if (servo_position < counter) {
      velocity = velocity + acceleration[servo_position];
      myservo.writeMicroseconds(velocity);
      if ( ( servo_position <= 34 && starting == true ) || stopping == true ) {
        servo_position++;
      }

    }
    else if (servo_position == 67 && stopping == true) {
      servo_position = 0;
      stopping = false;
    }

    servo_timer = millis();
  }
}
