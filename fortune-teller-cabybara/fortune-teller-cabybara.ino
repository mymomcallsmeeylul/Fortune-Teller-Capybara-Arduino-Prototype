#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mp3Serial(2, 3);
DFRobotDFPlayerMini player;

#define IR_PIN A0   // IR sensor on analog input

bool inRange = false;       // is someone detected right now?
bool isPlaying = false;     // is a track currently playing?

void setup() {
  Serial.begin(9600);
  mp3Serial.begin(9600);

  if (!player.begin(mp3Serial)) {
    Serial.println("DFPlayer not responding!");
    while(true) {}
  }

  Serial.println("DFPlayer is online!");
  player.volume(30);

  randomSeed(analogRead(A1));
}

void loop() {
  // --------- 1) READ IR SENSOR ---------
  int reading = analogRead(IR_PIN);
  Serial.println(reading);
  bool detected = (reading > 150);  // adjust threshold as needed

  // --------- 2) CHECK IF FINISHED PLAYING ---------
  if (player.available()) {
    uint8_t type = player.readType();
    int value = player.read();  // track number finished
    if (type == DFPlayerPlayFinished) {
      isPlaying = false;  // now we can trigger a new track
      Serial.println("Track finished");
    }
  }

  // --------- 3) IF detected + NOT playing + NOT previously in range → PLAY ---------
  if (detected && !inRange && !isPlaying) {
    playRandomFortune();
    isPlaying = true;
    inRange = true;
  }

  // --------- 4) If nobody detected, reset inRange ---------
  if (!detected) {
    inRange = false;
  }

  delay(100);
}

// --------- 5) RANDOM TRACK SELECTOR ---------
void playRandomFortune() {
  int track = random(1, 11);  // 1–10
  Serial.print("Playing: ");
  Serial.println(track);
  player.play(track);
}
