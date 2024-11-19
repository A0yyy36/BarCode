int sensorPin = A0;
int threshold = 900; // 光の強度の閾値
int barCount = 60; // バーコードのバーの数

// 左側 黒の和：偶数(白の和：奇数)
int oddL[10][4] = {
    {1,1,2,3},{1,2,2,2},{2,2,1,2},{1,1,4,1},{2,3,1,1},
    {1,3,2,1},{4,1,1,1},{2,1,3,1},{3,1,2,1},{2,1,1,3}
};

// 左側 黒の和：奇数(白の和：偶数)
int evenL[10][4] = {
    {3,2,1,1},{2,2,2,1},{2,1,2,2},{1,4,1,1},{1,1,3,2},
    {1,2,3,1},{1,1,1,4},{1,3,1,2},{1,2,1,3},{3,1,1,2}
};

// 左側1桁目 黒の総和の偶奇
int firstLeft[10][6] = {
    {0,0,0,0,0,0},{0,0,1,0,1,1},{0,0,1,1,0,1},{0,0,1,1,1,0},
    {0,1,0,0,1,1},{0,1,1,0,0,1},{0,1,1,1,0,0},{0,1,0,1,0,1},
    {0,1,0,1,1,0},{0,1,1,0,1,0}
};

// 右側
int judgeR[10][4] = {
    {3,2,1,1},{2,2,2,1},{2,1,2,2},{1,4,1,1},{1,1,3,2},
    {1,2,3,1},{1,1,1,4},{1,3,1,2},{1,2,1,3},{3,1,1,2}
};

// センサの値を15回読み取り平均値を返す
int lightRead(void) {
    int sum = 0;
    for (int i = 0; i < 15; i++) {
        sum += analogRead(sensorPin);
    }
    return sum / 15;
}

void setup() {
    Serial.begin(9600);
    pinMode(sensorPin, INPUT);
}

void loop() {
  unsigned long time = 0;
  unsigned long beforeTime = 0;
  unsigned long afterTime = 0;
  unsigned long barWidths[barCount] = {0};

  unsigned long base[barCount + 1] = {0}; // 基準幅
  double value = 0; // 今状態
  double result = 0; // 幅の計算結果
  unsigned long round = 0; // 四捨五入

  int rate[barCount] = {0}; 
  int barIndex = 0;
  double sensorValue; //= lightRead();
  int groupBar[12][4] = {0}; //バーを4つずつグループに分けたものを格納
  int judge[12] = {0}; //偶数・奇数判定
  int janNum[12] = {0}; //バーの幅をJANコードに変換したもの格納
  int firstNum = 0; //最初の数字を格納
  int state = (sensorValue > threshold) ? 1 : 0;
  beforeTime = micros(); //μ 秒計測

  //バーの幅計測
  while(barCount > barIndex) {
    sensorValue = lightRead();
    int nextState = (sensorValue > threshold) ? 1 : 0;
    //色が変化したら計測
    if(state != nextState) {
      afterTime = micros();
      time = afterTime - beforeTime;
      barWidths[barIndex++] = time;
      //後状態を現状態に移行
      beforeTime = afterTime;
      state = nextState;
      //計測したバーの情報表示
      Serial.print("No.");
      Serial.println(barIndex);
      Serial.print("状態: ");
      Serial.println(state == 1 ? "白" : "黒");
      Serial.print("時間: ");
      Serial.println(time);
    }
  }
  Serial.println("");

  //基準幅計算
  base[1] = barWidths[1];
  for (int i = 1; i < barCount - 1; i++) {
    value = barWidths[i + 1]; // 今状態の値挿入
    result = value / base[i]; // 今状態の幅を計算
    round = (int)(result + 0.5); // 基準に用いる四捨五入
    if(round > 4) {
      round = 4;
    } 
    else if(round < 0.5) {
      round = 1;
    }
    base[i + 1] = barWidths[i + 1] / round; // 基準幅を計算
    rate[i] = round;

/*
    Serial.print("Line");
    Serial.println(i + 2);
    Serial.print("幅:");
    Serial.println(barWidths[i + 1]);
    Serial.print("基準幅:");
    Serial.println(base[i]);
    Serial.print("比率:");
    Serial.println(round);
    Serial.println();
    */
  }

  //4つずつグループに分ける
  int groupIndex1 = 0;
  //2から26までの範囲を0から5までの配列に格納
  for(int i = 3; i < 28; i += 4) {
    for(int j = 0; j < 4; j++) {
      groupBar[groupIndex1][j] = rate[i + j];
    }
    groupIndex1++;
  }
  int groupIndex2 = 6;
  //31から55までの範囲を6から11までの配列に格納
  for(int i = 32; i < 57; i += 4) {
    for(int j = 0; j < 4; j++) {
      groupBar[groupIndex2][j] = rate[i + j];
    }
    groupIndex2++;
  }

  //左半分をJANコード標準を用いて変換
  for(int i = 0; i < 6; i++) {
    //1つのグループの中の黒い部分を足す
    int sumB = groupBar[i][1] + groupBar[i][3];
    if(sumB % 2 == 0) {
      judge[i] = 1; //偶数の時:1
    }
    else {
      judge[i] = 0; //奇数の時:0
    }
  }
  for(int i = 0; i < 6; i++) {
    if(judge[i] == 0) {
      janNum[i] = 0; // 初期値を 0 に設定
      for(int k = 0; k < 10; k++) {
        int j = 0;
        for(int l = 0; l < 4; l++) {
          if(groupBar[i][l] == evenL[k][l]) {
            j++;
          }
        }
        if(j >= 3) { //3個以上一致した場合
          janNum[i] = k;
          break;
        }
      }
    }
    else {
      janNum[i] = 0; // 初期値を 0 に設定
      for(int k = 0; k < 10; k++) {
        int j = 0;
        for(int l = 0; l < 4; l++) {
          if(groupBar[i][l] == oddL[k][l]) {
            j++;
          }
        }
        if(j >= 3) { //3個以上一致した場合
          janNum[i] = k;
          break;
        }
      }
    }
  }

  //最初の数字を計算
  for(int i = 0; i < 10; i++) {
    bool match = true;
    for(int j = 0; j < 6; j++) {
      if(judge[j] != firstLeft[i][j]) {
        match = false;
        break;
      }
    }
    if(match) {
      firstNum = i;
      break;
    }
  }

  //右半分をJANコード標準を用いて変換
  for(int i = 6; i < 12; i++) {
    janNum[i] = 0; // 初期値を 0 に設定
    for(int k = 0; k < 10; k++) {
      int j = 0;
      for(int l = 0; l < 4; l++) {
        if(groupBar[i][l] == judgeR[k][l]) {
          j++;
        }
      }
      if(j >= 3) { //変更箇所: 3個以上一致した場合
        janNum[i] = k;
        break;
      }
    }
  }

  //結果を出力
  Serial.println("読取結果:");
  Serial.print(firstNum);
  for(int i = 0; i < 12; i++) {
    Serial.print(janNum[i]);
  }

  Serial.println("");
  delay(100000);
}