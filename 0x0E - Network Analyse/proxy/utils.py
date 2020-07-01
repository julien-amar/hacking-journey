def ip_to_tuple(ip):
    ip, port = ip.split(':')
    return (ip, int(port))