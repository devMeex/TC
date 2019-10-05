/*
*Guarda en la  eeprom la
*ultima wave_type que se
*envio por serial. Al iniciar 
*el bosquejo se restaura la 
*wave_type desde la eeprom
*/
unsigned long start;
int etime = 0;
bool increasing = true;
float sen_aux = 0;
String serial_input;
float output_signal = 0;
String wave_type = "SINE";
int baudios = 9600;
float periodo;
int amplitud = 20;
int address = 30;

void setup() {
  //pinMode(A0, INPUT);
  //pinMode(A1, OUTPUT);
  Serial.begin(baudios);
  Serial.setTimeout(10);
  start = millis();
  initializeType();
}

void initializeType(){
  int tmp = EEPROM.read(address);
  if(tmp=1){
    wave_type = "TRIANGLE";
  }
  if(tmp=2){
    wave_type = "SQUARE";
  }
  if(tmp=3){
    wave_type = "SINE";
  }
}

void saveType(String type){
  int tmp = -1;
  if(type="TRIANGLE"){
    tmp = 1;
  }
  if(type="SQUARE"){ 
    tmp = 2;
  }
  if(type="SINE"){ 
    tmp = 3;
  }
  EEPROM.update(address, tmp);
}

void loop() {
  periodo = analogRead(A0) * 10;

  serial_input = Serial.readStringUntil('\n');

  if (serial_input != "TRIANGLE" and serial_input != "SQUARE" and serial_input != "SINE") {

    etime = millis() - start;
    if (etime >= periodo / 2 or etime <= 0) {
      start = millis();
      increasing = !increasing;
    }

    if (wave_type == "TRIANGLE") {
      if (increasing) {
        output_signal += amplitud * (20.0 / periodo);
      } else {
        output_signal -= amplitud * (20.0 / periodo); //9-29 para 1200 - 15-97 para 2400 - 30-320 para 4800 - 0-960 para 9600baudios
      }
    }

    if (wave_type == "SQUARE") {
      if (increasing) {
        output_signal = amplitud / 2.0;
      } else {
        output_signal = -amplitud / 2.0;
      }
    }

    if (wave_type == "SINE") {
      //  if (increasing){
      sen_aux += 0.042411501 * (2000 / periodo) ; //para 1200baudios 0.22619467 (pi*720) - para 2400baudios 0.11309733 (pi*360) - para 4800baudios 0.056548667 (pi*180) - para 9600baudios 0.042411501 (pi*135) - para 19200baudios 0.042411501 (pi*135)
      //  }else{
      //    sen_aux -= 0.042411501;
      //  }
      output_signal = sin(sen_aux) * amplitud / 2.0;
    }

    //analogWrite(A1, ooutput_signal);
    Serial.println(output_signal);
    
  } else {
    wave_type = serial_input;
    saveType(wave_type);
  }
}

