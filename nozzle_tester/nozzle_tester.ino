const int relayPin = 13;  // Пин, к которому подключено реле
const int blinkInterval_on = 2;  // Интервал включения в миллисекундах (1 секунда)
const int blinkInterval_off = 100;  // Интервал выключения в миллисекундах (1 секунда)

void setup() {
  pinMode(relayPin, OUTPUT);  // Настраиваем пин реле как выход
  digitalWrite(relayPin, HIGH);  // Выключаем реле (если оно нормально-замкнуто)
  // Если реле нормально-разомкнуто (NO), используйте LOW для выключения
}

void loop() {
  digitalWrite(relayPin, HIGH);  // Включаем реле (лампочка гаснет)
  delay(blinkInterval_on);          // Ждём
  digitalWrite(relayPin, LOW);   // Выключаем реле (лампочка загорается)
  delay(blinkInterval_off);          // Ждём
  
}