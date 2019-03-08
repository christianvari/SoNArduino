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
    uint16_t distance;

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


    while(1){

        if(arduino_receive_packet(&command_packet)){
            command = command_packet.command;
            data = command_packet.payload;
            if(command == START || command==STOP || command == SET_VELOCITY || command == SET_PRECISION)
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
                            break;

                        case SET_PRECISION:
                            if(save(data, ADDR_PRECISION)){
                                error_packet.error_code = SetOutOfRange;
                                arduino_send_packet((Packet*) (&error_packet));
                            }
                            else
                                precision = data;
                            break;
                        case SET_VELOCITY:
                            if(save(data, ADDR_VELOCITY)){
                                error_packet.error_code = SetOutOfRange;
                                arduino_send_packet((Packet*) (&error_packet));
                            }
                            else
                                velocity = data;
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
                        case STOP:
                            setAngle(0);
                            status = STATUS_SLEEP;
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