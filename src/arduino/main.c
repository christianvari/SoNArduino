#include "arduino_packet/uart.h"
#include "servo/servo.h"
#include "sonar/sonar.h"
#include "eeprom/my_eeprom.h"

#define STATUS_SLEEP 0
#define STATUS_WORKING 1

int main(void){

    uint8_t status = STATUS_SLEEP;
    uint8_t command = 255;
    uint8_t data = 255;
    uint8_t have_to_set_command = 0;


    UART_init();

    uint8_t velocity = load(ADDR_VELOCITY);
    uint8_t precision = load(ADDR_PRECISION);
    uint8_t direction = AUMENTA_ANGOLO;
    uint8_t angle;
    uint8_t distance;

    /**
    
        TODO:
        - Inviare questi dati sulla seriale con opportuno pacchetto
    
     */

    Packet cpacket;
    cpacket.type = COMMAND;
    CommandPacket command_packet;
    command_packet.packet = cpacket;

    Packet spacket;
    spacket.type=STATUS;
    StatusPacket status_packet;
    status_packet.packet=spacket;

    Packet epacket;
    epacket.type = ERROR;
    ErrorPacket error_packet;
    error_packet.packet = epacket;

    Packet evpacket;
    evpacket.type = EVENT;
    EventPacket event_packet;
    event_packet.packet = evpacket;

    Packet cgpacket;
    cgpacket.type = CONFIGURATION;
    ConfigurationPacket config_packet;
    config_packet.packet = cgpacket;

    while(1){

        if(arduino_receive_packet(&command_packet)){
            command = command_packet.command;
            data = command_packet.payload;
            if(command == START || command==STOP || command == SET_VELOCITY || command == SET_PRECISION || command == SEND_CONFIG || command == DISCONNECTION)
                have_to_set_command = 1;
        }

        switch (status)
        {
            case STATUS_SLEEP:
                if(have_to_set_command){
                    switch (command)
                    {
                        case START:
                            initServo();
                            initSonar();
                            status = STATUS_WORKING;
                            event_packet.event = STARTED;
                            arduino_send_packet((Packet*)(&event_packet));
                            break;
                        case STOP:
                            event_packet.event = STOPPED;
                            arduino_send_packet((Packet*)(&event_packet));
                            break;
                        case SET_PRECISION:
                            if(save(data, ADDR_PRECISION)){
                                error_packet.error_code = SetOutOfRange;
                                arduino_send_packet((Packet*) (&error_packet));
                            }
                            else
                                precision = data;
                                event_packet.event = SETTED_PRECISION;
                                arduino_send_packet((Packet*)(&event_packet));
                            break;
                        case SET_VELOCITY:
                            if(save(data, ADDR_VELOCITY)){
                                error_packet.error_code = SetOutOfRange;
                                arduino_send_packet((Packet*) (&error_packet));
                            }
                            else
                                velocity = data;
                                event_packet.event = SETTED_VELOCITY;
                                arduino_send_packet((Packet*)(&event_packet));
                            break;
                        case SEND_CONFIG:
                            config_packet.velocity = velocity;
                            config_packet.precision = precision;
                            arduino_send_packet((Packet*)(&config_packet));
                            break;
                        case DISCONNECTION:
                            event_packet.event = DISCONNECTED;
                            arduino_send_packet((Packet*)(&event_packet));
                            break;
                        default:
                            error_packet.error_code = CantHandleCommand;
                            arduino_send_packet((Packet*) (&error_packet));
                            break;
                    }
                    have_to_set_command = 0;
                }
                break;

            case STATUS_WORKING:
                if(have_to_set_command){
                    switch (command)
                    {   
                        
                        case START:
                            event_packet.event = STARTED;
                            arduino_send_packet((Packet*)(&event_packet));
                            break;
                            
                        case STOP:
                            setAngle(0);
                            status = STATUS_SLEEP;
                            event_packet.event = STOPPED;
                            arduino_send_packet((Packet*)(&event_packet));
                            break;
                        default:
                            error_packet.error_code = CantHandleCommand;
                            arduino_send_packet((Packet*) (&error_packet));
                            break;
                    }

                    have_to_set_command = 0;
                }

                angle = getAngle();
                if(angle == 180) direction = DIMINUISCI_ANGOLO;
                else if(angle == 0) direction = AUMENTA_ANGOLO; 

                modifyAngle(direction, velocity);

                distance = getDistance(precision);
                status_packet.angle= angle;
                status_packet.distance = distance;
                arduino_send_packet((Packet*) (&status_packet));

                //_delay_ms(50);

                break;
        }
    }
    
    


}