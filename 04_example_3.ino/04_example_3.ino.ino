#define PIN_LED 13
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT); // Missing semicolon added here
  Serial.begin(115200); // Initialize serial port
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  Serial.println("Hello World!");
  count = 0;
  toggle = LOW; // Initialize toggle to LOW (LED off)
  digitalWrite(PIN_LED, toggle); // Turn off LED
}

void loop() {
  Serial.println(++count);
  toggle = toggle_state(toggle); // Toggle LED value
  digitalWrite(PIN_LED, toggle); // Update LED status
  delay(1000); // Wait for 1,000 milliseconds
}

int toggle_state(int toggle) {
  return (toggle == LOW) ? HIGH : LOW; // Toggle state
}
