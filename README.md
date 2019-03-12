# Progetto Sistemi Operativi
## SONARDUINO

## Materiali
1x Arduino Mega rev v3
1x Sensore ultrasuoni hc-sr04
1x Servomotore sg90

## Introduzione

SONARDUINO è un sonar che opera a frequenze di (controllare) 40 Hz coprendo un angolo di 180 gradi. Per fare ciò utilizza un servomotre sg90 atto a descrivere l'angolo e un sensore a ultrasuoni hc-sr04. I dati raccolti vengono trasmessi dall'arduino al pc mediante una connessione seriale del tipo (controllare) s232. Dal pc è possibile impartire comandi al sonar tramite un'interfaccia grafica GTK che serve anche a graficare i dati ricevuti dal sonar. È possibile settare 3 paramentri: velocità, accuratezza e range di visualizzazione. La velocità può assumere un valore da 1 a 10 e indica l'angolo di avanzamento che deve descrivere il servomotore ad ogni step. L'accuratezza può invece avere valore compreso tra 1 e 5 e indica il numero di scansioni per step che il sensore deve effettuare. Settando il range invece si può scalare la visualizzazione grafica. 

## Arduino

### Servomotore

Il servomotore sg90 è un piccolo servo analogico che copre un angolo di 120 gradi (ma si può settare per usarlo a 180). Per comandarlo si usa una modulazione in PWM, nello specifico Fast PWM non invertita. A tal fine si utilizza un Timer (nel nostro caso il Timer/Counter1) con TOP = ICR. TOP viene calcolato con la seguente formula:
f_pwm = f_cpu/(prescale(1+top)) => top = (f_cpu / (f_pwm * prescale)) - 1

Per indicare l'angolo si setta l'output compare register del timer con il valore desiderato. Quest'ultimo viene calcolato in modo tale che sia compreso tra il duty cicle minimo per posizionare il servo nell'angolo 0 e quello per posizionare il servo nell'angolo 180. Tali informazioni sono reperibili sul datasheet del servomotore.

### HC-SR04

Il  sensore a ultrasuoni HC-SR04 può essere attivato mediante un segnale digitale sul pin TRIG della durata di 10 us. Una volta ricevuto il segnale emette gli ultrasuoni e porta il pin ECHO a +5V per una durata uguale al tempo di andat-ritorno del suono. LA distanza rilevata si calcola quindi come:
distanza = (tempo_pin_echo / 2) * velocita_suono
Per calcolare il tempo per il quale il pin ECHO è a +5V utilizzo un timer e sfrutto gli interrupt di overflow del timer. in questo modo posso calcolare il tempo come:
tempo_pin_echo = num_overflow * 65535 + TCNTn (se il timer è a 16 bit)

### UART

Usiamo la seriale per scambiare pacchetti tra l'arduino e il pc. Usiamo un approccio basato sulle interruzioni dato che i pacchetti vengono scambiati in modo bidirezionale e asincrono. I pacchetti hanno lunghezza pari ad un byte e l'informazione è ogrganizzata nel seguente modo:

0xaa 0x55 (header)
0x{TIPE}
0X{COMMAND}
0x{PAYLOAD}
0x{CHECKSUM}

Il checksum viene calcolato come somma dei byte precedenti a partire dal primo byte dell'header.

L'accettazione dei pacchetti avviene con un automa a stati finiti che controlla che il pacchetto ricevuto sia valido.

### EEPROM

Salviamo i valori dei setting di accuratezza e precisione nei primi due byte della EEPROM in modo tale da renderli permanenti nella memoria. 

##Client