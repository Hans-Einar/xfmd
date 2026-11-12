# VP08 — Channel contracts and sequences

Who communicates, what is exchanged, and in what order?

Explicit scenario steps validated against permits, participation, mode and request/result correlation.

Operating mode: [All declared modes](sdl-view://heos/VP08?collection=contracts&consumer=xfmd&projection=reading&target=main) · [MicroConsole](sdl-view://heos/VP08?collection=contracts&consumer=xfmd&mode=MicroConsole&projection=reading&target=main) · [MicroCore](sdl-view://heos/VP08?collection=contracts&consumer=xfmd&mode=MicroCore&projection=reading&target=main) · [MicroRetained](sdl-view://heos/VP08?collection=contracts&consumer=xfmd&mode=MicroRetained&projection=reading&target=main)

[Viewpoint overview](sdl-view://heos/VP08?consumer=xfmd&projection=reading&target=main)

## Contracts

Showing 1–13 of 13; page 1. The complete scope is retained across pages.

### Contents

- [CandidateLocalAddressContract](sdl-view://heos/VP08?consumer=xfmd&focus=CandidateLocalAddressContract&projection=reading&target=main) (contract)
- [CandidatePresentationContract](sdl-view://heos/VP08?consumer=xfmd&focus=CandidatePresentationContract&projection=reading&target=main) (contract)
- [CommittedRecordsShape](sdl-view://heos/VP08?consumer=xfmd&focus=CommittedRecordsShape&projection=reading&target=main) (contract)
- [NativeTaskApiContract](sdl-view://heos/VP08?consumer=xfmd&focus=NativeTaskApiContract&projection=reading&target=main) (contract)
- [NumericCommandApiContract](sdl-view://heos/VP08?consumer=xfmd&focus=NumericCommandApiContract&projection=reading&target=main) (contract)
- [NumericRequestShape](sdl-view://heos/VP08?consumer=xfmd&focus=NumericRequestShape&projection=reading&target=main) (contract)
- [NumericValueApiContract](sdl-view://heos/VP08?consumer=xfmd&focus=NumericValueApiContract&projection=reading&target=main) (contract)
- [OperationResultShape](sdl-view://heos/VP08?consumer=xfmd&focus=OperationResultShape&projection=reading&target=main) (contract)
- [RegistryBindingsShape](sdl-view://heos/VP08?consumer=xfmd&focus=RegistryBindingsShape&projection=reading&target=main) (contract)
- [RetainedApiContract](sdl-view://heos/VP08?consumer=xfmd&focus=RetainedApiContract&projection=reading&target=main) (contract)
- [RetainedRequestShape](sdl-view://heos/VP08?consumer=xfmd&focus=RetainedRequestShape&projection=reading&target=main) (contract)
- [ScheduleStateShape](sdl-view://heos/VP08?consumer=xfmd&focus=ScheduleStateShape&projection=reading&target=main) (contract)
- [TaskInvocationShape](sdl-view://heos/VP08?consumer=xfmd&focus=TaskInvocationShape&projection=reading&target=main) (contract)

Each chapter uses the same model content as its object detail. Definitions without relationships remain visible. Large diagram sets are deferred to object details; all chapter statements and object links remain present.

## CandidateLocalAddressContract

contract declaration: [CandidateLocalAddressContract — Contracts/MicroValues.design, line 5](sdl-source://s2/123/162).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [CandidateLocalAddress](sdl-view://heos/VP08?consumer=xfmd&focus=CandidateLocalAddress&projection=reading&target=main).

### CandidateLocalAddressByte fields

| Field | Value type | Presence | Source statement |
| --- | --- | --- | --- |
| [GroupNibble](sdl-view://heos/VP08?consumer=xfmd&focus=GroupNibble&projection=reading&target=main) | unsigned | required | [CandidateLocalAddressByte has-field GroupNibble. — Contracts/MicroValues.design, line 38](sdl-source://s2/1074/1122) |
| [MemberNibble](sdl-view://heos/VP08?consumer=xfmd&focus=MemberNibble&projection=reading&target=main) | unsigned | required | [CandidateLocalAddressByte has-field MemberNibble. — Contracts/MicroValues.design, line 39](sdl-source://s2/1123/1172) |

Declared variants: [CandidateLocalAddressByte](sdl-view://heos/VP08?consumer=xfmd&focus=CandidateLocalAddressByte&projection=reading&target=main).

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [CandidateLocalAddressContract has completeness = closed. — Contracts/MicroValues.design, line 41](sdl-source://s2/1238/1294) |

### Declared relationships

- [CandidateLocalAddress upholds CandidateLocalAddressContract. — Contracts/MicroValues.design, line 37](sdl-source://s2/1013/1073)
- [CandidateLocalAddressByte has-field GroupNibble. — Contracts/MicroValues.design, line 38](sdl-source://s2/1074/1122)
- [CandidateLocalAddressByte has-field MemberNibble. — Contracts/MicroValues.design, line 39](sdl-source://s2/1123/1172)
- [CandidateLocalAddressContract defines CandidateLocalAddressByte. — Contracts/MicroValues.design, line 40](sdl-source://s2/1173/1237)

### Related objects in this viewpoint

[CandidateLocalAddress](sdl-view://heos/VP08?consumer=xfmd&focus=CandidateLocalAddress&projection=reading&target=main), [CandidateLocalAddressByte](sdl-view://heos/VP08?consumer=xfmd&focus=CandidateLocalAddressByte&projection=reading&target=main), [GroupNibble](sdl-view://heos/VP08?consumer=xfmd&focus=GroupNibble&projection=reading&target=main), [MemberNibble](sdl-view://heos/VP08?consumer=xfmd&focus=MemberNibble&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## CandidatePresentationContract

contract declaration: [CandidatePresentationContract — Contracts/MicroValues.design, line 9](sdl-source://s2/270/309).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [CandidatePresentation](sdl-view://heos/VP08?consumer=xfmd&focus=CandidatePresentation&projection=reading&target=main).

### CandidatePresentationByte fields

| Field | Value type | Presence | Source statement |
| --- | --- | --- | --- |
| [DecimalPlaces](sdl-view://heos/VP08?consumer=xfmd&focus=DecimalPlaces&projection=reading&target=main) | unsigned | required | [CandidatePresentationByte has-field DecimalPlaces. — Contracts/MicroValues.design, line 49](sdl-source://s2/1720/1770) |
| [UnitCode](sdl-view://heos/VP08?consumer=xfmd&focus=UnitCode&projection=reading&target=main) | unsigned | required | [CandidatePresentationByte has-field UnitCode. — Contracts/MicroValues.design, line 50](sdl-source://s2/1771/1816) |

Declared variants: [CandidatePresentationByte](sdl-view://heos/VP08?consumer=xfmd&focus=CandidatePresentationByte&projection=reading&target=main).

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [CandidatePresentationContract has completeness = closed. — Contracts/MicroValues.design, line 52](sdl-source://s2/1882/1938) |

### Declared relationships

- [CandidatePresentation upholds CandidatePresentationContract. — Contracts/MicroValues.design, line 48](sdl-source://s2/1659/1719)
- [CandidatePresentationByte has-field DecimalPlaces. — Contracts/MicroValues.design, line 49](sdl-source://s2/1720/1770)
- [CandidatePresentationByte has-field UnitCode. — Contracts/MicroValues.design, line 50](sdl-source://s2/1771/1816)
- [CandidatePresentationContract defines CandidatePresentationByte. — Contracts/MicroValues.design, line 51](sdl-source://s2/1817/1881)

### Related objects in this viewpoint

[CandidatePresentation](sdl-view://heos/VP08?consumer=xfmd&focus=CandidatePresentation&projection=reading&target=main), [CandidatePresentationByte](sdl-view://heos/VP08?consumer=xfmd&focus=CandidatePresentationByte&projection=reading&target=main), [DecimalPlaces](sdl-view://heos/VP08?consumer=xfmd&focus=DecimalPlaces&projection=reading&target=main), [UnitCode](sdl-view://heos/VP08?consumer=xfmd&focus=UnitCode&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## CommittedRecordsShape

contract declaration: [CommittedRecordsShape — Contracts/MicroValues.design, line 14](sdl-source://s2/427/458).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [CommittedRecords](sdl-view://heos/VP08?consumer=xfmd&focus=CommittedRecords&projection=reading&target=main).

### CommittedRecordsShape fields

| Field | Value type | Presence | Source statement |
| --- | --- | --- | --- |
| [CommittedGeneration](sdl-view://heos/VP08?consumer=xfmd&focus=CommittedGeneration&projection=reading&target=main) | unsigned | required | [CommittedRecordsShape has-field CommittedGeneration. — Contracts/MicroValues.design, line 64](sdl-source://s2/2528/2580) |
| [CommittedPayload](sdl-view://heos/VP08?consumer=xfmd&focus=CommittedPayload&projection=reading&target=main) | bytes | required | [CommittedRecordsShape has-field CommittedPayload. — Contracts/MicroValues.design, line 65](sdl-source://s2/2581/2630) |
| [IntegrityMarker](sdl-view://heos/VP08?consumer=xfmd&focus=IntegrityMarker&projection=reading&target=main) | unsigned | required | [CommittedRecordsShape has-field IntegrityMarker. — Contracts/MicroValues.design, line 66](sdl-source://s2/2631/2679) |

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [CommittedRecordsShape has completeness = closed. — Contracts/MicroValues.design, line 63](sdl-source://s2/2479/2527) |

### Declared relationships

- [CommittedRecords upholds CommittedRecordsShape. — Contracts/MicroValues.design, line 62](sdl-source://s2/2431/2478)
- [CommittedRecordsShape has-field CommittedGeneration. — Contracts/MicroValues.design, line 64](sdl-source://s2/2528/2580)
- [CommittedRecordsShape has-field CommittedPayload. — Contracts/MicroValues.design, line 65](sdl-source://s2/2581/2630)
- [CommittedRecordsShape has-field IntegrityMarker. — Contracts/MicroValues.design, line 66](sdl-source://s2/2631/2679)

### Related objects in this viewpoint

[CommittedGeneration](sdl-view://heos/VP08?consumer=xfmd&focus=CommittedGeneration&projection=reading&target=main), [CommittedPayload](sdl-view://heos/VP08?consumer=xfmd&focus=CommittedPayload&projection=reading&target=main), [CommittedRecords](sdl-view://heos/VP08?consumer=xfmd&focus=CommittedRecords&projection=reading&target=main), [IntegrityMarker](sdl-view://heos/VP08?consumer=xfmd&focus=IntegrityMarker&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## NativeTaskApiContract

contract declaration: [NativeTaskApiContract — Contracts/MicroServices.design, line 8](sdl-source://s1/174/205).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [NativeTaskApi](sdl-view://heos/VP08?consumer=xfmd&focus=NativeTaskApi&projection=reading&target=main).

Allowed payloads: [TaskInvocation](sdl-view://heos/VP08?consumer=xfmd&focus=TaskInvocation&projection=reading&target=main), [TaskReturn](sdl-view://heos/VP08?consumer=xfmd&focus=TaskReturn&projection=reading&target=main). Request/result correlation requires explicit replies-to relationships and scenario steps.

No record fields are declared for this contract or its variants. A channel contract may describe permitted messages without defining record fields.

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [NativeTaskApiContract has completeness = closed. — Contracts/MicroServices.design, line 54](sdl-source://s1/2058/2106) |

### Declared relationships

- [NativeTaskApi upholds NativeTaskApiContract. — Contracts/MicroServices.design, line 53](sdl-source://s1/2013/2057)
- [NativeTaskApiContract permits TaskInvocation. — Contracts/MicroServices.design, line 55](sdl-source://s1/2107/2152)
- [NativeTaskApiContract permits TaskReturn. — Contracts/MicroServices.design, line 56](sdl-source://s1/2153/2194)

### Related objects in this viewpoint

[NativeTaskApi](sdl-view://heos/VP08?consumer=xfmd&focus=NativeTaskApi&projection=reading&target=main), [TaskInvocation](sdl-view://heos/VP08?consumer=xfmd&focus=TaskInvocation&projection=reading&target=main), [TaskReturn](sdl-view://heos/VP08?consumer=xfmd&focus=TaskReturn&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## NumericCommandApiContract

contract declaration: [NumericCommandApiContract — Contracts/MicroServices.design, line 10](sdl-source://s1/233/268).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [NumericCommandApi](sdl-view://heos/VP08?consumer=xfmd&focus=NumericCommandApi&projection=reading&target=main).

Allowed payloads: [CommandRequest](sdl-view://heos/VP08?consumer=xfmd&focus=CommandRequest&projection=reading&target=main), [CommandResult](sdl-view://heos/VP08?consumer=xfmd&focus=CommandResult&projection=reading&target=main). Request/result correlation requires explicit replies-to relationships and scenario steps.

No record fields are declared for this contract or its variants. A channel contract may describe permitted messages without defining record fields.

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [NumericCommandApiContract has completeness = closed. — Contracts/MicroServices.design, line 58](sdl-source://s1/2248/2300) |

### Declared relationships

- [NumericCommandApi upholds NumericCommandApiContract. — Contracts/MicroServices.design, line 57](sdl-source://s1/2195/2247)
- [NumericCommandApiContract permits CommandRequest. — Contracts/MicroServices.design, line 59](sdl-source://s1/2301/2350)
- [NumericCommandApiContract permits CommandResult. — Contracts/MicroServices.design, line 60](sdl-source://s1/2351/2399)

### Related objects in this viewpoint

[CommandRequest](sdl-view://heos/VP08?consumer=xfmd&focus=CommandRequest&projection=reading&target=main), [CommandResult](sdl-view://heos/VP08?consumer=xfmd&focus=CommandResult&projection=reading&target=main), [NumericCommandApi](sdl-view://heos/VP08?consumer=xfmd&focus=NumericCommandApi&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## NumericRequestShape

contract declaration: [NumericRequestShape — Contracts/MicroServices.design, line 11](sdl-source://s1/269/298).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [CommandRequest](sdl-view://heos/VP08?consumer=xfmd&focus=CommandRequest&projection=reading&target=main), [ReadValueRequest](sdl-view://heos/VP08?consumer=xfmd&focus=ReadValueRequest&projection=reading&target=main), [WriteValueRequest](sdl-view://heos/VP08?consumer=xfmd&focus=WriteValueRequest&projection=reading&target=main).

### NumericRequestShape fields

| Field | Value type | Presence | Source statement |
| --- | --- | --- | --- |
| [RequestMember](sdl-view://heos/VP08?consumer=xfmd&focus=RequestMember&projection=reading&target=main) | unsigned | required | [NumericRequestShape has-field RequestMember. — Contracts/MicroServices.design, line 62](sdl-source://s1/2447/2491) |
| [RequestOwner](sdl-view://heos/VP08?consumer=xfmd&focus=RequestOwner&projection=reading&target=main) | unsigned | required | [NumericRequestShape has-field RequestOwner. — Contracts/MicroServices.design, line 63](sdl-source://s1/2492/2535) |
| [RequestPayload](sdl-view://heos/VP08?consumer=xfmd&focus=RequestPayload&projection=reading&target=main) | bytes | optional | [NumericRequestShape has-field RequestPayload. — Contracts/MicroServices.design, line 64](sdl-source://s1/2536/2581) |

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [NumericRequestShape has completeness = closed. — Contracts/MicroServices.design, line 61](sdl-source://s1/2400/2446) |

### Declared relationships

- [CommandRequest upholds NumericRequestShape. — Contracts/MicroServices.design, line 42](sdl-source://s1/1547/1590)
- [NumericRequestShape has-field RequestMember. — Contracts/MicroServices.design, line 62](sdl-source://s1/2447/2491)
- [NumericRequestShape has-field RequestOwner. — Contracts/MicroServices.design, line 63](sdl-source://s1/2492/2535)
- [NumericRequestShape has-field RequestPayload. — Contracts/MicroServices.design, line 64](sdl-source://s1/2536/2581)
- [ReadValueRequest upholds NumericRequestShape. — Contracts/MicroServices.design, line 75](sdl-source://s1/3066/3111)
- [WriteValueRequest upholds NumericRequestShape. — Contracts/MicroServices.design, line 128](sdl-source://s1/5970/6016)

### Related objects in this viewpoint

[CommandRequest](sdl-view://heos/VP08?consumer=xfmd&focus=CommandRequest&projection=reading&target=main), [ReadValueRequest](sdl-view://heos/VP08?consumer=xfmd&focus=ReadValueRequest&projection=reading&target=main), [RequestMember](sdl-view://heos/VP08?consumer=xfmd&focus=RequestMember&projection=reading&target=main), [RequestOwner](sdl-view://heos/VP08?consumer=xfmd&focus=RequestOwner&projection=reading&target=main), [RequestPayload](sdl-view://heos/VP08?consumer=xfmd&focus=RequestPayload&projection=reading&target=main), [WriteValueRequest](sdl-view://heos/VP08?consumer=xfmd&focus=WriteValueRequest&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## NumericValueApiContract

contract declaration: [NumericValueApiContract — Contracts/MicroServices.design, line 13](sdl-source://s1/324/357).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [NumericValueApi](sdl-view://heos/VP08?consumer=xfmd&focus=NumericValueApi&projection=reading&target=main).

Allowed payloads: [ReadValueRequest](sdl-view://heos/VP08?consumer=xfmd&focus=ReadValueRequest&projection=reading&target=main), [ReadValueResult](sdl-view://heos/VP08?consumer=xfmd&focus=ReadValueResult&projection=reading&target=main), [WriteValueRequest](sdl-view://heos/VP08?consumer=xfmd&focus=WriteValueRequest&projection=reading&target=main), [WriteValueResult](sdl-view://heos/VP08?consumer=xfmd&focus=WriteValueResult&projection=reading&target=main). Request/result correlation requires explicit replies-to relationships and scenario steps.

No record fields are declared for this contract or its variants. A channel contract may describe permitted messages without defining record fields.

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [NumericValueApiContract has completeness = closed. — Contracts/MicroServices.design, line 66](sdl-source://s1/2631/2681) |

### Declared relationships

- [NumericValueApi upholds NumericValueApiContract. — Contracts/MicroServices.design, line 65](sdl-source://s1/2582/2630)
- [NumericValueApiContract permits ReadValueRequest. — Contracts/MicroServices.design, line 67](sdl-source://s1/2682/2731)
- [NumericValueApiContract permits ReadValueResult. — Contracts/MicroServices.design, line 68](sdl-source://s1/2732/2780)
- [NumericValueApiContract permits WriteValueRequest. — Contracts/MicroServices.design, line 69](sdl-source://s1/2781/2831)
- [NumericValueApiContract permits WriteValueResult. — Contracts/MicroServices.design, line 70](sdl-source://s1/2832/2881)

### Related objects in this viewpoint

[NumericValueApi](sdl-view://heos/VP08?consumer=xfmd&focus=NumericValueApi&projection=reading&target=main), [ReadValueRequest](sdl-view://heos/VP08?consumer=xfmd&focus=ReadValueRequest&projection=reading&target=main), [ReadValueResult](sdl-view://heos/VP08?consumer=xfmd&focus=ReadValueResult&projection=reading&target=main), [WriteValueRequest](sdl-view://heos/VP08?consumer=xfmd&focus=WriteValueRequest&projection=reading&target=main), [WriteValueResult](sdl-view://heos/VP08?consumer=xfmd&focus=WriteValueResult&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## OperationResultShape

contract declaration: [OperationResultShape — Contracts/MicroServices.design, line 14](sdl-source://s1/358/388).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [CommandResult](sdl-view://heos/VP08?consumer=xfmd&focus=CommandResult&projection=reading&target=main), [CommitResult](sdl-view://heos/VP08?consumer=xfmd&focus=CommitResult&projection=reading&target=main), [ReadValueResult](sdl-view://heos/VP08?consumer=xfmd&focus=ReadValueResult&projection=reading&target=main), [RecoverResult](sdl-view://heos/VP08?consumer=xfmd&focus=RecoverResult&projection=reading&target=main), [TaskReturn](sdl-view://heos/VP08?consumer=xfmd&focus=TaskReturn&projection=reading&target=main), [WriteValueResult](sdl-view://heos/VP08?consumer=xfmd&focus=WriteValueResult&projection=reading&target=main).

### OperationResultShape fields

| Field | Value type | Presence | Source statement |
| --- | --- | --- | --- |
| [ResultPayload](sdl-view://heos/VP08?consumer=xfmd&focus=ResultPayload&projection=reading&target=main) | bytes | optional | [OperationResultShape has-field ResultPayload. — Contracts/MicroServices.design, line 72](sdl-source://s1/2930/2975) |
| [ResultStatus](sdl-view://heos/VP08?consumer=xfmd&focus=ResultStatus&projection=reading&target=main) | unsigned | required | [OperationResultShape has-field ResultStatus. — Contracts/MicroServices.design, line 73](sdl-source://s1/2976/3020) |

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [OperationResultShape has completeness = closed. — Contracts/MicroServices.design, line 71](sdl-source://s1/2882/2929) |

### Declared relationships

- [CommandResult upholds OperationResultShape. — Contracts/MicroServices.design, line 45](sdl-source://s1/1673/1716)
- [CommitResult upholds OperationResultShape. — Contracts/MicroServices.design, line 50](sdl-source://s1/1882/1924)
- [OperationResultShape has-field ResultPayload. — Contracts/MicroServices.design, line 72](sdl-source://s1/2930/2975)
- [OperationResultShape has-field ResultStatus. — Contracts/MicroServices.design, line 73](sdl-source://s1/2976/3020)
- [ReadValueResult upholds OperationResultShape. — Contracts/MicroServices.design, line 78](sdl-source://s1/3200/3245)
- [RecoverResult upholds OperationResultShape. — Contracts/MicroServices.design, line 85](sdl-source://s1/3493/3536)
- [TaskReturn upholds OperationResultShape. — Contracts/MicroServices.design, line 122](sdl-source://s1/5531/5571)
- [WriteValueResult upholds OperationResultShape. — Contracts/MicroServices.design, line 131](sdl-source://s1/6108/6154)

### Related objects in this viewpoint

[CommandResult](sdl-view://heos/VP08?consumer=xfmd&focus=CommandResult&projection=reading&target=main), [CommitResult](sdl-view://heos/VP08?consumer=xfmd&focus=CommitResult&projection=reading&target=main), [ReadValueResult](sdl-view://heos/VP08?consumer=xfmd&focus=ReadValueResult&projection=reading&target=main), [RecoverResult](sdl-view://heos/VP08?consumer=xfmd&focus=RecoverResult&projection=reading&target=main), [ResultPayload](sdl-view://heos/VP08?consumer=xfmd&focus=ResultPayload&projection=reading&target=main), [ResultStatus](sdl-view://heos/VP08?consumer=xfmd&focus=ResultStatus&projection=reading&target=main), [TaskReturn](sdl-view://heos/VP08?consumer=xfmd&focus=TaskReturn&projection=reading&target=main), [WriteValueResult](sdl-view://heos/VP08?consumer=xfmd&focus=WriteValueResult&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## RegistryBindingsShape

contract declaration: [RegistryBindingsShape — Contracts/MicroValues.design, line 26](sdl-source://s2/701/732).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [RegistryBindings](sdl-view://heos/VP08?consumer=xfmd&focus=RegistryBindings&projection=reading&target=main).

### RegistryBindingsShape fields

| Field | Value type | Presence | Source statement |
| --- | --- | --- | --- |
| [BindingOwnerId](sdl-view://heos/VP08?consumer=xfmd&focus=BindingOwnerId&projection=reading&target=main) | unsigned | required | [RegistryBindingsShape has-field BindingOwnerId. — Contracts/MicroValues.design, line 89](sdl-source://s2/3585/3632) |
| [DisplayName](sdl-view://heos/VP08?consumer=xfmd&focus=DisplayName&projection=reading&target=main) | text | optional | [RegistryBindingsShape has-field DisplayName. — Contracts/MicroValues.design, line 90](sdl-source://s2/3633/3677) |
| [ExternalAccessPolicy](sdl-view://heos/VP08?consumer=xfmd&focus=ExternalAccessPolicy&projection=reading&target=main) | unsigned | required | [RegistryBindingsShape has-field ExternalAccessPolicy. — Contracts/MicroValues.design, line 91](sdl-source://s2/3678/3731) |
| [PackedLocalId](sdl-view://heos/VP08?consumer=xfmd&focus=PackedLocalId&projection=reading&target=main) | unsigned | required | [RegistryBindingsShape has-field PackedLocalId. — Contracts/MicroValues.design, line 92](sdl-source://s2/3732/3778) |
| [PresentationByte](sdl-view://heos/VP08?consumer=xfmd&focus=PresentationByte&projection=reading&target=main) | unsigned | optional | [RegistryBindingsShape has-field PresentationByte. — Contracts/MicroValues.design, line 93](sdl-source://s2/3779/3828) |
| [ScalarKind](sdl-view://heos/VP08?consumer=xfmd&focus=ScalarKind&projection=reading&target=main) | unsigned | required | [RegistryBindingsShape has-field ScalarKind. — Contracts/MicroValues.design, line 94](sdl-source://s2/3829/3872) |
| [ScalarReference](sdl-view://heos/VP08?consumer=xfmd&focus=ScalarReference&projection=reading&target=main) | bytes | required | [RegistryBindingsShape has-field ScalarReference. — Contracts/MicroValues.design, line 95](sdl-source://s2/3873/3921) |

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [RegistryBindingsShape has completeness = closed. — Contracts/MicroValues.design, line 88](sdl-source://s2/3536/3584) |

### Declared relationships

- [RegistryBindings upholds RegistryBindingsShape. — Contracts/MicroValues.design, line 87](sdl-source://s2/3488/3535)
- [RegistryBindingsShape has-field BindingOwnerId. — Contracts/MicroValues.design, line 89](sdl-source://s2/3585/3632)
- [RegistryBindingsShape has-field DisplayName. — Contracts/MicroValues.design, line 90](sdl-source://s2/3633/3677)
- [RegistryBindingsShape has-field ExternalAccessPolicy. — Contracts/MicroValues.design, line 91](sdl-source://s2/3678/3731)
- [RegistryBindingsShape has-field PackedLocalId. — Contracts/MicroValues.design, line 92](sdl-source://s2/3732/3778)
- [RegistryBindingsShape has-field PresentationByte. — Contracts/MicroValues.design, line 93](sdl-source://s2/3779/3828)
- [RegistryBindingsShape has-field ScalarKind. — Contracts/MicroValues.design, line 94](sdl-source://s2/3829/3872)
- [RegistryBindingsShape has-field ScalarReference. — Contracts/MicroValues.design, line 95](sdl-source://s2/3873/3921)

### Related objects in this viewpoint

[BindingOwnerId](sdl-view://heos/VP08?consumer=xfmd&focus=BindingOwnerId&projection=reading&target=main), [DisplayName](sdl-view://heos/VP08?consumer=xfmd&focus=DisplayName&projection=reading&target=main), [ExternalAccessPolicy](sdl-view://heos/VP08?consumer=xfmd&focus=ExternalAccessPolicy&projection=reading&target=main), [PackedLocalId](sdl-view://heos/VP08?consumer=xfmd&focus=PackedLocalId&projection=reading&target=main), [PresentationByte](sdl-view://heos/VP08?consumer=xfmd&focus=PresentationByte&projection=reading&target=main), [RegistryBindings](sdl-view://heos/VP08?consumer=xfmd&focus=RegistryBindings&projection=reading&target=main), [ScalarKind](sdl-view://heos/VP08?consumer=xfmd&focus=ScalarKind&projection=reading&target=main), [ScalarReference](sdl-view://heos/VP08?consumer=xfmd&focus=ScalarReference&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## RetainedApiContract

contract declaration: [RetainedApiContract — Contracts/MicroServices.design, line 26](sdl-source://s1/633/662).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [RetainedApi](sdl-view://heos/VP08?consumer=xfmd&focus=RetainedApi&projection=reading&target=main).

Allowed payloads: [CommitRequest](sdl-view://heos/VP08?consumer=xfmd&focus=CommitRequest&projection=reading&target=main), [CommitResult](sdl-view://heos/VP08?consumer=xfmd&focus=CommitResult&projection=reading&target=main), [RecoverRequest](sdl-view://heos/VP08?consumer=xfmd&focus=RecoverRequest&projection=reading&target=main), [RecoverResult](sdl-view://heos/VP08?consumer=xfmd&focus=RecoverResult&projection=reading&target=main). Request/result correlation requires explicit replies-to relationships and scenario steps.

No record fields are declared for this contract or its variants. A channel contract may describe permitted messages without defining record fields.

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [RetainedApiContract has completeness = closed. — Contracts/MicroServices.design, line 97](sdl-source://s1/3970/4016) |

### Declared relationships

- [RetainedApi upholds RetainedApiContract. — Contracts/MicroServices.design, line 96](sdl-source://s1/3929/3969)
- [RetainedApiContract permits CommitRequest. — Contracts/MicroServices.design, line 98](sdl-source://s1/4017/4059)
- [RetainedApiContract permits CommitResult. — Contracts/MicroServices.design, line 99](sdl-source://s1/4060/4101)
- [RetainedApiContract permits RecoverRequest. — Contracts/MicroServices.design, line 100](sdl-source://s1/4102/4145)
- [RetainedApiContract permits RecoverResult. — Contracts/MicroServices.design, line 101](sdl-source://s1/4146/4188)

### Related objects in this viewpoint

[CommitRequest](sdl-view://heos/VP08?consumer=xfmd&focus=CommitRequest&projection=reading&target=main), [CommitResult](sdl-view://heos/VP08?consumer=xfmd&focus=CommitResult&projection=reading&target=main), [RecoverRequest](sdl-view://heos/VP08?consumer=xfmd&focus=RecoverRequest&projection=reading&target=main), [RecoverResult](sdl-view://heos/VP08?consumer=xfmd&focus=RecoverResult&projection=reading&target=main), [RetainedApi](sdl-view://heos/VP08?consumer=xfmd&focus=RetainedApi&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## RetainedRequestShape

contract declaration: [RetainedRequestShape — Contracts/MicroServices.design, line 27](sdl-source://s1/663/693).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [CommitRequest](sdl-view://heos/VP08?consumer=xfmd&focus=CommitRequest&projection=reading&target=main), [RecoverRequest](sdl-view://heos/VP08?consumer=xfmd&focus=RecoverRequest&projection=reading&target=main).

### RetainedRequestShape fields

| Field | Value type | Presence | Source statement |
| --- | --- | --- | --- |
| [RecordPayload](sdl-view://heos/VP08?consumer=xfmd&focus=RecordPayload&projection=reading&target=main) | bytes | optional | [RetainedRequestShape has-field RecordPayload. — Contracts/MicroServices.design, line 103](sdl-source://s1/4237/4282) |

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [RetainedRequestShape has completeness = closed. — Contracts/MicroServices.design, line 102](sdl-source://s1/4189/4236) |

### Declared relationships

- [CommitRequest upholds RetainedRequestShape. — Contracts/MicroServices.design, line 47](sdl-source://s1/1759/1802)
- [RecoverRequest upholds RetainedRequestShape. — Contracts/MicroServices.design, line 82](sdl-source://s1/3366/3410)
- [RetainedRequestShape has-field RecordPayload. — Contracts/MicroServices.design, line 103](sdl-source://s1/4237/4282)

### Related objects in this viewpoint

[CommitRequest](sdl-view://heos/VP08?consumer=xfmd&focus=CommitRequest&projection=reading&target=main), [RecordPayload](sdl-view://heos/VP08?consumer=xfmd&focus=RecordPayload&projection=reading&target=main), [RecoverRequest](sdl-view://heos/VP08?consumer=xfmd&focus=RecoverRequest&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## ScheduleStateShape

contract declaration: [ScheduleStateShape — Contracts/MicroValues.design, line 31](sdl-source://s2/822/850).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [ScheduleState](sdl-view://heos/VP08?consumer=xfmd&focus=ScheduleState&projection=reading&target=main).

### ScheduleStateShape fields

| Field | Value type | Presence | Source statement |
| --- | --- | --- | --- |
| [CurrentPeriod](sdl-view://heos/VP08?consumer=xfmd&focus=CurrentPeriod&projection=reading&target=main) | unsigned | required | [ScheduleStateShape has-field CurrentPeriod. — Contracts/MicroValues.design, line 104](sdl-source://s2/4241/4284) |
| [LastDispatch](sdl-view://heos/VP08?consumer=xfmd&focus=LastDispatch&projection=reading&target=main) | unsigned | required | [ScheduleStateShape has-field LastDispatch. — Contracts/MicroValues.design, line 105](sdl-source://s2/4285/4327) |
| [TaskEnabled](sdl-view://heos/VP08?consumer=xfmd&focus=TaskEnabled&projection=reading&target=main) | boolean | required | [ScheduleStateShape has-field TaskEnabled. — Contracts/MicroValues.design, line 106](sdl-source://s2/4328/4369) |

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [ScheduleStateShape has completeness = closed. — Contracts/MicroValues.design, line 103](sdl-source://s2/4195/4240) |

### Declared relationships

- [ScheduleState upholds ScheduleStateShape. — Contracts/MicroValues.design, line 102](sdl-source://s2/4153/4194)
- [ScheduleStateShape has-field CurrentPeriod. — Contracts/MicroValues.design, line 104](sdl-source://s2/4241/4284)
- [ScheduleStateShape has-field LastDispatch. — Contracts/MicroValues.design, line 105](sdl-source://s2/4285/4327)
- [ScheduleStateShape has-field TaskEnabled. — Contracts/MicroValues.design, line 106](sdl-source://s2/4328/4369)

### Related objects in this viewpoint

[CurrentPeriod](sdl-view://heos/VP08?consumer=xfmd&focus=CurrentPeriod&projection=reading&target=main), [LastDispatch](sdl-view://heos/VP08?consumer=xfmd&focus=LastDispatch&projection=reading&target=main), [ScheduleState](sdl-view://heos/VP08?consumer=xfmd&focus=ScheduleState&projection=reading&target=main), [TaskEnabled](sdl-view://heos/VP08?consumer=xfmd&focus=TaskEnabled&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

## TaskInvocationShape

contract declaration: [TaskInvocationShape — Contracts/MicroServices.design, line 29](sdl-source://s1/718/747).

Completeness: **closed**. This is a declared shape constraint, not observed validation or delivery.

Used by: [TaskInvocation](sdl-view://heos/VP08?consumer=xfmd&focus=TaskInvocation&projection=reading&target=main).

### TaskInvocationShape fields

| Field | Value type | Presence | Source statement |
| --- | --- | --- | --- |
| [DispatchTimestamp](sdl-view://heos/VP08?consumer=xfmd&focus=DispatchTimestamp&projection=reading&target=main) | unsigned | required | [TaskInvocationShape has-field DispatchTimestamp. — Contracts/MicroServices.design, line 119](sdl-source://s1/5406/5454) |

### Explicit properties

| Property | Value | Source statement |
| --- | --- | --- |
| completeness | closed | [TaskInvocationShape has completeness = closed. — Contracts/MicroServices.design, line 118](sdl-source://s1/5359/5405) |

### Declared relationships

- [TaskInvocation upholds TaskInvocationShape. — Contracts/MicroServices.design, line 117](sdl-source://s1/5315/5358)
- [TaskInvocationShape has-field DispatchTimestamp. — Contracts/MicroServices.design, line 119](sdl-source://s1/5406/5454)

### Related objects in this viewpoint

[DispatchTimestamp](sdl-view://heos/VP08?consumer=xfmd&focus=DispatchTimestamp&projection=reading&target=main), [TaskInvocation](sdl-view://heos/VP08?consumer=xfmd&focus=TaskInvocation&projection=reading&target=main)

## Interpretation and unspecified behavior

Participation topology is static. Only numbered steps within one scenario establish an order. Contracts declare allowed payloads; participation does not prove delivery. No transport, precondition, or runtime scheduling is inferred.

Showing 1–13 of 13; page 1. The complete scope is retained across pages.

