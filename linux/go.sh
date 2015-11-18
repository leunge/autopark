/home/leunge/down/libcoap-4.1.1/examples/./coap-client -e "$1" -m put coap://192.168.1.137/go&

if [ "$1" -eq 1 ] || [ "$1" -eq 2 ];then
    sleep 0.3
    pkill coap
    sleep 0.3
    /home/leunge/down/libcoap-4.1.1/examples/./coap-client -e "3" -m put coap://192.168.1.137/go&
fi

sleep 0.3
pkill coap
