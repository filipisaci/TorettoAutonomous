/* 
 Toretto Autonomus
 Codigo Original:
 Desenvolvido por: Jaspion Lopes
 Curso: Ciencia da Computacao
 Instituiзгo: Faculdade Pitagoras Votorantim-Sorocaba
 
 Codigo adaptado por:
 Desenvolvido por: Filipi Souza
 Curso: Engenharia da Computaзo
 Instituiзao: Faculdade SATC
 */

//Com o jumper a shield usa a bateria externa para motor
#include <AFMotor.h>
#include <Servo.h>

const int vEsquerdo = 70;
const int vDireito = 70;
const int vEsquerdoVolta = 200;
const int vDireitoVolta = 200;
const int vEsquerdoApx = 90;
const int vDireitoApx = 70;
const int vEsquerdoAft = 70;
const int vDireitoAft = 110;

// declaraзгo de variбveis utilizadas para controle do projeto;
long duracao_frente, duracao_tras, duracao_fesq, duracao_fdir, fdireita, fesquerda, frente, tras, duracao_tesq, duracao_tdir, tdireita, tesquerda; 
int bola = 0;
int voltar = 0;

//cria variveis globais
AF_DCMotor motor_direito2(3);
//direito da frente
AF_DCMotor motor_direito(2);

AF_DCMotor motor_esquerdo2(1);
//esquerdo tras
AF_DCMotor motor_esquerdo(4);


Servo servo_baixo;
Servo servo_cima;

//Define pinos dos sensores ultrassonicos
int trigger_frente = A12;
int echo_frente = A13;
int trigger_frente_esquerda = A14;
int echo_frente_esquerda = A15;
int trigger_frente_direita = A10;
int echo_frente_direita = A11;

int echo_fotoTransistor = A9;

int trigger_tras = A1;
int echo_tras = A0;
int trigger_tras_esquerda = A3;
int echo_tras_esquerda = A2;
int trigger_tras_direita = A5;
int echo_tras_direita = A4;


void setup(){
  Serial.begin(9600);
  Serial.print("Iniciando o codigo\n");
  //Porque isso faz a garra subir e abri-la??
  servo_cima.attach(10); // abre e fecha
  servo_baixo.attach(9); // sobe e desce

  abreGarra();
  levantaGarraMaisAlto();

  //passa a constante do pino e diz se  entrada ou saida
  pinMode(trigger_frente, OUTPUT);
  pinMode(echo_frente, INPUT);
  pinMode(trigger_frente_direita, OUTPUT);
  pinMode(echo_frente_direita, INPUT);
  pinMode(trigger_frente_esquerda, OUTPUT);
  pinMode(echo_frente_esquerda, INPUT);

  pinMode(trigger_tras, OUTPUT);
  pinMode(echo_tras, INPUT);
  pinMode(trigger_tras_direita, OUTPUT);
  pinMode(echo_tras_direita, INPUT);
  pinMode(trigger_tras_esquerda, OUTPUT);
  pinMode(echo_tras_esquerda, INPUT);

  pinMode(echo_fotoTransistor, INPUT); //Pino ligado ao coletor do fototransistor  

  delay(3000);  
}






