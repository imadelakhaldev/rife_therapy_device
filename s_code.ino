#define FREQ_OUT_PIN 9 // Frequency output pin.
#define BUZZER_PIN 4 // Buzzer pin to indicate end of process.
#define WAIT_DELAY 300000 // Wait delay between each frequency in millisecond (preferably 5 - 15 minutes each frequency).

int FREQS_LIST_COUNT = 9; // Number of frequencies in the list below. Used to allocate memory, and to loop through frequency list.
int FREQS_LIST[9] = {1550,880,802,832,787,776,422,727,20}; // List of frequencies to use (in this example frequencies to cure Constipation), you can find a full list of frequencies in other files.

void setup() {
  Serial.begin(9600);
  pinMode(FREQ_OUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  Serial.println("PROCESS STARTED.");

  // Main program loop.
  for(int i = 0; i < FREQS_LIST_COUNT; i++) {
    Serial.print(set_frequency(FREQS_LIST[i]));
    Serial.println(" HZ");
    delay(WAIT_DELAY);

    // Blink to indicate change of frequency.
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Buzzer to indicate end of process. Repeating the same process after a while.
  tone(BUZZER_PIN, 100);
  Serial.println("DONE PROCESSING. REPEATING AFTER 30 SECONDS.");
  delay(30000);
  noTone(BUZZER_PIN);
}

// Frequency generation function (References: https://playground.arduino.cc/Main/TimerPWMCheatsheet/, https://www.instructables.com/Arduino-Timer-Interrupts/).
long set_frequency(long freq) {
  TCCR1A = 0b01000000; 
  uint16_t ocr;
  
  if(freq < 134) {
    TCCR1B = 0b00001010; 
    ocr = 1000000UL/freq - 1;
    freq = 1000000UL/(ocr+1);
  } else {
    TCCR1B = 0b00001001;
    ocr = 8000000UL/freq - 1;
    freq = 8000000UL/(ocr+1);
  }
  
  OCR1AH = highByte(ocr);
  OCR1AL = lowByte(ocr);
  TIMSK1 = 0;
  
  return freq;
}
