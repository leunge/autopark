#include <stdbool.h>
#include <string.h>
#include "coapa.h"

static char light = '0';
static char go = '0';

#define RSP_BUFFER_SIZE 64 
const uint16_t rsplen = RSP_BUFFER_SIZE;
static char rsp[RSP_BUFFER_SIZE] = "";
void build_rsp(void);

#include "Arduino.h"
static int led = 4;

static int enablePin = 9;
static int in1Pin = 6;
static int in2Pin = 8;
static int enable2Pin = 3;
static int in3Pin = 5;
static int in4Pin = 2;

int speed = 505;
int tspeed = 0;
boolean reverse = 0; 


void endpoint_setup(void)
{                
    pinMode(led, OUTPUT);  
    pinMode(enablePin, OUTPUT);  
    pinMode(enable2Pin, OUTPUT); 
    build_rsp();
}


static const coap_endpoint_path_t path_well_known_core = {2, {".well-known", "core"}};
static int handle_get_well_known_core(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    return coap_make_response(scratch, outpkt, (const uint8_t *)rsp, strlen(rsp), id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_APPLICATION_LINKFORMAT);
}

static const coap_endpoint_path_t path_light = {1, {"light"}};
static int handle_get_light(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    return coap_make_response(scratch, outpkt, (const uint8_t *)&light, 1, id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_put_light(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    if (inpkt->payload.len == 0)
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);
    if (inpkt->payload.p[0] == '1')
    {
        light = '1';
        digitalWrite(led, HIGH);
        return coap_make_response(scratch, outpkt, (const uint8_t *)&light, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
    else
    {
        light = '0';
        digitalWrite(led, LOW);
        return coap_make_response(scratch, outpkt, (const uint8_t *)&light, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
}

static const coap_endpoint_path_t path_go = {1, {"go"}};
static int handle_get_go(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    return coap_make_response(scratch, outpkt, (const uint8_t *)&go, 1, id_hi, id_lo, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static int handle_put_go(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    if (inpkt->payload.len == 0)
        return coap_make_response(scratch, outpkt, NULL, 0, id_hi, id_lo, COAP_RSPCODE_BAD_REQUEST, COAP_CONTENTTYPE_TEXT_PLAIN);
    if (inpkt->payload.p[0] == '1')
    {
        go = '1';
        operate(speed,!reverse,speed,reverse);
        return coap_make_response(scratch, outpkt, (const uint8_t *)&go, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
    else if (inpkt->payload.p[0] == '2')
    {
        go = '2';
        operate(speed,reverse,speed,!reverse);
        return coap_make_response(scratch, outpkt, (const uint8_t *)&go, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
    else if (inpkt->payload.p[0] == '3')
    {
        go = '3';
        operate(speed,reverse,speed,reverse);
        return coap_make_response(scratch, outpkt, (const uint8_t *)&go, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
    else if (inpkt->payload.p[0] == '4')
    {
        go = '4';
        operate(speed,!reverse,speed,!reverse);
        return coap_make_response(scratch, outpkt, (const uint8_t *)&go, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
    else if (inpkt->payload.p[0] == '5')
    {
        go = '5';
        operate(speed-100,reverse,speed,reverse);
        return coap_make_response(scratch, outpkt, (const uint8_t *)&go, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
    else if (inpkt->payload.p[0] == '6')
    {
        go = '6';
        operate(speed,reverse,speed-100,reverse);
        return coap_make_response(scratch, outpkt, (const uint8_t *)&go, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
    else if (inpkt->payload.p[0] == '+')
    {
        go = '9';
        speed = speed+10;
        return coap_make_response(scratch, outpkt, (const uint8_t *)&go, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
     else if (inpkt->payload.p[0] == '-')
    {
        go = '8';
        speed = speed- 10;
        return coap_make_response(scratch, outpkt, (const uint8_t *)&go, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
    else
    {
        go = '0';
        stop();
        return coap_make_response(scratch, outpkt, (const uint8_t *)&go, 1, id_hi, id_lo, COAP_RSPCODE_CHANGED, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
}


const coap_endpoint_t endpoints[] =
{
    {COAP_METHOD_GET, handle_get_well_known_core, &path_well_known_core, "ct=40"},
    {COAP_METHOD_GET, handle_get_light, &path_light, "ct=0"},
    {COAP_METHOD_PUT, handle_put_light, &path_light, NULL},
    {COAP_METHOD_GET, handle_get_go, &path_go, "ct=0"},
    {COAP_METHOD_PUT, handle_put_go, &path_go, NULL},
    {(coap_method_t)0, NULL, NULL, NULL}
};

void build_rsp(void)
{
    uint16_t len = rsplen;
    const coap_endpoint_t *ep = endpoints;
    int i;

    len--; // Null-terminated string

    while(NULL != ep->handler)
    {
        if (NULL == ep->core_attr) {
            ep++;
            continue;
        }

        if (0 < strlen(rsp)) {
            strncat(rsp, ",", len);
            len--;
        }

        strncat(rsp, "<", len);
        len--;

        for (i = 0; i < ep->path->count; i++) {
            strncat(rsp, "/", len);
            len--;

            strncat(rsp, ep->path->elems[i], len);
            len -= strlen(ep->path->elems[i]);
        }

        strncat(rsp, ">;", len);
        len -= 2;

        strncat(rsp, ep->core_attr, len);
        len -= strlen(ep->core_attr);

        ep++;
    }
}

void operate(int sppedL, boolean wheelL, int sppedR, boolean wheelR){
  analogWrite(enable2Pin, sppedL);
  digitalWrite(in3Pin, wheelL);
  digitalWrite(in4Pin, !wheelL);
  delay(100);
  analogWrite(enablePin, sppedR);
  digitalWrite(in1Pin, wheelR);
  digitalWrite(in2Pin, !wheelR);
  delay(100);
  
}

void set_right(int speeda, boolean wheel){
  analogWrite(enable2Pin, speeda);
  digitalWrite(in3Pin, ! wheel);
  digitalWrite(in4Pin, wheel);
  
}

void set_left(int speeda, boolean wheel){ 
  analogWrite(enablePin, speeda);
  digitalWrite(in1Pin, ! wheel);
  digitalWrite(in2Pin, wheel); 
}


void stop()
{
  analogWrite(in1Pin, 0);
  analogWrite(in2Pin, 0);
  analogWrite(in3Pin, 0);
  analogWrite(in4Pin, 0);
}


int speed_check(int speed){
 switch(speed){
   case '0':
     return 110;
   case '1':
     return 150;
   case '2':
     return 180;
   case '3':
     return 200;
   case '4':
     return 220;
   case '5':
     return 250;
   default:
     return 0;
 }
}