void loop(){

  digitalWrite(trigger_frente, LOW);  
  delayMicroseconds(2);               
  digitalWrite(trigger_frente, HIGH);
  delayMicroseconds(5);                
  digitalWrite(trigger_frente, LOW);  
  duracao_frente = pulseIn(echo_frente, HIGH);
  frente = duracao_frente/38/2;
  //Serial.print("Frente: ");
  //Serial.print(frente);
  //Serial.print("\n");
  delay(100);

  digitalWrite(trigger_frente_direita, LOW);  
  delayMicroseconds(2);               
  digitalWrite(trigger_frente_direita, HIGH);
  delayMicroseconds(5);                
  digitalWrite(trigger_frente_direita, LOW);  
  duracao_fdir = pulseIn(echo_frente_direita, HIGH);
  fdireita = duracao_fdir/38/2;
  Serial.print("Frente Direita: ");
  Serial.print(fdireita);
  Serial.print("\n");
  delay(100);

  digitalWrite(trigger_frente_esquerda, LOW);  
  delayMicroseconds(2);               
  digitalWrite(trigger_frente_esquerda, HIGH);
  delayMicroseconds(5);                
  digitalWrite(trigger_frente_esquerda, LOW);  
  duracao_fesq = pulseIn(echo_frente_esquerda, HIGH);
  fesquerda = duracao_fesq/38/2;
  Serial.print("Frente Esquerda: ");
  Serial.print(fesquerda);
  Serial.print("\n");
  delay(100);

  //---------------------------------------
  //*** Inicio do looping                 |
  //---------------------------------------


  bola = testaBola();
  if (bola > 0 && bola < 700) {



    while(voltar) {

      Serial.print("Pegou a bola\n");
      baixaGarra();
      fechaGarra();
      levantaGarra();

      goto saindoLabirinto;

saindoLabirinto:
      Serial.print("Saindo do labirinto\n");
      while(1){


      } 
    } 
  } 
  else { //fecha o if bola

    //controle para manter o carro dentro de uma distancia ideal da parede
    if(frente > 7) 
    {  
      if(fdireita > (8.5) && fdireita< (8.55)) 
      {               
        paraFrenteVmin();                                             
      }

      if(fdireita > (8.5))  
      {
        aproximaDireita();                    
      }
      if(fdireita < (8.5))   
      {
        afastaDireita();         
      }
    } 

    //essa condição fará o carro entrar na primeira entrada as esquerda
    if(fdireita < 30 && fesquerda > 20 && (frente < 69 && frente > 64)) {
      parar();
      vireEsquerda(); 
      //Serial.print("Primeira Entrada Esquerda\n");
    } 

    if (fdireita < 20 && fesquerda > 20 && (frente > 7 && frente < 25 )) {
      parar();
      vireEsquerda();
      //Serial.print("Segunda Entrada Esquerda\n");
    }

    //essa condição fará o carro entrar as direita quando não tiver espaco a frente e a esquerda  
    if (fesquerda < 30 && fdireita > 20  && (frente > 7 && frente < 20)) {
      parar();
      vireDireita();
    } 
    //vire a direita para pegar a bola
    if (fesquerda < 30 && fdireita > 20  && (frente > 54 && frente < 69)) {
      parar();
      vireDireita();
    } 
    //essa condição fará o carro entrar as esquerda quando nao tiver espaço a frente, mas ele tem duas opção para os lados  
    if (frente < 7 && fesquerda > 20 && fdireita > 20) {
      parar();
      vireEsquerda();
      //Serial.print("Entrando as esquerda mas posso ir para os dois lados\n");
    } 
    //aqui ele não tem condição valida para nenhum lado, entao ele tenta virar um 180. 
    if (fdireita < 20 && fesquerda < 20 && frente < 7) {
      parar();
    } 
  }
}//fim looping



//---------------------------------------
//*** Funcoes de deslocamento           |
//---------------------------------------

void parar(){
  //Serial.print("Opa parei");
  //Serial.print("\n");
  motor_direito.setSpeed(0);
  motor_direito.run(RELEASE);
  motor_esquerdo.setSpeed(0);
  motor_esquerdo.run(RELEASE);
  motor_direito2.setSpeed(0);
  motor_direito2.run(RELEASE);
  motor_esquerdo2.setSpeed(0);
  motor_esquerdo2.run(RELEASE);
  delay(1000);
} 

void reh(){
  //Serial.print("Ops Reeeeeeee");
  //Serial.print("\n");
  motor_direito.setSpeed(vDireito);
  motor_direito.run(BACKWARD);
  motor_esquerdo.setSpeed(vEsquerdo);
  motor_esquerdo.run(BACKWARD);
  motor_direito2.setSpeed(vDireito);
  motor_direito2.run(BACKWARD);
  motor_esquerdo2.setSpeed(vEsquerdo);
  motor_esquerdo2.run(BACKWARD);
} 

void paraFrenteVmin(){
//  Serial.print("Para Frente Vmin");
//  Serial.print("\n");
  motor_direito.setSpeed(vDireito);
  motor_direito.run(FORWARD);
  motor_esquerdo.setSpeed(vEsquerdo);
  motor_esquerdo.run(FORWARD);
  motor_direito2.setSpeed(vDireito);
  motor_direito2.run(FORWARD);
  motor_esquerdo2.setSpeed(vEsquerdo);
  motor_esquerdo2.run(FORWARD);
}

void vireEsquerda() {
//  Serial.print("Entrando as Esquerda");
//  Serial.print("\n");
  motor_direito.setSpeed(vDireitoVolta);
  motor_direito.run(FORWARD);
  motor_direito2.setSpeed((vDireitoVolta+50));
  motor_direito2.run(FORWARD);
  motor_esquerdo.setSpeed(vEsquerdoVolta/1.5);
  motor_esquerdo.run(BACKWARD);
  motor_esquerdo2.setSpeed((vEsquerdoVolta+50)/1.5);
  motor_esquerdo2.run(BACKWARD);
  delay(500);
}

void vireDireita() {
//  Serial.print("Entrando as Direita");
//  Serial.print("\n");
  motor_direito.setSpeed((vDireitoVolta+50)/1.5);
  motor_direito.run(BACKWARD);
  motor_direito2.setSpeed(vDireitoVolta/1.5);
  motor_direito2.run(BACKWARD);
  motor_esquerdo.setSpeed((vEsquerdoVolta+50));
  motor_esquerdo.run(FORWARD);
  motor_esquerdo2.setSpeed(vEsquerdoVolta);
  motor_esquerdo2.run(FORWARD);
  delay(500);
}

