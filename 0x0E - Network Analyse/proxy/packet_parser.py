import struct

def handler_noop(data):
    return data

handlers_send = {
#    0x0000: handler_move,
}

handlers_receive = {
#    0x0000: handler_move,
}

def send_packet(proxy_socket, data, client, server):
    while len(data)>0:
        packet_id = struct.unpack(">H", data[0:2])[0]
        if packet_id not in handlers_send:
            print(">> {}".format(" ".join(["{:02x}".format(x) for x in data])))
        return handlers_send.get(packet_id, handler_noop)(data[2:])

def receive_packet(proxy_socket, data, client, server):
    while len(data)>0:
        packet_id = struct.unpack(">H", data[0:2])[0]
        if packet_id not in handlers_receive:
            print("<< {}".format(" ".join(["{:02x}".format(x) for x in data])))
        return handlers_receive.get(packet_id, handler_noop)(data[2:])