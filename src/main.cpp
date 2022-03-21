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
#include "WIFIConnector_MKR1000.h"
#include "MQTTConnector.h"

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
	TERMINE, 					// On lance le programme. Aucune situation... L'appui des touches ne servira à rien
	SELECTION_LED,				// On doit choisir la LED à utiliser.
	SELECTION_INTENSITE,		// On doit choisir son intensité.
					// Toute les réglages sont faites, on a terminé. On peut maintenant régler la LED.
};

// Déclaration de la structure LED qui contient la lettre et la PIN pour l'affichage dans le moniteur série.
struct LED
{
	char Lettre = 'A'; // La LED A est sélectionnée par défaut.
	uint8_t Broche;
	uint8_t Intensite = 255; // Valeur par défaut de l'intensité de la LED.
};

LED MesLED[4];


// Déclaration des constantes.
const int LED_A = 2;
const int LED_B = 3;
const int LED_C = 4;
const int LED_D = 5;

// Déclaration des variables.
short int IndexLEDUtilise = 3;
MODE Situation;

void keypadEvent(KeypadEvent key);
void ChooseLED(char LEDValue);
void ConfigureeLED();

void setup(){
		Serial.begin(9600);
		// Initialisation du WiFi et du broquer MQTT.
		wifiConnect();
		MQTTConnect();

		pinMode(LED_A, OUTPUT);              // Sets the digital pin as output.
		pinMode(LED_B, OUTPUT);              // Sets the digital pin as output.
		pinMode(LED_C, OUTPUT);              // Sets the digital pin as output.
		pinMode(LED_D, OUTPUT);              // Sets the digital pin as output.
		ConfigureeLED();
		keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
		Situation = TERMINE; 					// Car on vient de démarrer le programme. On peut alors choisir une LED.
}

void loop(){
		char keys = keypad.getKey();

		if (keys) {
			Serial.println(keys);
			Serial.print("La valeur du mode est : ");
			Serial.println(Situation);
		}
		
		if (keys && Situation == SELECTION_LED && keys >= 'A' && keys <= 'D'){
			ChooseLED(keys); // On allume la LED correspondante.
		}
		if (keys && Situation == SELECTION_INTENSITE && keys <= '9' && keys >= '0')
		{
			short int ValeurIntensite = int (keys) - 48;
			Serial.print("La valeur de l'intensité est : ");
			
			MesLED[IndexLEDUtilise].Intensite = map(ValeurIntensite, 0, 9, 0, 255);
			Serial.println(MesLED[IndexLEDUtilise].Intensite);
			Serial.print("La LED choisie est : ");
			Serial.println(MesLED[IndexLEDUtilise].Lettre);
			// Appliquons l'intensité à temps réel de sélection sur la LED choisie.
			analogWrite(MesLED[IndexLEDUtilise].Broche, ValeurIntensite);
		}
		
		if (Situation == TERMINE){
			
			Serial.print("La valeur de l'index est : ");
			Serial.println(IndexLEDUtilise);
			analogWrite(MesLED[IndexLEDUtilise].Broche, MesLED[IndexLEDUtilise].Intensite);    // Change the Broche from Hi2Lo or Lo2Hi.
			delay(100);
			analogWrite(MesLED[IndexLEDUtilise].Broche, 0);    // Change the Broche from Hi2Lo or Lo2Hi.
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
				
			break;

		case HOLD:	
				if (key == '0' && Situation == TERMINE)
				{
					Situation = SELECTION_LED; // On passe alors au mode suivant.
					// Extension de toutes les LED pour choisir la LED.
					analogWrite(LED_A, 0);
					analogWrite(LED_B, 0);
					analogWrite(LED_C, 0);
					analogWrite(LED_D, 0);
				}
							
				if (key == '#' && Situation == SELECTION_LED) {
						Situation = SELECTION_INTENSITE; // On peut maintenant choisir l'intensité de la LED.
				}
				if (key == '*' && Situation == SELECTION_INTENSITE) {
						Situation = TERMINE; 		// On passe à l'état Terminé pour régler la LED.
					// On peut maintenant envoyer les informations sur T.B.
					appendPayload("LED", (1 + IndexLEDUtilise));
					appendPayload("Intensite", MesLED[IndexLEDUtilise].Intensite);
					sendPayload();
				}
				break;
		}
}

void ChooseLED(char LEDValue){
	switch (LEDValue)
	{
	case 'A':
		analogWrite(LED_A, 255);
		analogWrite(LED_B, 0);
		analogWrite(LED_C, 0);
		analogWrite(LED_D, 0);
		IndexLEDUtilise = 0;
		break;
	
	case 'B':
		analogWrite(LED_A, 0);
		analogWrite(LED_B, 255);
		analogWrite(LED_C, 0);
		analogWrite(LED_D, 0);
		IndexLEDUtilise = 1;
		break;
	
	case 'C':
		analogWrite(LED_A, 0);
		analogWrite(LED_B, 0);
		analogWrite(LED_C, 255);
		analogWrite(LED_D, 0);
		IndexLEDUtilise = 2;
		break;
	
	case 'D':
		analogWrite(LED_A, 0);
		analogWrite(LED_B, 0);
		analogWrite(LED_C, 0);
		analogWrite(LED_D, 255);
		IndexLEDUtilise = 3;
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