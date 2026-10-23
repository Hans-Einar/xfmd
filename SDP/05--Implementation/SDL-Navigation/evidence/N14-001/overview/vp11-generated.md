# VP11 — Properties, traceability and model gaps

Revision: `b3011e73ee1eece4b0df6861ea14435f312f5e54fcccec544f2b0ceebfe7012e`.

Declarations and all facts with source positions; support boundaries are retained.

## Snapshot coverage

Counts describe this checked source snapshot, not implementation coverage or verification evidence. This inspection index contains no combined architecture diagram.

| Inventory | Count |
| --- | --- |
| Declarations | 47 |
| Facts | 87 |
| Explicit properties | 29 |
| Reported model gaps | 7 |

### Declaration kinds

| Kind / verb | Count |
| --- | --- |
| activity | 4 |
| actor | 2 |
| capability | 2 |
| channel | 2 |
| container | 2 |
| contract | 5 |
| database | 2 |
| datagram | 1 |
| dataset | 2 |
| encoding | 2 |
| feature | 1 |
| field | 6 |
| functionality | 1 |
| interface | 2 |
| message | 2 |
| mode | 3 |
| scenario | 1 |
| unit | 2 |
| usecase | 3 |
| variant | 2 |

### Fact verbs

| Kind / verb | Count |
| --- | --- |
| addresses | 1 |
| allocated-to | 2 |
| consumes | 2 |
| contains | 1 |
| contributes-to | 1 |
| defines | 2 |
| delivers | 1 |
| depends-on | 1 |
| encodes | 2 |
| from | 1 |
| has | 29 |
| has-field | 6 |
| holds | 2 |
| owns | 3 |
| permits | 2 |
| places | 6 |
| projects | 1 |
| provides | 1 |
| pursues | 2 |
| realizes | 1 |
| refines | 2 |
| requires | 2 |
| runs-in | 1 |
| step | 1 |
| supports | 1 |
| upholds | 7 |
| uses | 6 |

## Explicit properties

Property values, completeness and implementation-status are source assertions, not proof of implemented behavior or owner acceptance. Missing properties are unspecified, with no inferred defaults.

