# C.R.I.C. (Cooperative Rover Integrated Controller)

## 📌 Descrizione del Progetto

**C.R.I.C.** è un rover a quattro ruote motrici a guida remota radiocontrollato, privo di asse di sterzo meccanico, sviluppato come Project Work per il Dipartimento di Ingegneria dell'Informazione, Elettrica e Matematica Applicata dell'Università degli Studi di Salerno (Anno Accademico 2025/2026).

Il veicolo è capace di muoversi nello spazio controllando in maniera indipendente i 4 motori ed evitando autonomamente gli ostacoli. Il fulcro dell'intero progetto è la **Dependability** (affidabilità) : il sistema è progettato per essere resiliente a condizioni anomale, adottando un'architettura hardware distribuita su due schede di controllo (Master e Slave) che garantiscono ridondanza logica e operativa, con avanzate politiche di fail-safe e modalità di funzionamento degradato.

## 👥 Team di Sviluppo (Gruppo 2)

* **Enrico Cavuoto** 


* **Biagio De Martino** 


* **Gianmarco Guerriero** 


* **Attilio Marco Sessa** 

---

## 🛠️ Architettura Hardware

Il sistema di controllo si basa su due microcontrollori **STM32G474RE** cooperanti , a cui sono interfacciate varie periferiche:

### Board 1 (Slave) - Locomozione e Monitoraggio ⚙️

È il nodo centrale per il controllo fisico e dinamico del rover.

* **Attuazione Motori**: Comanda 4 motori brushed indipendenti tramite due motor driver Sabertooth 2X12.


* **Controllo ad Anello Chiuso (PID)**: Legge gli encoder magnetici a due canali dei motori per eseguire la retroazione.


* **Diagnostica**: Integra il monitoraggio della tensione della batteria e della temperatura interna del microcontrollore per prevenire danni termici ed energetici.


* **Segnalazione Visiva**: Gestisce luci bianche ad alta potenza e luci rosse frontali, oltre a una striscia LED RGB posteriore.


### Board 2 (Master) - Percezione e Comando 📡

Progettata per analizzare l'ambiente circostante e ricevere i comandi dall'utente.

* **Rilevamento Ostacoli**: Equipaggiata con 3 sensori a ultrasuoni HC-SR04 (sinistro, destro e centrale) per la navigazione sicura.


* **Dinamica e Assetto**: Integra un'unità inerziale MPU6050 (accelerometro e giroscopio) interfacciata in .


* **Comando Remoto**: Si collega a una ricevente esterna via  per acquisire gli input dell'utente.


* **Ridondanza Attuativa**: In caso di malfunzionamento della Board 1, possiede un collegamento diretto ai motor driver per assumere il controllo di emergenza (in modalità degradata *open-loop*).



---

## 💻 Architettura Software e Controllo

* **Model-Based Design**: La logica di supervisione, i protocolli e gli automi sono stati modellati interamente in ambiente **Simulink/Stateflow**.


* **Generazione del Codice Automatica**: La conversione dei modelli in codice C/C++ per i microcontrollori è stata realizzata tramite **Embedded Coder**.


* **Sistema Operativo Real-Time (RTOS)**: L'architettura software utilizza **FreeRTOS**. Il task set è schedulato con una politica preemptive a priorità fissa *Rate Monotonic* (RM), assicurando il rispetto stringente delle deadline (come i 5ms necessari per il controllo PID dei motori).


* **Protocollo di Supervisione Robusto**: La comunicazione Master-Slave utilizza l'interfaccia UART con DMA e validazione incrociata tramite checksum CRC a 32 bit. Il protocollo implementa un handshake rigoroso su 3 macro-fasi e governa le transizioni tra modalità *Normale*, *Degradata* (perdita temporanea di connessione) e *Single-Board* (guasto definitivo).



---

## 🎮 Interfaccia e Controllo Remoto

Il rover è controllabile a distanza grazie a un sistema custom:

* **Controller Dedicato**: Basato sul framework Arduino, impiega un microcontrollore ESP32-2432S028R con display touchscreen da 2.8" e due joystick analogici, oltre a una dev-board YD-RP2040 come I/O expander.


* **Protocollo ESP-NOW**: La comunicazione tra il radiocomando e la ricevente del rover (YD ESP32-S3) utilizza ESP-NOW, garantendo bassissima latenza, lunga portata in modalità connectionless e tolleranza ai fault (failsafe watchdog).


* **Visione in Prima Persona (FPV) e Dashboard**: A bordo è presente un modulo AI-THINKER ESP32-CAM configurato come Access Point WiFi. Permette all'operatore di visualizzare uno streaming video e una GUI web completa di telemetrie (velocità motori, stato batteria, distanze ostacoli e diagnostica) aggiornata in tempo reale.


---

## 🛠️ Elettronica e Stampa 3D Custom

* **PCB Custom e Sistemi di Sicurezza**: Per evitare comandi incontrollati dei motor driver all'avvio, è stata progettata una PCB di "controllo sicuro". Questa scheda impiega multiplexer (74HC157) e relè fisici per abilitare la potenza ai motori solo in presenza di un segnale validato. Sono state progettate, inoltre, delle schede "HAT" per ottimizzare il cablaggio sulle NUCLEO STM32.


* **Componenti Meccanici**: Numerosi componenti (strutture di supporto per sensori HC-SR04, case per ESP-CAM e antenne, guscio ergonomico per il controller) sono stati disegnati e realizzati tramite stampa 3D.
