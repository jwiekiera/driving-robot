#define inputRight1 12 //pierwszy pin determinujacy kierunek prawego koła (HIGH - do przodu)
#define inputRight2 13 //pierwszy pin determinujacy kierunek prawego koła (HIGH - do tyłu)
#define enablePinRight 11 // PWM prawego koła (nadawanie prędkości obrotowej prawego koła)

#define inputLeft1 8 //pierwszy pin determinujacy kierunek lewego koła (HIGH - do przodu)
#define inputLeft2 9 //pierwszy pin determinujacy kierunek lewego koła (HIGH - do tyłu)
#define enablePinLeft 10 // PWM lewego koła (nadawanie prędkości obrotowej lewego koła)

#define trigPinFront 7 //wyjście odpowiadające wejściu zezwalającemu przedmiego czujnika
#define echoPinFront 6 //wejście odpowiadające wyjściu przedniego czujnika zwracającego pomiar

#define trigPinRight 4 //wyjście odpowiadające wejściu zezwalającemu prawego czujnika
#define echoPinRight 5 //wejście odpowiadające wyjściu prawego czujnika zwracającego pomiar

#define trigPinLeft 2 //wyjście odpowiadające wejściu zezwalającemu lewego czujnika
#define echoPinLeft 3 //wejście odpowiadające wyjściu lewego czujnika zwracającego pomiar

#define barier 30.0 //odległość wyrażona w centymetrach, w przypadku której robot ma zatrzymać się przed dalszą jazdą 

#define degree90time 1200 //czas w mikrosekundach, przez jaki ma wykonywać się operacja skrętu, by robot skręcił o 90 stopni (na testowym podłożu)

#define motorSpeed 100 //częstotliwość decydująca o prędkości obrotowej silników

void setup() {
  Serial.begin (9600);

  pinMode(inputRight1, OUTPUT); //ustawienie pierwszego pinu prawego koła na wyjście
  pinMode(inputRight2, OUTPUT); //ustawienie drugiego pinu prawego koła na wyjście
  pinMode(enablePinRight, OUTPUT); //ustawienie PWM prawego koła na wyjście (zakres 0-225)
  
  pinMode(inputLeft1, OUTPUT); //ustawienie pierwszego pinu lewego koła na wyjście
  pinMode(inputLeft2, OUTPUT); //ustawienie drugiego pinu lewego koła na wyjście
  pinMode(enablePinLeft, OUTPUT); //ustawienie PWM lewego koła na wyjście (zakres 0-225)

  pinMode(trigPinFront, OUTPUT); // ustawienie pinów podłączonych do trig czujnika jako wyjście
  pinMode(echoPinFront, INPUT); // ustawienie pinów podłączonych do echo czujnika jako wejście

  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);

  analogWrite(enablePinRight, motorSpeed); //nadanie prędkości obrotowej silników
  analogWrite(enablePinLeft, motorSpeed); 
}

void loop() {
  // algorytm omijania napotkanej przeszkody 
  if(measureDistance('f') > barier){
    goForward();
    delay(100);
  }
  else{
    stop(1000);
    if(measureDistance('r') > barier){
      turnRight(degree90time);
    } else if(measureDistance('l') > barier) {
      turnLeft(degree90time);
    } else{
        goBack(1000);
        if(measureDistance('r') > barier){
          turnRight(degree90time);
        } else if(measureDistance('l') > barier) {
          turnLeft(degree90time);
        }
    }
  }
}

// pomiar odległości na wybranym czujniku
// parametr: direction - wybór kierunku pomiaru:
//f - front, przedmi czujnik
//r - right, prawy czujnik
//l - left, lewy czujnik
float measureDistance(char direction) {
  float czas, dystans;
  if(direction == 'f'){
    digitalWrite(trigPinFront, HIGH); // nadanie sygnału czujnika, rozpoczęcie pomiaru odkległości
    delayMicroseconds(10); // odczekanie 10 mikrosekund
    digitalWrite(trigPinFront, LOW);
 
    czas = pulseIn(echoPinFront, HIGH); // odczyt czasu pomiaru 
  }
  else if(direction == 'r'){
    digitalWrite(trigPinRight, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinRight, LOW);
 
    czas = pulseIn(echoPinRight, HIGH);

  }
  else if(direction == 'l'){
    digitalWrite(trigPinLeft, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinLeft, LOW);
 
    czas = pulseIn(echoPinLeft, HIGH);
  }

  dystans = czas / 58.00; // obliczenie odległości od przeszkody (w centymetrach)
  // testowe wypisanie odkległości
  Serial.print(direction);
  Serial.print(": ");
  Serial.println(dystans);

  return dystans;
}

// skręt w prawo 
// parametr: time - czas skrętu
void turnRight(long time){
  rightRun(1);
  leftRun(0);
  delay(time);
}

// skręt w lewo 
// parametr: time - czas skrętu
void turnLeft(long time){
  rightRun(0);
  leftRun(1);
  delay(time);
}

// jazda do przodu
// parametr: time - czas jazdy
void goForward(){
  rightRun(1);
  leftRun(1);  
}

// jazda do tyłu
// parametr: time - czas jazdy
void goBack(long time){
  rightRun(-1);
  leftRun(-1); 
  delay(time);
}

// zatrzymanie się robota
// parametr: time - czas zatrzymania
void stop(int time){
  rightRun(0);
  leftRun(0);
  delay(time);
}

// nadanie kierunku prawego koła
// parametr: i - kierunek jazdy:
// -1 - koło jedzie do tyłu
//  0 - koło się zatrzymuje
//  1 - koło jedzie do przodu
void rightRun(int i){
  if(i == -1) { //back
    digitalWrite(inputRight1, LOW);
    digitalWrite(inputRight2, HIGH);
  }
  else if(i == 0){ //stop
    digitalWrite(inputRight1, LOW);
    digitalWrite(inputRight2, LOW);
  }
  else if(i == 1){ //forward
    digitalWrite(inputRight1, HIGH);
    digitalWrite(inputRight2, LOW);
  }
}

// nadanie kierunku lewego koła
// parametr: i - kierunek jazdy:
// -1 - koło jedzie do tyłu
//  0 - koło się zatrzymuje
//  1 - koło jedzie do przodu
void leftRun(int i){
  if(i == -1) { //back
    digitalWrite(inputLeft1, LOW);
    digitalWrite(inputLeft2, HIGH);
  }
  else if(i == 0){ //stop
    digitalWrite(inputLeft1, LOW);
    digitalWrite(inputLeft2, LOW);
  }
  else if(i == 1){ //forward
    digitalWrite(inputLeft1, HIGH);
    digitalWrite(inputLeft2, LOW);
  }
}