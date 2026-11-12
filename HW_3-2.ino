// 腳位設定
const int ledPin = 3;       // LED燈設定於 D3 腳位
const int buttonPin = 2;    // 按鈕模組腳位 (請依實際接線調整，例如 D2)

// 模式變數：0:衡亮, 1:慢閃, 2:不快不慢閃, 3:快閃
int mode = 0; 

// 按鈕防彈跳（Debounce）相關變數
int lastButtonState = HIGH;
int currentButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 防彈跳延遲時間 (ms)

// LED 閃爍時間控制變數
unsigned long lastBlinkTime = 0;
bool ledState = HIGH;

void setup() {
  pinMode(ledPin, OUTPUT);
  
  // 使用內部上拉電阻（未按下為 HIGH，按下為 LOW）
  // 若按鈕模組為高電位觸發，請依實際情況修正接點或改為 INPUT
  pinMode(buttonPin, INPUT_PULLUP); 

  // 初始狀態：衡亮
  digitalWrite(ledPin, HIGH);
}

void loop() {
  // 1. 按鈕讀取與切換邏輯 (防彈跳)
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currentButtonState) {
      currentButtonState = reading;

      // 當按鈕被按下時 (LOW)
      if (currentButtonState == LOW) {
        // 模式切換：0 -> 1 -> 2 -> 3 -> 0 (第4下回到衡亮)
        mode = (mode + 1) % 4; 
        
        // 切換模式時重置 LED 狀態
        ledState = HIGH;
        digitalWrite(ledPin, ledState);
        lastBlinkTime = millis();
      }
    }
  }
  lastButtonState = reading;

  // 2. 依據當前模式執行相應的 LED 燈狀態
  unsigned long currentMillis = millis();

  switch (mode) {
    case 0: 
      // 模式 0：衡亮
      digitalWrite(ledPin, HIGH);
      break;

    case 1: 
      // 模式 1：慢閃爍 (每 1000 ms 切換一次)[cite: 1]
      if (currentMillis - lastBlinkTime >= 1000) {
        lastBlinkTime = currentMillis;
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
      }
      break;

    case 2: 
      // 模式 2：不快不慢的閃爍 (每 400 ms 切換一次)[cite: 1]
      if (currentMillis - lastBlinkTime >= 400) {
        lastBlinkTime = currentMillis;
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
      }
      break;

    case 3: 
      // 模式 3：快閃爍 (每 100 ms 切換一次)[cite: 1]
      if (currentMillis - lastBlinkTime >= 100) {
        lastBlinkTime = currentMillis;
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
      }
      break;
  }
}