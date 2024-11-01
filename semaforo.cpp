#define LED_VERMELHO 6
#define LED_AMARELO 5
#define LED_VERDE 4
#define LED_RGB_VERDE 9
#define LED_RGB_VERMELHO 10
#define BOTAO 12

// variaveis que definem o intervalo entre cada estado
// no fim vai ter um total de 6s devido ser 3,5s ligados o vermelho e o verde do pedestre e os outros 2,5s com o led vermelho ligado e o led do vermelho do pedestre piscando
const int TEMPO_VERMELHO = 3500; 
const int TEMPO_AMARELO = 2000;
const int TEMPO_VERDE = 4000;

// variavel que define o intervalo entre as piscadas do led vermelho do pedestre
const int INTERVALO_PISCA = 500;

// variavel que define quantas piscadas o led vermelho do pedestre vai ter
const int PISCADAS_RGB_VERMELHO = 3;

// enum para definir em qual estado do semaforo está
enum Estados {
    ESTADO_VERMELHO,
    ESTADO_AMARELO,
    ESTADO_VERDE
};

// variaveis que serão utilizadas durante o código
int estado_botao;
long tempo_decorrido;
long tempo_anterior = 0;
long intervalo = 0;
bool estado_led_rgb = LOW;
int pisca_count = 0;
bool estadoVerdeAtivo = false;
Estados estado_atual = ESTADO_VERMELHO;
bool piscar_iniciado = false;

// configuração inicial dos pinos
void setup() {
    pinMode(LED_VERMELHO, OUTPUT);
    pinMode(LED_AMARELO, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_RGB_VERDE, OUTPUT);
    pinMode(LED_RGB_VERMELHO, OUTPUT);
    pinMode(BOTAO, INPUT);
}

// loop onde acontece a lógica do código
void loop() {
  
  	// começa a contar o tempo decorrido desde o inicio do programa
    tempo_decorrido = millis();
  
  	// lê se o botão está pressionado ou não
    estado_botao = digitalRead(BOTAO);

  	// caso o botão estiver pressionado e o estado verde estiver ativo..
    if (estado_botao == HIGH && estadoVerdeAtivo) {
      
      	// modifica o estado atual para amarelo e define o intervalço atual como 0
        estado_atual = ESTADO_AMARELO;
        intervalo = 0;
    }

  	// verifica se já passou o tempo do intervalo 
    if (tempo_decorrido - tempo_anterior >= intervalo) {
      
      	// desliga todos os leds
        resetarLEDs();

      	// verifica em qual estado está para chamar as funções correspondentes
        switch (estado_atual) {
            case ESTADO_VERMELHO:
                estadoVermelho();
                break;

            case ESTADO_AMARELO:
                estadoAmarelo();
                break;

            case ESTADO_VERDE:
                estadoVerde();
                break;
        }
	
      	// modifica o tempo anterior para o tempo decorrido
      	// para poder verificar posteriormente se o tempo do intervalo passou
        tempo_anterior = tempo_decorrido;
    }
}

// função para desligar todos os leds
void resetarLEDs() {
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_RGB_VERDE, LOW);
    digitalWrite(LED_RGB_VERMELHO, LOW);
}

// função que controla o estado vermelho
void estadoVermelho() {
  
  	// liga o led vermelho
    digitalWrite(LED_VERMELHO, HIGH);

  	// se o piscar não tiver iniciado ainda... (ou seja se não for o momento de piscar o led vermelho para o pedestre)
    if (!piscar_iniciado) {
      
      	// liga o led verde do pedestre
        digitalWrite(LED_RGB_VERDE, HIGH);
      	
      	// modifica quanto tempo o led verde do pedestre vai ficar ligado
        intervalo = TEMPO_VERMELHO;
      	
      	// informa que no proximo turno o led vermelho do pedestre vai começar a piscar
        piscar_iniciado = true;
      
    } else { // se for para o led vermelho do pedestre piscar ...
      	
      	// chama a função de piscar o led verm,elho do pedestre
        piscarRGBVermelho();
    }
}

// função para controlar o estado amarelo
void estadoAmarelo() {
  	
  	// liga o led vermelho do pedestre, pois ele não pode passar
    digitalWrite(LED_RGB_VERMELHO, HIGH);
  
  	// liga o led amarelo
    digitalWrite(LED_AMARELO, HIGH);
  
  	// modifica o intervalo para o que o led amarelo vai ficar ligado
    intervalo = TEMPO_AMARELO;
  
  	// muda para o próximo estado, se o verde estava ativo muda para o vermelho, se não vai para o verde
    estado_atual = estadoVerdeAtivo ? ESTADO_VERMELHO : ESTADO_VERDE;
  
  	// inverte a variavel estadoVerdeAtivo
    estadoVerdeAtivo = !estadoVerdeAtivo;
}

// função para controlar o estado verde
void estadoVerde() {
  	
  	// liga o led vermelho do pedestre, pois ele não pode passar
    digitalWrite(LED_RGB_VERMELHO, HIGH);
  
  	// liga o led verde
    digitalWrite(LED_VERDE, HIGH);
  
  	// modifica o intervalo para o que o led verde vai ficar ligado
    intervalo = TEMPO_VERDE;
  
  	// muda para o estado verde
    estado_atual = ESTADO_AMARELO;
}

// função ´para controlar a piscada do led vermelho do pedestre
void piscarRGBVermelho() {
  	
  	// modifica o estado do led
    estado_led_rgb = !estado_led_rgb;
  
  	// liga ou desliga o led de acordo com seu estado modificado anteriormente
    digitalWrite(LED_RGB_VERMELHO, estado_led_rgb);

  	// modifica o intervalo para o que o led vai piscar
    intervalo = INTERVALO_PISCA;

  	// caso o estado do led esteja como desligado significa que já deu uma piscada, ligou e desligou
    if (!estado_led_rgb) {
      	
      	// adiciona um na quantidade de vezes o led piscou
        pisca_count++;
    }
  
  	// verifica se já piscou a quantidade de vezes necessárias
    if (pisca_count >= PISCADAS_RGB_VERMELHO) {
      	
      	// liga o led vermelho do pedestre
        digitalWrite(LED_RGB_VERMELHO, HIGH);
      	
      	// reinicia a varia que acumula quantas vezes o led piscou
        pisca_count = 0;
      
      	// muda para o estado amarelo
        estado_atual = ESTADO_AMARELO;
      
      	// define o piscar iniciado como falso
        piscar_iniciado = false;
      
      	// define o estado do led como low
      	estado_led_rgb = LOW;
    }
}
