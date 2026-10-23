# Packet field readability

One flag, a twelve-bit number and a sixteen-bit payload.

```mermaid
packet
0: "PacketFlag"
1-12: "PacketNumber"
13-28: "PacketPayload"
```
