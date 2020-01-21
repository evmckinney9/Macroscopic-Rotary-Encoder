#define ANALOGPIN A0
const int delayTime = 200;
const int sample_size = 5;
unsigned long startTime;
unsigned long endTime;
unsigned long xTime;

int count;
int flips;
boolean prevState;
float frequency;
const int rate = 2;
float data[delayTime*rate]= {0};
float data_time[delayTime*rate] ={0};
double slits = 10.0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ANALOGPIN, INPUT);
  Serial.begin(9600);
  startTime = millis();
  count = 0;
  flips = 0;
  prevState = false;
 
}

void loop() {
 
   //handle signal input
  float signalRead;
  float voltage;
 
  float sample_average = 0;
  float data_average = 0;
 
  for (int i =0; i< sample_size; i++){
    signalRead = analogRead(ANALOGPIN);
    voltage = signalRead * 5 / 1024.0;
    sample_average += voltage;
  }
  sample_average = sample_average/sample_size;
 
  //Serial.println(sample_average);
  //normalize input data
  boolean state = false;
  if (sample_average > 3.3){
    state = true;
  }
  //Serial.println(state);
  //calculate count
  boolean stateChange = false;
  if (state != prevState){
    stateChange = true;
    //Serial.println(stateChange);
  }
  prevState = state;
   
  if (stateChange){
    count+= 1;
    int index = count % (rate*delayTime);
    data[index] = 1;
    data_time[index] = millis();
  }

  //calculate frequency
  float xTime = millis() - delayTime; //xTime is minimum allowed
  flips = 0;
  for (int i =0; i< rate*delayTime; i++){
     if (data_time[i] < xTime){
        data_time[i] = 0;
        data[i] = 0;
     }
     flips += data[i];
  }
 
  frequency = flips/(slits * (delayTime/1000.0));

  Serial.println(frequency);
}