void aproximaDireita(){
  //Serial.print("Aproximei");
  //Serial.print("\n");
  motor_direito.setSpeed(vDireitoApx);
  motor_direito.run(FORWARD);
  motor_direito2.setSpeed(vDireitoApx);
  motor_direito2.run(FORWARD);
  motor_esquerdo.setSpeed(vEsquerdoApx);
  motor_esquerdo.run(FORWARD);
  motor_esquerdo2.setSpeed(vEsquerdoApx);
  motor_esquerdo2.run(FORWARD);
  delay(18);
}

void afastaDireita(){
  //Serial.print("    AFASTEI     ");
  //Serial.print("\n");
  motor_direito.setSpeed(vDireitoAft);
  motor_direito.run(FORWARD);
  motor_direito2.setSpeed(vDireitoAft);
  motor_direito2.run(FORWARD);
  motor_esquerdo.setSpeed(vEsquerdoAft);
  motor_esquerdo.run(FORWARD);
  motor_esquerdo2.setSpeed(vEsquerdoAft);
  motor_esquerdo2.run(FORWARD);
  delay(18);
}

void rehVmin(){
//  Serial.print("Andando de Reh Vmin");
//  Serial.print("\n");
  motor_direito.setSpeed(vDireito);
  motor_direito.run(BACKWARD);
  motor_esquerdo.setSpeed(vEsquerdo);
  motor_esquerdo.run(BACKWARD);
  motor_direito2.setSpeed(vDireito);
  motor_direito2.run(BACKWARD);
  motor_esquerdo2.setSpeed(vEsquerdo);
  motor_esquerdo2.run(BACKWARD);
}


void vireEsquerdaReh() {
//  Serial.print("Entrando as Esquerda de Reh");
//  Serial.print("\n");
  motor_esquerdo.setSpeed(vDireitoVolta/1.5);
  motor_esquerdo.run(FORWARD);
  motor_esquerdo2.setSpeed((vDireitoVolta+50)/1.5);
  motor_esquerdo2.run(FORWARD);
  motor_direito.setSpeed(vEsquerdoVolta);
  motor_direito.run(BACKWARD);
  motor_direito2.setSpeed(vEsquerdoVolta+50);
  motor_direito2.run(BACKWARD);
  delay(500);
}

void vireDireitaReh() {
//  Serial.print("Entrando as Direita de Reh");
//  Serial.print("\n");
  motor_esquerdo.setSpeed(vDireitoVolta+50);
  motor_esquerdo.run(BACKWARD);
  motor_esquerdo2.setSpeed(vDireitoVolta);
  motor_esquerdo2.run(BACKWARD);
  motor_direito.setSpeed((vEsquerdoVolta+50)/1.5);
  motor_direito.run(FORWARD);
  motor_direito2.setSpeed(vEsquerdoVolta/1.5);
  motor_direito2.run(FORWARD);
  delay(500);
}

void aproximaEsquerdaReh(){
  //Serial.print("        APROXIMEI      de Reh à esquerda");
  //Serial.print("\n");
  motor_direito.setSpeed(vDireitoApx);
  motor_direito.run(BACKWARD);
  motor_direito2.setSpeed(vDireitoApx);
  motor_direito2.run(BACKWARD);
  motor_esquerdo.setSpeed(vEsquerdoApx);
  motor_esquerdo.run(BACKWARD);
  motor_esquerdo2.setSpeed(vEsquerdoApx);
  motor_esquerdo2.run(BACKWARD);
  delay(18);
}

void afastaEsquerdaReh(){
  //Serial.print("       AFASTEI        de Reh à esquerda");
  //Serial.print("\n");
  motor_direito.setSpeed(vDireitoAft);
  motor_direito.run(BACKWARD);
  motor_direito2.setSpeed(vDireitoAft);
  motor_direito2.run(BACKWARD);
  motor_esquerdo.setSpeed(vEsquerdoAft);
  motor_esquerdo.run(BACKWARD);
  motor_esquerdo2.setSpeed(vEsquerdoAft);
  motor_esquerdo2.run(BACKWARD);
  delay(18);
}

int testaBola(){
  bola = analogRead(echo_fotoTransistor);  
  if (bola > 0 && bola < 700)  
  {  
    Serial.println("Objeto : Detectado");  
    Serial.println(bola);  
    Serial.println("\n");  
    delay(10);
    voltar = 1;
  }  
  else  
  {  
    Serial.println("Objeto : Ausente !");  
    Serial.println(bola);  
    Serial.println("\n");  
    delay(10);
    voltar = -1;
  }
  return voltar;
}

void levantaGarraMaisAlto(){
  servo_baixo.write(50); 
  delay(1000);
}

void levantaGarra(){
  servo_baixo.write(-30); 
  delay(1000);
}

void baixaGarra(){
  servo_baixo.write(40); 
  delay(1000);
}

void abreGarra(){
  servo_baixo.write(-100); 
  delay(2000);
}

void fechaGarra(){
  servo_baixo.write(40); 
  delay(1000);
}
