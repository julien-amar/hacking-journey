#!/usr/bin/env python3

from threading import Thread
from importlib import reload

from proxy import packet_parser as parser
from proxy import utils

import socket

BUFFER_SIZE = 4096

class UDPproxy(Thread):
    def __init__(self, client, server):
        super(UDPproxy, self).__init__()
        self.client_host, self.client_port = self.client = utils.ip_to_tuple(client)
        self.server_host, self.server_port = self.server = utils.ip_to_tuple(server)

    def run(self):
        print('{}:{} <-> {}:{} (UDP)'.format(self.client_host, self.client_port, self.server_host, self.server_port))
        
        self.proxy_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.proxy_socket.bind(self.client)
        
        self.client_address = None

        while True:
            data, address = self.proxy_socket.recvfrom(BUFFER_SIZE)
            
            try:
                reload(parser)
                if address == self.server:
                    data = parser.receive_packet(self.proxy_socket, data, self.server, self.client)
                    if self.client_address:
                        target = self.client_address
                else:
                    data = parser.send_packet(self.proxy_socket, data, self.client, self.server)
                    target = self.server
                    self.client_address = address
            except Exception as e:
                print('[ERROR] {}'.format(e))

            self.proxy_socket.sendto(data, target)
    
    def send_packet(self, data):
        if self.proxy_socket:
            self.proxy_socket.sendto(data, self.server)
        else:
            print("[ERROR] Proxy is not running.")
    
    def receive_packet(self, data):
        if self.proxy_socket and self.client_address:
            self.proxy_socket.sendto(data, self.client_address)
        else:
            print("[ERROR] Proxy is not running or client is not known.")