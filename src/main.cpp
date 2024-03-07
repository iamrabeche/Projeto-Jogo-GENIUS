#include <Arduino.h>

#define MAX_RODADAS 30

/*
O tratamento de erros implementado foi principalmente a fun��o que identifica se um bot�o
est� travado. Segundo a l�gica de funcionamento, diante de um bot�o travado, o ledACERTO
e ledERRO s�o acionados.
Se dois bot�es s�o acionados ao mesmo tempo, o jogo vai registrar o primeiro acionado e
seguir a l�gica de funcionamento. Testei no f�sico e o tempo de resposta entre os
acionamentos est� ok para um usu�rio *normal*, ent�o n�o foi um problema identificado
para o qual tenha sido necess�rio implementar uma solu��o espec�fica.
N�o implementei uma fun��o que identifique o "clique-duplo", visto que o senhor mencionou que
seria o conte�do seguinte, ent�o n�o faria sentido colocar a carro�a a frente dos bois.

H�, claro, solu��es e tratamentos de erros mais "elegantes", como um teste dos leds
e bot�es em opera��o, mas pelo tempo dispon�vel e proposta da atividade n�o foi poss�vel
implementa-los.
*/

// COMPONENTES
int ledRed = 2;
int ledYellow = 3;
int ledGreen = 4;
int btnRed = 5;
int btnYellow = 6;
int btnGreen = 7;
int ledERRO = 8;
int ledACERTO = 9;

// VARI�VEIS E VETORES
int led = 0;
int sequencia[MAX_RODADAS];
int sequencia_digitada[MAX_RODADAS];
int position = 0;
int time_led_ON = 200;
int time_led_OFF = 300;
bool erro = false;

// FUN��ES
void inicio();
void ganhou();
void perdeu();
void ligar_led(int pos);
void sequencia_jogador(int pos);
bool verificar_sequencia(int vector[], int vector2[]);
void botao_travado();

void setup(){
  pinMode(btnRed, INPUT);
  pinMode(btnYellow, INPUT);
  pinMode(btnGreen, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledERRO, OUTPUT);
  pinMode(ledACERTO, OUTPUT);
  randomSeed(analogRead(0));
  inicio();
}

void loop()
{

if (position < MAX_RODADAS)
{
    led = random(0,3); // gera um numero aleatorio em 0 e 2
    sequencia[position] = led; // adiciona o numero sorteado a sequencia
    
    ligar_led(position);
    delay(50);
    sequencia_jogador(position);
    delay(50);
    erro = verificar_sequencia(sequencia,sequencia_digitada);
    
    if(erro == false)
    {
      digitalWrite(ledACERTO, HIGH);
      delay(500);
      digitalWrite(ledACERTO, LOW);
      position++;
    }
    else
    {
      perdeu();
      position = 0;
      inicio();
    }
}
else if(position == MAX_RODADAS)
{
  ganhou();
  position = 0;
  inicio();
}

}

void inicio()
{
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledGreen, HIGH);
    delay(300);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
    delay(500);
  }
}

void perdeu() {
  for (int f = 0; f < 3; f++)
  {
    digitalWrite(ledERRO, HIGH);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledGreen, HIGH);
    delay(300);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledERRO, LOW);
    delay(500);
  }
}

void ganhou() {
  for (int g = 0; g < 5; g++)
  {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledACERTO, HIGH);
    digitalWrite(ledERRO, HIGH);
    delay(300);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledERRO, LOW);
    digitalWrite(ledACERTO, LOW);
    delay(500);
  }
}

void sequencia_jogador(int pos)
{
  for(int r = 0; r <= pos; r++)
  {
    // aguarda um bot�o seja pressionado
    while (digitalRead(btnGreen) == LOW && digitalRead(btnYellow) == LOW && digitalRead(btnRed) == LOW)
    {
      delay(50); // pausa para n�o sobrecarregar o processador
    }

    unsigned long start = millis();

    // verifica qual bot�o foi pressionado
    if(digitalRead(btnGreen) == HIGH)
    {
      sequencia_digitada[r] = 0;
    }
    else if(digitalRead(btnYellow) == HIGH)
    {
      sequencia_digitada[r] = 1;
    }
    else if(digitalRead(btnRed) == HIGH)
    {
      sequencia_digitada[r] = 2;
    }
    // aguarda at� que o bot�o seja solto
    while (digitalRead(btnGreen) == HIGH || digitalRead(btnYellow) == HIGH || digitalRead(btnRed) == HIGH)
    {
      
      delay(50); // pausa
      
      // verificando se o botao est� sendo pressionado h� mais de 3s
      if(millis() - start >= 3000)
      {
        botao_travado();
      }
      
    }
    if(sequencia[r] != sequencia_digitada[r]){
    break;
    }
  }
  delay(50);
}

void botao_travado()
{
  digitalWrite(ledACERTO, HIGH);
  digitalWrite(ledERRO, HIGH);
  delay(500);
  digitalWrite(ledACERTO, LOW);
  digitalWrite(ledERRO, LOW);
}

bool verificar_sequencia(int vector[],int vector2[])
{
  bool aux = false; 
  
  for(int k = 0; k <= position; k++)
  {
    if(vector[k] == vector2[k])
    {
      aux = false;
    }
    else
    {
      aux = true;
      digitalWrite(ledERRO, HIGH);
      delay(500);
      digitalWrite(ledERRO, LOW);
      return aux;
    }
  }

  return aux;
}

void ligar_led(int pos)
{
//acender os leds de acordo com a sequ�ncia presente no vetor
  for (int j = 0; j <= pos; j++)
  {
    switch (sequencia[j])
    {
      case 0:
        digitalWrite(ledGreen, HIGH);
        delay(time_led_ON);
        digitalWrite(ledGreen, LOW);
        delay (time_led_OFF);
        break;
      case 1:
        digitalWrite(ledYellow, HIGH);
        delay(time_led_ON);
        digitalWrite(ledYellow, LOW);
        delay (time_led_OFF);
        break;
      case 2:
        digitalWrite(ledRed, HIGH);
        delay(time_led_ON);
        digitalWrite(ledRed, LOW);
        delay (time_led_OFF); 
        break;
      default:
        break;
    }
  }
}