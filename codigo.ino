#include <LiquidCrystal_I2C.h>  // Biblioteca para controlar o LCD com interface I2C
#include <Wire.h>               // Biblioteca para comunicação I2C (usada por LCD e RTC)
#include <RTClib.h>             // Biblioteca para usar o módulo de relógio RTC DS1307
#include <EEPROM.h>             // Biblioteca para ler e gravar dados na memória EEPROM

// ====================
//  Configurações LCD I2C
// ====================
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD com endereço 0x27, 16 colunas e 2 linhas

// Caracteres personalizados para animação de onda no LCD
byte wave0[8] = { B00000, B00100, B01010, B10001, B10001, B01010, B00100, B00000 };
byte wave1[8] = { B00100, B01010, B10001, B01010, B00100, B00000, B00000, B00000 };
byte wave2[8] = { B01010, B10001, B01010, B00100, B00000, B00000, B00000, B00000 };
byte wave3[8] = { B10001, B01010, B00100, B00000, B00000, B00000, B00000, B00000 };
byte wave4[8] = { B01010, B00100, B00000, B00000, B00000, B00000, B00000, B00000 };
byte wave5[8] = { B00100, B00000, B00000, B00000, B00000, B00000, B00000, B00000 };

// Array com ponteiros para os frames da onda
byte* waves[6] = { wave0, wave1, wave2, wave3, wave4, wave5 };

// ====================
//  Configurações RTC
// ====================
RTC_DS1307 rtc; // Objeto para acessar o RTC

// ====================
//  Pinos Sensores e Atuadores
// ====================
const int trigPin = 9;    // Pino trigger do sensor ultrassônico
const int echoPin = 10;   // Pino echo do sensor ultrassônico
const int buzzerPin = 12; // Pino do buzzer

// Array com pinos dos LEDs
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8};  
const int totalLeds = sizeof(ledPins) / sizeof(ledPins[0]); // Quantidade de LEDs

// ====================
//  Variáveis EEPROM
// ====================
unsigned long lastSave = 0;  // Armazena tempo da última gravação
int eepromAddress = 0;       // Endereço atual da EEPROM para escrita

// ====================
//  Funções da Animação do LCD
// ====================

// Desenha uma linha de onda animada no LCD
void drawWaveLine(int length, int frameOffset, int row) {
  lcd.setCursor(0, row);
  for (int i = 0; i < 16; i++) {
    if (i < length) {
      int frame = (frameOffset + i) % 6;
      lcd.write(frame);
    } else {
      lcd.write(' ');
    }
  }
}

// Mostra texto linha por linha com efeito "fade-in"
void fadeInTextTwoLines(const char* line1, const char* line2) {
  lcd.setCursor(0, 0);
  for (int i = 0; line1[i] != '\0'; i++) {
    lcd.print(line1[i]);
    delay(100);
  }
  lcd.setCursor(0, 1);
  for (int i = 0; line2[i] != '\0'; i++) {
    lcd.print(line2[i]);
    delay(100);
  }
}

// Preenche o LCD com a animação de onda
void animationFillScreen() {
  for (int total = 0; total <= 32; total++) {
    int row0_len = min(total, 16);
    int row1_len = max(0, total - 16);
    for (int frame = 0; frame < 6; frame++) {
      drawWaveLine(row0_len, frame, 0);
      drawWaveLine(row1_len, frame, 1);
      delay(40);
    }
  }
}

