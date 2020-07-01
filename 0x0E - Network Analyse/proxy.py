#!/usr/bin/env python3

from proxy import tcp_proxy as Proxy

import os

def main():
    proxy = Proxy.TCPproxy('127.0.0.1:5000', '127.0.0.1:8000')
    proxy.start()

    while True:
        try:
            cmd = input('$ ')
            if cmd[:4] == 'quit':
                os._exit(0)
            elif cmd[0:3] == '>> ':
                proxy.send_packet(bytes.fromhex(cmd[3:]))
            elif cmd[0:3] == '<< ':
                proxy.receive_packet(bytes.fromhex(cmd[3:]))
        except Exception as e:
            print(e)

if __name__ == '__main__':
    main()