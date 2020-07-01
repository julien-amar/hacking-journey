#!/usr/bin/env python3

from proxy import tcp_proxy as TcpProxy
from proxy import udp_proxy as UdpProxy

import os

def main():
    proxy_tcp = TcpProxy.TCPproxy('127.0.0.1:5000', '127.0.0.1:8000')
    proxy_udp = UdpProxy.UDPproxy('127.0.0.1:5001', '127.0.0.1:8001')
    proxy_tcp.start()
    proxy_udp.start()

    while True:
        try:
            cmd = input('$ ')
            if cmd[:4] == 'quit':
                os._exit(0)
            elif cmd[0:3] == '>> ':
                proxy_tcp.send_packet(bytes.fromhex(cmd[3:]))
            elif cmd[0:3] == '<< ':
                proxy_tcp.receive_packet(bytes.fromhex(cmd[3:]))
        except Exception as e:
            print(e)

if __name__ == '__main__':
    main()