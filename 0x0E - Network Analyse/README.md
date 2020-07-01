# Network Analyze

## Wireshark

Wireshark is a network protocol analyzer. It lets you see what’s happening over a network/USB connection.

Download: https://www.wireshark.org/#download

### Filters

When listening the traffic on a network interface, you can encounter a lot a traffic, in order to restraint your analyze to a specific client/server communication you can:
* focus your analyse on a specific protocol
* apply filters, to focus on specific packets

#### Focus on specific protocols

The Enabled Protocols dialog box lets you enable or disable specific protocols. Most protocols are enabled by default. 

To enable or disable protocols select `Analyze → Enabled Protocols`

Documentation: https://www.wireshark.org/docs/wsug_html_chunked/ChCustProtocolDissectionSection.html#ChAdvEnabledProtocols

#### Use filters

Here is a list of simple wireshark filters:

```
tcp.port=42 && tcp.len>0       List packet (containing data) over a TCP connection port 42
```

## Packet Injection

TCP/UDP proxies enables to inspect & forge packets on the fly. Making protocol analysis much easier.

## Proxy Implementation

A sample implemnentation is available in `proxy.py`, it allow packet inspection & injection with code hot reload.

```sh
# Start the proxy
$ ./proxy.py 
127.0.0.1:5000 <-> 127.0.0.1:8000

# Start a server on the port 8000, use -u for UDP
$ nc -l -p 8000

# Connect a client on port 5000, use -u for UDP
$ nc 127.0.0.1 5000
```

The proxy runs in a dedicated thread, allowing the user to type some commands while traffic is inspected:
* `>> [HEX DATA]` Inject hexadecimal encoded data to the server (TCP)
* `<< [HEX DATA]` Inject hexadecimal encoded data to the client (TCP)
* `quit` Stop the inspection, and close connection to both client & server.

Feel free to adapt the code according to your usage.