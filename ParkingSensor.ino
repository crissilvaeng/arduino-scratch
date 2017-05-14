// ParkingSensor
// -------------
//
// Sensor de estacionamento com alerta luminoso e sonoro de distância.
// Circuits.io: https://circuits.io/circuits/4945678-lse-i-sensor-de-re 
//
// Criado em 14 de Maio de 2017.
// Copyleft: Cristina Silva <cristina.silva@openmailbox.org>
//
// Referências:
//      Ping Tutorial - https://www.arduino.cc/en/tutorial/ping
//      Buzzer Notes - https://www.circuitar.com.br/projetos/controlando-buzzer/

const int pingPin = 8;      // Pino do sensor de ultrasom.

const int greenLed = 9;     // Pino da LED verde.
const int yellowLed = 10;    // Pino da LED amarela.
const int redLed = 11;      // Pino da LED vermelha

const int buzzerPin = 12;   // Pino do buzzer.

const int doNote = 261; // Nota dó em Hz.
const int miNote = 329; // Nota mi em Hz.
const int solNote = 392; // Nota sol em Hz.

void setup() {
	Serial.begin(9600); // Inicializa comunicação serial.
    
    pinMode(greenLed, OUTPUT);
    pinMode(yellowLed, OUTPUT);
    pinMode(redLed, OUTPUT);

    pinMode(buzzerPin, OUTPUT);
}

void loop() {
	long duration, cm; // Duração da viagem do sinal e o correspondente em centimetros.
    int risk; // Nivel de proximidade, sendo 0 distante, 1 medianamente distante, 2 proximo.

    wakeup(); // Inicializa sensor de ultrasom.
    duration = scan(); // Realiza leitura e devolve duração da viagem do sinal.
    cm = microsecondsToCentimeters(duration);

    Serial.println(cm); // Exibe no Serial Monitor o resultado em cm da leitura.
    
    risk = calcRisk(cm); // Calcula o risco de colisão dada a proximidade.
    feedback(risk); // Fornece feedback sonoro e visual baseado no risco.
}

void wakeup() {
    // O sensor é trigado com um pulso HIGH de dois microsegundos ou mais, um
    // pulso anterior LOW é dado para garantir uma captura limpa.
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);

    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);
}

long scan() {
    // O mesmo pino é utilizado para fazer leituras, emitindo um pulso HIGH e
    // cronometrando o tempo até a volta do sinal.
    pinMode(pingPin, INPUT);
    return pulseIn(pingPin, HIGH);
}

int calcRisk(long proximity) {
    if (proximity <= 20) {
        return 2;
    }
    if (proximity <= 50) {
        return 1;
    }
    return 0;
}

void lowRisk() {
    digitalWrite(greenLed, HIGH);
    tone(buzzerPin, doNote);
    delay(100);
}

void mediumRisk() {
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, HIGH);
    tone(buzzerPin, miNote);
    delay(100);
}

void highRisk() {
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(redLed, HIGH);
    tone(buzzerPin, solNote);
    delay(100);
}

void feedback(int risk) {
    noTone(buzzerPin); 
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, LOW);

    switch (risk) {
        case 2:
            highRisk();
            break;
        case 1:
            mediumRisk();
            break;
        default: 
            lowRisk();
        break;
    }
}

long microsecondsToCentimeters(long microseconds) {
  	// O calculo é feito considerando a velocidade do som de 240 m/s, ou
    // 29 microsegundos. O sinal faz uma viagem de ida e volta, para achar a
    // distância é necessário dividir por 2.
	return microseconds / 29 / 2;
}