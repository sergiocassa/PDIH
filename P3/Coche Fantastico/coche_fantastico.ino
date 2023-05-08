// C++ code
void setup(){
// put your setup code here, to run once:
pinMode(10, OUTPUT); //pin de salida de una LED
pinMode(11, OUTPUT); //pin de salida de una LED
pinMode(12, OUTPUT); //pin de salida de una LED
pinMode(13, OUTPUT); //pin de salida de una LED
}
void loop(){
// put your main code here, to run repeatedly:
digitalWrite(11, LOW); //Apagar LED
digitalWrite(10, HIGH); //Encender LED
delay(75); // Esperar durante 0,075 segundos
digitalWrite(10, LOW); //Apagar LED
digitalWrite(11, HIGH); //Encender LED
delay(75); // Esperar durante 0,075 segundos
digitalWrite(11, LOW); //Apagar LED
digitalWrite(12, HIGH); //Encender LED
delay(75); // Esperar durante 0,075 segundos
digitalWrite(12, LOW); //Apagar LED
digitalWrite(13, HIGH); //Encender LED
delay(75); // Esperar durante 0,075 segundos
digitalWrite(13, LOW); //Apagar LED
digitalWrite(12, HIGH); //Encender LED
delay(75); // Esperar durante 0,075 segundos
digitalWrite(12, LOW); //Apagar LED
digitalWrite(11, HIGH); //Encender LED
delay(75); // Esperar durante 0,075 segundos
}