// ====================
//  Setup
// ====================
void setup() {
  Serial.begin(9600);   // Inicia comunicação serial
  Wire.begin();         // Inicia comunicação I2C

  lcd.init();           // Inicializa LCD
  lcd.backlight();      // Liga a luz de fundo do LCD

  // Cria os caracteres personalizados no LCD
  for (int i = 0; i < 6; i++) {
    lcd.createChar(i, waves[i]);
  }

  rtc.begin();          // Inicializa o RTC
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Ajusta data/hora com base no computador
  }

  // Define pinos do sensor ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Define pino do buzzer
  pinMode(buzzerPin, OUTPUT);

  // Define pinos dos LEDs como saída
  for (int i = 0; i < totalLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  lcd.clear();           // Limpa o LCD

  animationFillScreen(); // Exibe animação inicial
  delay(300);

  lcd.clear();
  fadeInTextTwoLines("FIAP sensor de", "enchentes"); // Nome do projeto
  delay(1500);
  lcd.clear();

  // Exibe os primeiros bytes da EEPROM no monitor serial (debug)
  Serial.println("Dados da EEPROM:");
  for (int i = 0; i < 8; i++) {
    Serial.print("Byte ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(EEPROM.read(i));
  }
}

// ====================
//  Loop Principal
// ====================
void loop() {
  // Envia pulso para iniciar medição do sensor ultrassônico
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lê duração do eco
  long duration = pulseIn(echoPin, HIGH);

  // Calcula a distância em cm
  float distance = (duration * 0.0343) / 2.0;
  if (distance > 400.0) distance = 400.0;

  int distanceInt = (int)distance; // Converte para inteiro

  // Acende LEDs conforme a distância (mais próximo → mais LEDs)
  digitalWrite(2, distanceInt <= 400 ? HIGH : LOW);
  digitalWrite(3, distanceInt <= 350 ? HIGH : LOW);
  digitalWrite(4, distanceInt <= 300 ? HIGH : LOW);
  digitalWrite(5, distanceInt <= 250 ? HIGH : LOW);
  digitalWrite(6, distanceInt <= 200 ? HIGH : LOW);
  digitalWrite(7, distanceInt <= 150 ? HIGH : LOW);

  // Se a distância for muito curta, acende LED crítico e buzzer
  if (distanceInt <= 20) {
    digitalWrite(8, millis() % 500 < 250 ? HIGH : LOW); // Pisca
    tone(buzzerPin, 1000);  // Ativa buzzer com frequência de 1000Hz
  } else {
    digitalWrite(8, LOW);
    noTone(buzzerPin);     // Desliga buzzer
  }

  // Define status com base na distância
  String status;
  byte statusByte;
  if (distanceInt >= 0 && distanceInt <= 20) {
    status = "RISCO!";
    statusByte = 2;
  } else if (distanceInt > 20 && distanceInt <= 150) {
    status = "ATENCAO";
    statusByte = 1;
  } else {
    status = "SEGURO";
    statusByte = 0;
  }

  // Pega hora atual do RTC
  DateTime now = rtc.now();

  // Atualiza LCD com distância e horário
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance, 1);  // Mostra uma casa decimal
  lcd.print("cm    ");

  lcd.setCursor(0, 1);
  lcd.print(status);
  lcd.print(" ");

  // Exibe hora formatada com dois dígitos
  if (now.hour() < 10) lcd.print("0");
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());
  lcd.print("    ");

  // Salva os dados na EEPROM a cada 10 segundos
  if (millis() - lastSave >= 10000) {
    EEPROM.write(eepromAddress++, now.day());
    EEPROM.write(eepromAddress++, now.month());
    EEPROM.write(eepromAddress++, now.year() - 2000); // Salva apenas os dois últimos dígitos
    EEPROM.write(eepromAddress++, now.hour());
    EEPROM.write(eepromAddress++, now.minute());
    EEPROM.write(eepromAddress++, now.second());
    EEPROM.write(eepromAddress++, (byte)distanceInt);
    EEPROM.write(eepromAddress++, statusByte);

    // Mostra no monitor serial o que foi salvo
    Serial.println("Dados salvos na EEPROM:");
    Serial.print("Data: ");
    Serial.print(now.day()); Serial.print("/");
    Serial.print(now.month()); Serial.print("/");
    Serial.println(now.year());

    Serial.print("Hora: ");
    Serial.print(now.hour()); Serial.print(":");
    Serial.println(now.minute());

    Serial.print("Distancia: ");
    Serial.println(distanceInt);

    Serial.print("Status: ");
    Serial.println(status);

    // Reinicia o endereço se passar de 900 (evita ultrapassar limite da EEPROM)
    if (eepromAddress > 900) eepromAddress = 0;
    lastSave = millis(); // Atualiza tempo da última gravação
  }

  delay(500); // Aguarda meio segundo antes da próxima leitura
}
