#include <Keypad.h>// Libreria per tastierino
#include <LiquidCrystal.h> // Libreria per schermo lcd
#include <Servo.h>  // Libreria per i servo motori

int ledr = 5;  // LED rosso
int ledg = 4;  // LED verde
int accensione = 3;  // accensione razzo

LiquidCrystal lcd(13, 12, 11, 10, 9, 8); // pin schermo lcd

const byte RIGHE = 4; 
const byte COLONNE = 4; 
char hexaKeys[RIGHE][COLONNE]{ 
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[RIGHE] = {A0, A1, A2, A3}; 
byte colPins[COLONNE] = {A4, A5, 3, 2}; 

Keypad mioTastierino = Keypad(makeKeymap(hexaKeys), rowPins, colPins, RIGHE, COLONNE);

Servo servo1, servo2, servo3, servo4;  // Dichiarazione dei servo motori

String Pwd = "";  // Variabile per memorizzare il timer come stringa
int c = 0;  // Contatore per la posizione sul display
int timer = 0;  // Valore del timer in secondi

void setup() { 
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(leaccensionedg, OUTPUT);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Inserire timer:");
  lcd.setCursor(0, 1);
  delay(1000);
  lcd.clear();

  // Inizializzazione dei servo motori
  servo1.attach(6);  // Collegare il servo 1 al pin 6
  servo2.attach(7);  // Collegare il servo 2 al pin 7
  servo3.attach(8);  // Collegare il servo 3 al pin 8
  servo4.attach(9);  // Collegare il servo 4 al pin 9

  // Posizione iniziale dei servo
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
}

void loop() { 
  char tasto = mioTastierino.getKey();
  if (tasto) {  // Se un tasto è stato premuto
    lcd.setCursor(c, 0);
    Serial.println(tasto);
    lcd.write(tasto);
    Pwd += tasto;
    c++;

    if (tasto == 'A') {  // Tasto A per avviare il timer
      Pwd.remove(Pwd.length() - 1);  // Rimuove il carattere 'A'
      lcd.clear();

      if (Pwd.length() > 0 && isNumeric(Pwd)) {  // Verifica validità
        timer = Pwd.toInt();  // Converte Pwd in intero
        lcd.print("Timer: ");
        lcd.print(timer);
        lcd.print(" s");
        delay(1000);
        startTimer(timer);
      } else {
        lcd.print("Errore input!");
        delay(2000);
      }

      // Reset per nuovo input
      Pwd = "";
      c = 0;
      lcd.clear();
      lcd.print("Inserire timer:");
    }
  }
}

// Funzione per avviare il timer
void startTimer(int seconds) {
  lcd.clear();
  lcd.print("Timer avviato!");
  digitalWrite(ledg, HIGH);// Accende il LED verde

  while (seconds > 0) {
    lcd.setCursor(0, 1);
    lcd.print("T - ");
    lcd.print(seconds);
    lcd.print(" s  ");
    delay(1000);  // Pausa di 1 secondo
    seconds--;
  }
  lcd.clear();
  lcd.print("LIFT OFF");
  digitalWrite(ledg, LOW);  // Spegne il LED verde
  digitalWrite(ledr, HIGH);  // Accende il LED rosso
  digitalWrite(accensione, HIGH);  // Accende il LED rosso
  

  // Movimento dei servo motori
  servo1.write(180);
  servo2.write(180);
  servo3.write(180);
  servo4.write(180);
  delay(5000);  // Attesa per osservare il movimento
  digitalWrite(ledg, HIGH);


  // Ripristino posizione iniziale dei servo motori
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
  delay(1000);
  
}

// Funzione per verificare se una stringa è numerica
bool isNumeric(String str) {
  for (int i = 0; i < str.length(); i++) {
    if (!isDigit(str[i])) return false;
  }
  return true;
}
  
