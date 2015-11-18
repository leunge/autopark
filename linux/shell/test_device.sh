/home/leunge/down/libcoap-4.1.1/examples/./coap-client -e "1" -m put coap://$1/light&
sleep 0.3
pkill coap

/home/leunge/down/libcoap-4.1.1/examples/./coap-client -e "0" -m put coap://$1/light&
sleep 0.3
pkill coap

/home/leunge/down/libcoap-4.1.1/examples/./coap-client -e "1" -m put coap://$1/light&
sleep 0.3
pkill coap

/home/leunge/down/libcoap-4.1.1/examples/./coap-client -e "0" -m put coap://$1/light&
sleep 0.3
pkill coap
