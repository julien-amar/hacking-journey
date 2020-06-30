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