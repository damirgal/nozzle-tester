const int relayPin = 13;        // Пин, к которому подключено реле
const int buttonPin = 2;        // Пин, к которому подключена кнопка

// Массив возможных значений для blinkInterval_off
const int intervals_off[] = {100, 1000, 5000};
const int numIntervals = 3;     // Количество вариантов интервалов

const int blinkInterval_on = 2; // Интервал включения
int blinkInterval_off = 100;    // Текущий интервал выключения

int currentIntervalIndex = 0;   // Текущий индекс в массиве интервалов
int lastButtonState = HIGH;     // Предыдущее состояние кнопки
unsigned long lastPressTime = 0; // Время последнего нажатия

void setup() {
  pinMode(relayPin, OUTPUT);    // Настраиваем пин реле как выход
  pinMode(buttonPin, INPUT_PULLUP); // Настраиваем пин кнопки как вход с подтяжкой к питанию
  
  digitalWrite(relayPin, HIGH); // Выключаем реле (если оно нормально-замкнуто)
  
  Serial.begin(9600);           // Инициализируем Serial для отладки
  Serial.println("Скетч запущен. Текущий интервал OFF: " + String(blinkInterval_off) + " мс");
}

void loop() {
  // Обработка нажатия кнопки
  handleButton();
  
  // Управление реле (согласно вашей логике)
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
  // Переходим к следующему интервалу (по кругу)
  currentIntervalIndex++;
  
  // Если дошли до конца массива, начинаем сначала
  if (currentIntervalIndex >= numIntervals) {
    currentIntervalIndex = 0;
  }
  
  blinkInterval_off = intervals_off[currentIntervalIndex];
  
  // Выводим информацию в Serial
  Serial.print("Интервал OFF изменён: ");
  Serial.print(blinkInterval_off);
  Serial.print(" мс (индекс: ");
  Serial.print(currentIntervalIndex);
  Serial.println(")");
  
  // Мигаем реле 3 раза для индикации изменения
  for (int i = 0; i < 3; i++) {
    digitalWrite(relayPin, LOW);
    delay(150);
    digitalWrite(relayPin, HIGH);
    delay(150);
  }
  
  // Возвращаемся к нормальной работе
  digitalWrite(relayPin, HIGH);
}