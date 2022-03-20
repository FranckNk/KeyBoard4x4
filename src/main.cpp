/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

// Configuration du clavier matriciel 4x4.

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
	{'1', '2', '3', 'A'}, 
	{'4', '5', '6', 'B'}, 
	{'7', '8', '9', 'C'}, 
	{'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {13, 12, 11, 10}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// Type pour gérer les états du programme, afin d'éviter les bugs.
enum MODE{
	DEPART, 					// On lance le programme. Aucune situation... L'appui des touches ne servira à rien
	SELECTION_LED,				// On doit choisir la LED à utiliser.
	SELECTION_INTENSITE,		// On doit choisir son intensité.
	TERMINE,					// Toute les réglages sont faites, on a terminé. On peut maintenant régler la LED.
};

// Déclaration de la structure LED qui contient la lettre et la PIN pour l'affichage dans le moniteur série.
struct LED
{
	char Lettre;
	short int Broche;
};

LED MesLED[4];

//initialize an instance of class NewKeypad
byte ledPin = 12; 

boolean blink = false;
boolean ledPin_state;

// Déclaration des constantes.
const int LED_A = 0;
const int LED_B = 1;
const int LED_C = 2;
const int LED_D = 10;

// Déclaration des variables.
bool ChoixLED = false;
bool ChoixIntensite = false;

short int GetIntensite = 0;
MODE Situation;

void keypadEvent(KeypadEvent key);

void setup(){
		Serial.begin(9600);
		pinMode(ledPin, OUTPUT);              // Sets the digital pin as output.
		digitalWrite(ledPin, HIGH);           // Turn the LED on.
		ledPin_state = digitalRead(ledPin);   // Store initial LED state. HIGH when LED is on.
		keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
		Situation = DEPART; 					// Car on vient de démarrer le programme. On peut alors choisir une LED.
}

void loop(){
		char keys = keypad.getKey();

		if (keys) {
				Serial.println(keys);
		}
		if (Situation == DEPART){

		}
		if (Situation == TERMINE)
		{
			
		}
		
		if (blink){
				digitalWrite(ledPin,!digitalRead(ledPin));    // Change the ledPin from Hi2Lo or Lo2Hi.
				delay(100);
		}
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key){
		switch (keypad.getState()){
		case PRESSED:
				// Vérifions le bouton préssé afin de préciser la LED choisie et son intensité.

				break;

		case RELEASED:
				if (key == '*') {
						digitalWrite(ledPin,ledPin_state);    // Restore LED state from before it started blinking.
						blink = false;
				}
				break;

		case HOLD:	
				if (key == '0' && Situation == DEPART)
				{
					Situation = SELECTION_LED; // On passe alors au mode suivant.
				}
							
				if (key == '#' && Situation == DEPART) {
						ChoixLED = true;
						Situation = SELECTION_INTENSITE; // On peut maintenant choisir l'intensité de la LED.
				}
				if (key == '*' && Situation == SELECTION_INTENSITE) {
						ChoixIntensite = true;    	// Blink the LED when holding the * key.
						Situation = TERMINE; 		// On passe à l'état Terminé pour régler la LED.
						ChoixLED = false;

				}
				break;
		}
}

void ChooseLED(char LEDValue){
	switch (LEDValue)
	{
	case 'A':
		digitalWrite(LED_A, HIGH);
		digitalWrite(LED_B, LOW);
		digitalWrite(LED_C, LOW);
		digitalWrite(LED_D, LOW);
		break;
	
	case 'B':
		digitalWrite(LED_A, LOW);
		digitalWrite(LED_B, HIGH);
		digitalWrite(LED_C, LOW);
		digitalWrite(LED_D, LOW);
		break;
	
	case 'C':
		digitalWrite(LED_A, LOW);
		digitalWrite(LED_B, LOW);
		digitalWrite(LED_C, HIGH);
		digitalWrite(LED_D, LOW);
		break;
	
	case 'D':
		digitalWrite(LED_A, LOW);
		digitalWrite(LED_B, LOW);
		digitalWrite(LED_C, LOW);
		digitalWrite(LED_D, HIGH);
		break;
	
	default:
		break;
	}
}

void ConfigureeLED(){
	MesLED[0].Broche = LED_A;
	MesLED[0].Lettre = 'A';
	MesLED[1].Broche = LED_B;
	MesLED[1].Lettre = 'B';
	MesLED[2].Broche = LED_C;
	MesLED[2].Lettre = 'C';
	MesLED[3].Broche = LED_D;
	MesLED[3].Lettre = 'D';
}