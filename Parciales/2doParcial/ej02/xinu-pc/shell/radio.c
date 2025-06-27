#include <xinu.h>


const char *mensajes[] = {
    "Mensaje de la primer radio.\n",
    "Segunda radio transmitiendo.\n",
    "Tercera emisora enviando.\n",
};

int PID_RADIOPLAY;
int PID_RADIOCONTROL;
int PID_EMISORA1;
int PID_EMISORA2;
int PID_EMISORA3;
int NUM_CONTROL = -1;

#define MSG_NONE -1
#define KEY_DOWN_NOW(key)  (tecla_actual == key)

int radio() {
    PID_RADIOPLAY = create(radio_play, 2048, 10, "Radio Play", 0);
    PID_RADIOCONTROL = create(radio_play, 2048, 10, "Radio Control", 0);

    PID_EMISORA1 = create(emisora, 2048, 8, "Emisora 1", 2, 1, 600);
    PID_EMISORA2 = create(emisora, 2048, 8, "Emisora 2", 2, 2, 500);
    PID_EMISORA3 = create(emisora, 2048, 8, "Emisora 3", 2, 3, 888);
	
    resume(PID_RADIOPLAY);
    resume(PID_RADIOCONTROL);
	resume(PID_PID_EMISORA1);
    resume(PID_PID_EMISORA2);
    resume(PID_PID_EMISORA3);
	
	if (receive() == MSG_END) {
        kill(PID_RADIOPLAY);
        kill(PID_RADIOCONTROL);
        kill(PID_PID_EMISORA1);
        kill(PID_PID_EMISORA2);
        kill(PID_PID_EMISORA3);
		printf("RADIO finaliad.\n");
	}

    return 0;
}



void radio_play() {
    int msg = MSG_NONE, n;
    char c;
    while(1) {
        if (msg != MSG_NONE && NUM_CONTROL != -1) {
            n = (msg >> 8); // obtenemos el numero de emisora
            if (NUM_CONTROL == n) {
                c = (char) (msg & 0xFF); // obtenemos la letra del mensaje 
                // en la variable n quedó el nro de la emisora 
                // en la variable c quedó la letra recibida del mensaje

                // comentario pivote, esta zona debe ser sincronizada junto con radio_contol
                // MUTEX

                printf(c);

                //FIN MUTEX
            }
            
        }
        // msg es el mensaje recibido. Aquí lo definimos sólo para el ejemplo
        msg = recieve();
    }
    
}

void radio_contol() {
    //Control de entrada del usuario

}

void emisora(int num_emisora, int ms_retardo) {
    //Obtine mensaje correspondiente   
    char c, *mensaje = mensajes[num_emisora-1];
    int msg, n; // Estructura para enviar a radio_play
    
    for(int i = 0; i < mensaje.lenght ;i++) {
        n = num_emisora; // ejemplo de nro de emisora
        c = mensajes[num_emisora][i]; // ejemplo de letra ASCII a enviar 
        msg = (n << 8) | c; //  colocamos en msg el nro de la emisora y el ASCII            
        // en la variable msg quedó el mensaje ya armado listo para enviar

        //Envia caracter a radio_play, luego de un retraso
        sleepms(ms_retardo);
        send(PID_RADIOPLAY, msg);
    }
}
