# IR Proximity Sensor + DFPlayer Mini (Random Fortune Teller Capybara)

<p>
  This project creates an interactive “fortune teller” system that plays a random audio message whenever someone approaches. Using an infrared proximity sensor and a DFPlayer Mini MP3 module, the Arduino detects nearby users and triggers one of several prerecorded sound clips.
</p>


## 🎥 Demo Video

[![Watch the demo on YouTube](https://img.youtube.com/vi/fhjD2fes6jI/0.jpg)](https://youtube.com/shorts/fhjD2fes6jI?feature=share)




 ## ⚙️ Equipment & Components

<p>
  This project uses infrared sensing and audio playback to create an interactive experience.
</p>

| Component | Quantity | Notes |
|:---|:---:|:---|
| **[Arduino Uno](https://airtable.com/appCpmcjYA1vwj8jn/tblOHGyZIGOZuJhCj/viwcQ6Lj5fpoG6Hvh/recQ1P43HKyVMjA79?blocks=hide)** | 1 | Main microcontroller |
| **[Breadboard](https://airtable.com/appCpmcjYA1vwj8jn/tblZz5NUA546g9J6o/viwu3SMJU1AEGhMGK/recF514LASWf2n9LH?blocks=hide)** | 1 | For building the circuit |
| **[IR Proximity Sensor](https://www.adafruit.com/product/164)** | 1 | Detects when someone is nearby |
| **[DFPlayer Mini MP3 Module](https://store-usa.arduino.cc/products/dfplayer-a-mini-mp3-player)** | 1 | Plays stored audio files |
| **[MicroSD Card](https://www.adafruit.com/product/1294)** | 1  | Stores MP3 files (formatted FAT32) |
| **[Speaker](https://store-usa.arduino.cc/products/stereo-enclosed-speaker-3w-8%CF%89)** | 1  | Outputs sound |
| **[1kΩ / 10kΩ Resistors (optional)](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JA10K0/21720224?gad_campaignid=20682878391&gbraid=0AAAAADrbLlirN4jkM1ESFmaahexqC2yYP)** | As needed  | For signal stability |
| **[Jumper Wires](https://www.digikey.com/en/products/detail/sparkfun-electronics/12795/5993860?gad_campaignid=20232005509&gbraid=0AAAAADrbLlgQjXHdDWPH-Y_gmE0kpnNl2)** | 1 Bundle | For all connections |


  <p>
    <strong>Important Note:</strong>

Make sure your MicroSD card is formatted to FAT32 and contains MP3 files named:
0001.mp3 → 0010.mp3 before starting.  

  </p>

<details>
  <summary>
     <h2>Step 1: IR Proximity Sensor (Distance Detection)</h2>
  </summary>
  <br>
  
  <p>
The infrared proximity sensor detects nearby objects by emitting IR light and measuring how much is reflected back. When a person moves close, the reflected signal increases.
In this project, the sensor is connected to an analog input (A0). The Arduino reads this signal and compares it to a threshold to decide whether someone is present. 
    
  </p>

For this project, connect the IR Proximity sensor to the arduino and DFPLayer to the arduino as follows:

| IR Proximity Sensor | Arduino Pin |
|:---|:---:|
| VCC | 5V |
| GND | GND |
| OUT | A0 |

| DFPLayer | Arduino Pin |
|:---|:---:|
| VCC | 5V |
| GND | GND |
| TX | Pin 2 |
| RX | Pin 3 |
| SPK+/SPK- | Speaker |

Optional: Use a 1kΩ resistor between Arduino TX and DFPlayer RX for signal protection.<br>


  <p>
   To make the coding process easier, we can utilize a library that has simplified the complex operations needed to utilize the MP3 Player To install the DFRobotDFPlayerMini:
   
   1. First open your Arduino IDE program. Then click on the Library Manager icon on the left sidebar.
   2. Type “DFRobotDFPlayerMini” in the search box to filter your results.
   3. Look for the DFRobotDFPlayerMini library.
   4. Click the Install button to add it to your Arduino IDE. <br>

  </p>
</details>

  <details>
  <summary>
     <h2>Step 2: Wiring The Prototype & Library Setup</h2>
  </summary>
  <br>

**System Logic:** <br />
    
1. Read IR sensor
2. Detect presence
3. Check if audio is playing
4. If someone enters → play random sound
5. Wait until finished
6. Reset when user leaves

  <p>

This prevents repeated triggering while someone is still standing nearby. <br>

  </p>
  
**Arduino Code:** <br />
```C++
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

```

<p>
Run the code above and check the serial monitor. Place your hand in front of your sensor-- did the output change? Try moving your hand a bit farther or closer. If your sensor reading gets higer or lower accordingly, congrats, you did it! Move on to the next step. 
</p>
</details>

<details>
  <summary>
     <h2>Step 3: Prototype Installation</h2>
  </summary>
  <br>

To improve durability and presentation, you may design a custom enclosure that:
* Exposes the IR sensor
* Houses the speaker
* Secures the DFPlayer
* Protects wiring

**You can:** <br />
* 3D print a case
* Laser cut an acrylic box
* Embed it into an installation

**Recommended features:** <br />
* Front sensor window
* Rear SD access
* Sound vents
* Mounting holes
</details>

<details>
  <summary><h2>Step 4: Final Outcome</h2></summary>
  
When completed, this system:
* Detects approaching users
* Plays one random audio message (fell free to use AI to generate these, I used Elevenlabs)
* Waits until playback finishes
* Resets after user leaves
* Creates a responsive, playful experience<br> </br>

