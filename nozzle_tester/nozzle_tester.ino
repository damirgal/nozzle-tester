const int relayPin = 13;        // Пин, к которому подключено реле
const int buttonPin = 3;        // Пин, к которому подключена кнопка

// Массивы возможных значений для интервалов
const int intervals_on[] = {2, 4, 6};
const int intervals_off[] = {10, 20, 30, 40};
const int numIntervalsOn = 3;   // Количество вариантов для ON
const int numIntervalsOff = 4;  // Количество вариантов для OFF

int blinkInterval_on = 2;       // Текущий интервал включения
int blinkInterval_off = 10;    // Текущий интервал выключения

int currentIntervalIndex_on = 0;   // Текущий индекс для ON
int currentIntervalIndex_off = 0;  // Текущий индекс для OFF

int lastButtonState = HIGH;     // Предыдущее состояние кнопки
unsigned long lastPressTime = 0; // Время последнего нажатия

void setup() {
  pinMode(relayPin, OUTPUT);    // Настраиваем пин реле как выход
  pinMode(buttonPin, INPUT_PULLUP); // Настраиваем пин кнопки как вход с подтяжкой к питанию
  
  digitalWrite(relayPin, HIGH); // Выключаем реле (если оно нормально-замкнуто)
  
  Serial.begin(9600);           // Инициализируем Serial для отладки
  Serial.println("=== СКЕТЧ ЗАПУЩЕН ===");
  printCurrentState();
}

void loop() {
  // Обработка нажатия кнопки
  handleButton();
  
  // Управление реле
  digitalWrite(relayPin, HIGH);  // Включаем реле (лампочка гаснет)
  delay(blinkInterval_on);       // Ждём
  digitalWrite(relayPin, LOW);   // Выключаем реле (лампочка загорается)
  delay(blinkInterval_off);      // Ждём
}

void handleButton() {
  // Читаем состояние кнопки
  int currentButtonState = digitalRead(buttonPin);
  
  // Проверяем нажатие (переход с HIGH на LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    // Защита от дребезга - проверяем время с последнего нажатия
    if (millis() - lastPressTime > 500) { // 500 мс между нажатиями
      changeInterval();
      lastPressTime = millis();
    }
  }
  
  lastButtonState = currentButtonState;
}

void changeInterval() {
  // Сначала перебираем все значения intervals_off
  currentIntervalIndex_off = (currentIntervalIndex_off + 1) % numIntervalsOff;
  blinkInterval_off = intervals_off[currentIntervalIndex_off];
  
  // Если дошли до начала intervals_off, переходим к следующему intervals_on
  if (currentIntervalIndex_off == 0) {
    currentIntervalIndex_on = (currentIntervalIndex_on + 1) % numIntervalsOn;
    blinkInterval_on = intervals_on[currentIntervalIndex_on];
  }
  
  printCurrentState();
  indicateChange();
}

void printCurrentState() {
  Serial.println("-----------------------------------");
  Serial.print("Комбинация: ");
  Serial.print(currentIntervalIndex_on + 1);
  Serial.print(" из ");
  Serial.print(numIntervalsOn);
  Serial.print(" (ON) | ");
  Serial.print(currentIntervalIndex_off + 1);
  Serial.print(" из ");
  Serial.print(numIntervalsOff);
  Serial.println(" (OFF)");
  Serial.print("blinkInterval_on: ");
  Serial.print(blinkInterval_on);
  Serial.println(" мс");
  Serial.print("blinkInterval_off: ");
  Serial.print(blinkInterval_off);
  Serial.println(" мс");
  Serial.println("-----------------------------------");
}

void indicateChange() {
  // Индикация миганием - количество миганий соответствует индексу ON + 1
  int blinkCount = currentIntervalIndex_on + 2;
  
  for (int i = 0; i < blinkCount; i++) {
    digitalWrite(relayPin, LOW);
    delay(500);
    digitalWrite(relayPin, HIGH);
    if (i < blinkCount - 1) { // Не делать паузу после последнего мигания
      delay(500);
    }
  }
  
  // Возвращаемся к нормальной работе
  digitalWrite(relayPin, HIGH);
}