// C++ code
void setup(){
// put your setup code here, to run once:
pinMode(7, INPUT); //pin de entrada del INTERRUPTOR
pinMode(11, OUTPUT); //pin de salida de la LED roja
pinMode(12, OUTPUT); //pin de salida de la LED amarilla
pinMode(13, OUTPUT); //pin de salida de la LED verde
}
void loop(){
// put your main code here, to run repeatedly:
//Comprueba si el pulsador est´a pulsado
if ((digitalRead(7) == true)) {
digitalWrite(11, HIGH); //Encender LED roja
digitalWrite(12, LOW); //Apagar LED amarilla
digitalWrite(13, LOW); //Apagar LED verde
}
//Comprueba si el pulsador no est´a pulsado
else if ((digitalRead(7) == false)) {
digitalWrite(11, LOW); //Apagar LED roja
digitalWrite(12, HIGH); //Encender LED amarilla
digitalWrite(13, HIGH); //Encender LED verde
}
}