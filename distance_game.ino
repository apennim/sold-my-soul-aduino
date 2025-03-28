#define TRIG_PIN 12
#define ECHO_PIN 11
#define LED_PIN 8
#define BUZZER_PIN 9

// 遊戲參數
const int TARGET_DISTANCE = 20; // 目標距離（公分）
const int TOLERANCE = 2;        // 允許誤差範圍（公分）

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // 初始化時先確保 TRIG 腳位為低電位
  digitalWrite(TRIG_PIN, LOW);
  delay(100);  // 等待感應器穩定
  
  Serial.println("距離遊戲開始！");
  Serial.println("請將物體放在感應器前方約 20 公分處");
}

void loop() {
  // 測量距離
  long duration;
  float distance;
  
  // 發送觸發信號
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // 計算回傳時間
  duration = pulseIn(ECHO_PIN, HIGH, 30000); // 設定 timeout 為 30ms
  
  // 除錯訊息
  Serial.print("回傳時間 (微秒): ");
  Serial.println(duration);
  
  // 檢查是否超時
  if (duration == 0) {
    Serial.println("警告：感應器沒有收到回聲！請檢查接線或移除障礙物");
    delay(1000);
    return;
  }
  
  // 將時間轉換為距離 (聲速 = 340m/s = 0.034 cm/微秒)
  distance = duration * 0.034 / 2;
  
  Serial.print("當前距離: ");
  Serial.print(distance);
  Serial.println(" 公分");
  
  // 判斷是否達到目標距離
  if (abs(distance - TARGET_DISTANCE) <= TOLERANCE) {
    // 成功！
    digitalWrite(LED_PIN, HIGH);
    playWinTone();
    Serial.println("太棒了！你找到正確的距離了！");
  } else {
    digitalWrite(LED_PIN, LOW);
    // 提示距離差異
    if (distance < TARGET_DISTANCE) {
      Serial.println("太近了！請後退一點");
      playErrorTone();
    } else {
      Serial.println("太遠了！請靠近一點");
      playErrorTone();
    }
  }
  
  delay(100); // 延遲以避免讀數過快，但不要太久
}

// 播放勝利音效
void playWinTone() {
  // 使用方波信號產生音效
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 100; j++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(500);  // 頻率約 1000Hz
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(500);
    }
    delay(50);
  }
}

// 播放錯誤音效
void playErrorTone() {
  // 低音錯誤提示音
  for(int i = 0; i < 100; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(2000);  // 頻率約 250Hz
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(2000);
  }
  delay(100);
}