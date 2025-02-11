  #include <stdlib.h>

  char buff[10];

  void setup() {
    Serial.begin(9600);
    pinMode(A0, INPUT);
  }

  void loop() {
    // Read normal Arduino value
    int in0 = analogRead(A0);
    float val0 = in0 * 5.0 / 1024.0;

    // Read correct supply voltage
    float supply = readVcc() / 1000.0;
    float val0Corrected = supply / 5 * val0;

    // Print values to Serial Monitor
  
    dtostrf(val0Corrected, 4, 3, buff);
    Serial.print("Corr: ");
    Serial.println(buff);
    
    delay(500);
  }

  long readVcc() {
    long result;
    // Read 1.1V reference against AVcc
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while (bit_is_set(ADCSRA, ADSC));
    result = ADCL;
    result |= ADCH << 8;
    result = 1126400L / result; // Calculate Vcc (in mV); 1126400 = 1.1*1024*1000
    return result;
  }