- [ABuild](sdl-source://s0/34/50): implementation-status = implemented. Source fact: [f0002](sdl-source://s0/974/1021).
- [APlan](sdl-source://s0/71/86): implementation-status = planned. Source fact: [f0004](sdl-source://s0/1044/1086).
- [AVerify](sdl-source://s0/87/104): implementation-status = verified. Source fact: [f0006](sdl-source://s0/1114/1159).
- [ChannelBody](sdl-source://s0/149/170): completeness = closed. Source fact: [f0009](sdl-source://s0/1210/1248).
- [ChannelEvents](sdl-source://s0/171/194): completeness = closed. Source fact: [f0011](sdl-source://s0/1285/1325).
- [ChannelNotice](sdl-source://s0/195/217): message-kind = event. Source fact: [f0014](sdl-source://s0/1400/1439).
- [ChannelUpdate](sdl-source://s0/218/240): message-kind = event. Source fact: [f0016](sdl-source://s0/1475/1514).
- [ChannelValue](sdl-source://s0/241/260): presence = required. Source fact: [f0018](sdl-source://s0/1550/1587).
- [ChannelValue](sdl-source://s0/241/260): value-type = text. Source fact: [f0019](sdl-source://s0/1588/1623).
- [DataRecord](sdl-source://s0/261/281): completeness = closed. Source fact: [f0020](sdl-source://s0/1624/1661).
- [DataValue](sdl-source://s0/282/298): presence = required. Source fact: [f0022](sdl-source://s0/1694/1728).
- [DataValue](sdl-source://s0/282/298): value-type = text. Source fact: [f0023](sdl-source://s0/1729/1761).
- [Display](sdl-source://s0/334/356): repeatability = deterministic. Source fact: [f0038](sdl-source://s0/2304/2346).
- [Display](sdl-source://s0/334/356): state-retention = stateful. Source fact: [f0039](sdl-source://s0/2347/2386).
- [Example](sdl-source://s0/357/374): completeness = open. Source fact: [f0042](sdl-source://s0/2459/2491).
- [PacketBig](sdl-source://s0/523/542): bit-order = most-significant-first. Source fact: [f0050](sdl-source://s0/2727/2776).
- [PacketBig](sdl-source://s0/523/542): byte-order = big-endian. Source fact: [f0051](sdl-source://s0/2777/2815).
- [PacketFlag](sdl-source://s0/563/580): presence = required. Source fact: [f0057](sdl-source://s0/3018/3053).
- [PacketFlag](sdl-source://s0/563/580): value-type = boolean. Source fact: [f0058](sdl-source://s0/3054/3090).
- [PacketLittle](sdl-source://s0/581/603): bit-order = least-significant-first. Source fact: [f0060](sdl-source://s0/3124/3177).
- [PacketLittle](sdl-source://s0/581/603): byte-order = little-endian. Source fact: [f0061](sdl-source://s0/3178/3222).
- [PacketNumber](sdl-source://s0/604/623): presence = required. Source fact: [f0065](sdl-source://s0/3363/3400).
- [PacketNumber](sdl-source://s0/604/623): value-type = signed. Source fact: [f0066](sdl-source://s0/3401/3438).
- [PacketPayload](sdl-source://s0/624/644): presence = required. Source fact: [f0067](sdl-source://s0/3439/3477).
- [PacketPayload](sdl-source://s0/624/644): value-type = bytes. Source fact: [f0068](sdl-source://s0/3478/3515).
- [UnusedShape](sdl-source://s0/797/818): completeness = open. Source fact: [f0077](sdl-source://s0/3830/3866).
- [WireShape](sdl-source://s0/862/881): completeness = closed. Source fact: [f0082](sdl-source://s0/3986/4022).
- [WireValue](sdl-source://s0/882/898): presence = optional. Source fact: [f0084](sdl-source://s0/4055/4089).
- [WireValue](sdl-source://s0/882/898): value-type = bytes. Source fact: [f0085](sdl-source://s0/4090/4123).

## Reported model gaps

These are observations from the producer's current gap rules across viewpoints, not an exhaustive requirements audit. A missing modeled relation need not be a missing implementation or a required feature. An absence has no source statement; links identify declared context only. No reported gaps does not establish model completeness.

### VP01 — Use cases and traceability

- NO_CONTRIBUTION — [Export](sdl-source://s0/375/390): No direct or Feature-mediated Functionality contributions.
- NO_ACTOR — [Isolated](sdl-source://s0/421/438): No Actor pursues this use case.
- NO_CONTRIBUTION — [Isolated](sdl-source://s0/421/438): No direct or Feature-mediated Functionality contributions.
- NO_GOAL — [Guest](sdl-source://s0/391/403): Actor has no pursues relation.

### VP08 — Channel contracts and sequences

- INCOMPLETE_PARTICIPATION — [Bus](sdl-source://s0/136/148); mode [Offline](sdl-source://s0/496/509): Incomplete participation for ChannelUpdate.
- NO_PARTICIPANTS — [IdleBus](sdl-source://s0/404/420): No explicit participation.

### VP10 — Datagram encoding and packets

- NO_ENCODING — [WireVariant](sdl-source://s0/899/919): No fixed wire layout; a logical variant does not define a packet.

## Declaration register

Every declaration is retained, including isolated definitions. Source links identify exact spans in the original checked files.

- [ABuild](sdl-source://s0/34/50) (activity) — Overview.design, line 2.
- [AIsolated](sdl-source://s0/51/70) (activity) — Overview.design, line 3.
- [APlan](sdl-source://s0/71/86) (activity) — Overview.design, line 4.
- [AVerify](sdl-source://s0/87/104) (activity) — Overview.design, line 5.
- [Api](sdl-source://s0/105/119) (interface) — Overview.design, line 6.
- [Browse](sdl-source://s0/120/135) (usecase) — Overview.design, line 7.
- [Bus](sdl-source://s0/136/148) (channel) — Overview.design, line 8.
- [ChannelBody](sdl-source://s0/149/170) (contract) — Overview.design, line 9.
- [ChannelEvents](sdl-source://s0/171/194) (contract) — Overview.design, line 10.
- [ChannelNotice](sdl-source://s0/195/217) (message) — Overview.design, line 11.
- [ChannelUpdate](sdl-source://s0/218/240) (message) — Overview.design, line 12.
- [ChannelValue](sdl-source://s0/241/260) (field) — Overview.design, line 13.
- [DataRecord](sdl-source://s0/261/281) (contract) — Overview.design, line 14.
- [DataValue](sdl-source://s0/282/298) (field) — Overview.design, line 15.
- [Delivery](sdl-source://s0/299/319) (capability) — Overview.design, line 16.
- [Desktop](sdl-source://s0/320/333) (unit) — Overview.design, line 17.
- [Display](sdl-source://s0/334/356) (functionality) — Overview.design, line 18.
- [Example](sdl-source://s0/357/374) (scenario) — Overview.design, line 19.
- [Export](sdl-source://s0/375/390) (usecase) — Overview.design, line 20.
- [Guest](sdl-source://s0/391/403) (actor) — Overview.design, line 21.
- [IdleBus](sdl-source://s0/404/420) (channel) — Overview.design, line 22.
- [Isolated](sdl-source://s0/421/438) (usecase) — Overview.design, line 23.
- [Local](sdl-source://s0/439/455) (container) — Overview.design, line 24.
- [MemoryData](sdl-source://s0/456/475) (dataset) — Overview.design, line 25.
- [Navigation](sdl-source://s0/476/495) (feature) — Overview.design, line 26.
- [Offline](sdl-source://s0/496/509) (mode) — Overview.design, line 27.
- [Online](sdl-source://s0/510/522) (mode) — Overview.design, line 28.
- [PacketBig](sdl-source://s0/523/542) (encoding) — Overview.design, line 29.
- [PacketBody](sdl-source://s0/543/562) (variant) — Overview.design, line 30.
- [PacketFlag](sdl-source://s0/563/580) (field) — Overview.design, line 31.
- [PacketLittle](sdl-source://s0/581/603) (encoding) — Overview.design, line 32.
- [PacketNumber](sdl-source://s0/604/623) (field) — Overview.design, line 33.
- [PacketPayload](sdl-source://s0/624/644) (field) — Overview.design, line 34.
- [PersistentStore](sdl-source://s0/645/670) (database) — Overview.design, line 35.
- [Quiet](sdl-source://s0/671/682) (mode) — Overview.design, line 36.
- [Reader](sdl-source://s0/683/696) (actor) — Overview.design, line 37.
- [Remote](sdl-source://s0/697/714) (container) — Overview.design, line 38.
- [SparePort](sdl-source://s0/715/735) (interface) — Overview.design, line 39.
- [Standalone](sdl-source://s0/736/752) (unit) — Overview.design, line 40.
- [StoredData](sdl-source://s0/753/772) (dataset) — Overview.design, line 41.
- [Unconnected](sdl-source://s0/773/796) (capability) — Overview.design, line 42.
- [UnusedShape](sdl-source://s0/797/818) (contract) — Overview.design, line 43.
- [UnusedStore](sdl-source://s0/819/840) (database) — Overview.design, line 44.
- [WireFamily](sdl-source://s0/841/861) (datagram) — Overview.design, line 45.
- [WireShape](sdl-source://s0/862/881) (contract) — Overview.design, line 46.
- [WireValue](sdl-source://s0/882/898) (field) — Overview.design, line 47.
- [WireVariant](sdl-source://s0/899/919) (variant) — Overview.design, line 48.

## Complete fact registry

All statements are preserved, including mode, ternary relation, sequence and placement arguments. Source traceability does not prove native code conformance.

- [f0000](sdl-source://s0/920/945) — ABuild addresses Display. (Overview.design, line 49).
- [f0001](sdl-source://s0/946/973) — ABuild delivers Navigation. (Overview.design, line 50).
- [f0002](sdl-source://s0/974/1021) — ABuild has implementation-status = implemented. (Overview.design, line 51).
- [f0003](sdl-source://s0/1022/1043) — ABuild refines APlan. (Overview.design, line 52).
- [f0004](sdl-source://s0/1044/1086) — APlan has implementation-status = planned. (Overview.design, line 53).
- [f0005](sdl-source://s0/1087/1113) — AVerify depends-on ABuild. (Overview.design, line 54).
- [f0006](sdl-source://s0/1114/1159) — AVerify has implementation-status = verified. (Overview.design, line 55).
- [f0007](sdl-source://s0/1160/1182) — AVerify refines APlan. (Overview.design, line 56).
- [f0008](sdl-source://s0/1183/1209) — Bus upholds ChannelEvents. (Overview.design, line 57).
- [f0009](sdl-source://s0/1210/1248) — ChannelBody has completeness = closed. (Overview.design, line 58).
- [f0010](sdl-source://s0/1249/1284) — ChannelBody has-field ChannelValue. (Overview.design, line 59).
- [f0011](sdl-source://s0/1285/1325) — ChannelEvents has completeness = closed. (Overview.design, line 60).
- [f0012](sdl-source://s0/1326/1362) — ChannelEvents permits ChannelNotice. (Overview.design, line 61).
- [f0013](sdl-source://s0/1363/1399) — ChannelEvents permits ChannelUpdate. (Overview.design, line 62).
- [f0014](sdl-source://s0/1400/1439) — ChannelNotice has message-kind = event. (Overview.design, line 63).
- [f0015](sdl-source://s0/1440/1474) — ChannelNotice upholds ChannelBody. (Overview.design, line 64).
- [f0016](sdl-source://s0/1475/1514) — ChannelUpdate has message-kind = event. (Overview.design, line 65).
- [f0017](sdl-source://s0/1515/1549) — ChannelUpdate upholds ChannelBody. (Overview.design, line 66).
- [f0018](sdl-source://s0/1550/1587) — ChannelValue has presence = required. (Overview.design, line 67).
- [f0019](sdl-source://s0/1588/1623) — ChannelValue has value-type = text. (Overview.design, line 68).
- [f0020](sdl-source://s0/1624/1661) — DataRecord has completeness = closed. (Overview.design, line 69).
- [f0021](sdl-source://s0/1662/1693) — DataRecord has-field DataValue. (Overview.design, line 70).
- [f0022](sdl-source://s0/1694/1728) — DataValue has presence = required. (Overview.design, line 71).
- [f0023](sdl-source://s0/1729/1761) — DataValue has value-type = text. (Overview.design, line 72).
- [f0024](sdl-source://s0/1762/1800) — Delivery requires Api in mode Offline. (Overview.design, line 73).
- [f0025](sdl-source://s0/1801/1844) — Delivery requires SparePort in mode Online. (Overview.design, line 74).
- [f0026](sdl-source://s0/1845/1866) — Desktop consumes Api. (Overview.design, line 75).
- [f0027](sdl-source://s0/1867/1892) — Desktop holds MemoryData. (Overview.design, line 76).
- [f0028](sdl-source://s0/1893/1914) — Desktop owns Display. (Overview.design, line 77).
- [f0029](sdl-source://s0/1915/1944) — Desktop owns PersistentStore. (Overview.design, line 78).
- [f0030](sdl-source://s0/1945/1970) — Desktop owns UnusedStore. (Overview.design, line 79).
- [f0031](sdl-source://s0/1971/1997) — Desktop provides Delivery. (Overview.design, line 80).
- [f0032](sdl-source://s0/1998/2060) — Desktop uses Bus as receiver of ChannelNotice in mode Offline. (Overview.design, line 81).
- [f0033](sdl-source://s0/2061/2120) — Desktop uses Bus as sender of ChannelNotice in mode Online. (Overview.design, line 82).
- [f0034](sdl-source://s0/2121/2180) — Desktop uses Bus as sender of ChannelUpdate in mode Online. (Overview.design, line 83).
- [f0035](sdl-source://s0/2181/2224) — Display allocated-to Local in mode Offline. (Overview.design, line 84).
- [f0036](sdl-source://s0/2225/2268) — Display allocated-to Remote in mode Online. (Overview.design, line 85).
- [f0037](sdl-source://s0/2269/2303) — Display contributes-to Navigation. (Overview.design, line 86).
- [f0038](sdl-source://s0/2304/2346) — Display has repeatability = deterministic. (Overview.design, line 87).
- [f0039](sdl-source://s0/2347/2386) — Display has state-retention = stateful. (Overview.design, line 88).
- [f0040](sdl-source://s0/2387/2431) — Display projects StoredData into WireFamily. (Overview.design, line 89).
- [f0041](sdl-source://s0/2432/2458) — Display realizes Delivery. (Overview.design, line 90).
- [f0042](sdl-source://s0/2459/2491) — Example has completeness = open. (Overview.design, line 91).
- [f0043](sdl-source://s0/2492/2515) — Example runs-in Online. (Overview.design, line 92).
- [f0044](sdl-source://s0/2516/2582) — Example step 1 sends ChannelNotice from Desktop to Remote via Bus. (Overview.design, line 93).
- [f0045](sdl-source://s0/2583/2613) — IdleBus upholds ChannelEvents. (Overview.design, line 94).
- [f0046](sdl-source://s0/2614/2637) — Local contains Desktop. (Overview.design, line 95).
- [f0047](sdl-source://s0/2638/2668) — MemoryData upholds DataRecord. (Overview.design, line 96).
- [f0048](sdl-source://s0/2669/2696) — Navigation supports Browse. (Overview.design, line 97).
- [f0049](sdl-source://s0/2697/2726) — PacketBig encodes PacketBody. (Overview.design, line 98).
- [f0050](sdl-source://s0/2727/2776) — PacketBig has bit-order = most-significant-first. (Overview.design, line 99).
- [f0051](sdl-source://s0/2777/2815) — PacketBig has byte-order = big-endian. (Overview.design, line 100).
- [f0052](sdl-source://s0/2816/2856) — PacketBig places PacketFlag at 0 bits 1. (Overview.design, line 101).
- [f0053](sdl-source://s0/2857/2900) — PacketBig places PacketNumber at 1 bits 12. (Overview.design, line 102).
- [f0054](sdl-source://s0/2901/2946) — PacketBig places PacketPayload at 13 bits 16. (Overview.design, line 103).
- [f0055](sdl-source://s0/2947/2981) — PacketBody has-field PacketNumber. (Overview.design, line 104).
- [f0056](sdl-source://s0/2982/3017) — PacketBody has-field PacketPayload. (Overview.design, line 105).
- [f0057](sdl-source://s0/3018/3053) — PacketFlag has presence = required. (Overview.design, line 106).
- [f0058](sdl-source://s0/3054/3090) — PacketFlag has value-type = boolean. (Overview.design, line 107).
- [f0059](sdl-source://s0/3091/3123) — PacketLittle encodes PacketBody. (Overview.design, line 108).
- [f0060](sdl-source://s0/3124/3177) — PacketLittle has bit-order = least-significant-first. (Overview.design, line 109).
- [f0061](sdl-source://s0/3178/3222) — PacketLittle has byte-order = little-endian. (Overview.design, line 110).
- [f0062](sdl-source://s0/3223/3266) — PacketLittle places PacketFlag at 0 bits 1. (Overview.design, line 111).
- [f0063](sdl-source://s0/3267/3313) — PacketLittle places PacketNumber at 1 bits 12. (Overview.design, line 112).
- [f0064](sdl-source://s0/3314/3362) — PacketLittle places PacketPayload at 13 bits 16. (Overview.design, line 113).
- [f0065](sdl-source://s0/3363/3400) — PacketNumber has presence = required. (Overview.design, line 114).
- [f0066](sdl-source://s0/3401/3438) — PacketNumber has value-type = signed. (Overview.design, line 115).
- [f0067](sdl-source://s0/3439/3477) — PacketPayload has presence = required. (Overview.design, line 116).
- [f0068](sdl-source://s0/3478/3515) — PacketPayload has value-type = bytes. (Overview.design, line 117).
- [f0069](sdl-source://s0/3516/3549) — PersistentStore holds StoredData. (Overview.design, line 118).
- [f0070](sdl-source://s0/3550/3572) — Reader pursues Browse. (Overview.design, line 119).
- [f0071](sdl-source://s0/3573/3595) — Reader pursues Export. (Overview.design, line 120).
- [f0072](sdl-source://s0/3596/3616) — Remote consumes Api. (Overview.design, line 121).
- [f0073](sdl-source://s0/3617/3677) — Remote uses Bus as receiver of ChannelNotice in mode Online. (Overview.design, line 122).
- [f0074](sdl-source://s0/3678/3738) — Remote uses Bus as receiver of ChannelUpdate in mode Online. (Overview.design, line 123).
- [f0075](sdl-source://s0/3739/3798) — Remote uses Bus as sender of ChannelNotice in mode Offline. (Overview.design, line 124).
- [f0076](sdl-source://s0/3799/3829) — StoredData upholds DataRecord. (Overview.design, line 125).
- [f0077](sdl-source://s0/3830/3866) — UnusedShape has completeness = open. (Overview.design, line 126).
- [f0078](sdl-source://s0/3867/3894) — WireFamily from StoredData. (Overview.design, line 127).
- [f0079](sdl-source://s0/3895/3924) — WireFamily upholds WireShape. (Overview.design, line 128).
- [f0080](sdl-source://s0/3925/3954) — WireShape defines PacketBody. (Overview.design, line 129).
- [f0081](sdl-source://s0/3955/3985) — WireShape defines WireVariant. (Overview.design, line 130).
- [f0082](sdl-source://s0/3986/4022) — WireShape has completeness = closed. (Overview.design, line 131).
- [f0083](sdl-source://s0/4023/4054) — WireShape has-field PacketFlag. (Overview.design, line 132).
- [f0084](sdl-source://s0/4055/4089) — WireValue has presence = optional. (Overview.design, line 133).
- [f0085](sdl-source://s0/4090/4123) — WireValue has value-type = bytes. (Overview.design, line 134).
- [f0086](sdl-source://s0/4124/4156) — WireVariant has-field WireValue. (Overview.design, line 135).

