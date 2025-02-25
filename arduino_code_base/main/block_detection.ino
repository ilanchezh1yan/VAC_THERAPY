#define TACHOMETER_PIN 15

static uint16_t edge_count;

void IRAM_ATTR countEdges(void) {
  edge_count++;
}

void check_block(void) {

  pinMode(TACHOMETER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TACHOMETER_PIN), countEdges, CHANGE);
  delay(1000);
  noInterrupts();
  edge_count = 0;
  detachInterrupt(TACHOMETER_PIN);
}