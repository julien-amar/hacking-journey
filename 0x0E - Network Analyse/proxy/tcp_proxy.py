#!/usr/bin/env python3

from threading import Thread
from importlib import reload

from proxy import packet_parser as parser
from proxy import utils

import socket
import select
import os

BUFFER_SIZE = 4096

class TCPproxy(Thread):
    def __init__(self, client, server):
        super(TCPproxy, self).__init__()
        self.daemon = True
        self.client_host, self.client_port = self.client = utils.ip_to_tuple(client)
        self.server_host, self.server_port = self.server = utils.ip_to_tuple(server)
        self.client_socket = None
        self.server_socket = None

    def run(self):
        print('{}:{} <-> {}:{} (TCP)'.format(self.client_host, self.client_port, self.server_host, self.server_port))

        proxy = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        proxy.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        proxy.bind(self.client)
        proxy.listen(1)

        self.client_socket, _ = proxy.accept()

        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.connect(self.server)

        sockets = [ self.client_socket, self.server_socket ]

        while True:
            socket_read, _, _ = select.select(sockets, [], [])

            for s in socket_read:
                data = s.recv(BUFFER_SIZE)

                if not data:
                    print('[INFO] Connection closed, stopping proxy.')
                    self.client_socket.close()
                    self.server_socket.close()
                    return

                target = None
                try:
                    reload(parser)
                    if s is self.client_socket:
                        data = parser.receive_packet(self.client_socket, data, self.client, self.server)
                        target = self.server_socket
                    elif s is self.server_socket:
                        data = parser.send_packet(self.server_socket, data, self.server, self.client)
                        target = self.client_socket
                except Exception as e:
                    print('[ERROR] {}'.format(e))

                if target is not None and data is not None:
                    target.sendall(data)
    
    def send_packet(self, data):
        if self.server_socket:
            self.server_socket.sendall(data)
        else:
            print("[ERROR] Proxy is not running.")
    
    def receive_packet(self, data):
        if self.client_socket:
            self.client_socket.sendall(data)
        else:
            print("[ERROR] Proxy is not running.")