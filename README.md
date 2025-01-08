# ASN1SCC Examples

## Build the examples

First build the ASN1SCC tool:

```bash
cd asn1scc
dotnet build asn1scc.sln
```

Then build the examples:

UPER:

```bash
cd ex1-uper
./build_uper.sh
```

ACN:

```bash
cd ex1-acn
./build_acn.sh
```

## Run the examples

Here is the test message:

```c
Message testMessage = {  
        .msgId  = 1,
        .myflag = 2,
        .value  = 3.14,
        .szDescription = {
             .arr = "HelloWorld"
        }
    };
```

UPER:

```bash
cd ex1-uper
./sample_test
```

expected:

```bash
❯ cd ex1-uper
❯ ./build_uper.sh
❯ ./sample_test
00 00 80 84 c0 66 8c 8f 5c 28 f5 c2 8f a4 32 b6 36 37 ab b7 b9 36 32 00 (24 bytes)
```

ACN:

```bash
cd ex1-acn
./sample_test_acn
```

expected:

```bash
❯ cd ex1-acn
❯ ./build_acn.sh
❯ ./sample_test_acn
00 01 02 40 09 1e b8 51 eb 85 1f 48 65 6c 6c 6f 57 6f 72 6c 64 (21 bytes)
```

As we can see, the specified binary format specified by the ACN config makes the output smaller than the UPER format.

## Dynamic size example

Message format:

```bash
Choice DEFINITIONS AUTOMATIC TAGS ::= BEGIN
EXPORTS ALL;
MyChoice ::= CHOICE
{
  choice1 SEQUENCE {
    fst INTEGER,
    snd INTEGER
  },
  choice2 INTEGER
}
END
```

generates this size function:

```scala
def size(offset: Long): Long = {
  require((0L <= offset) && (offset <= 9223372036854775662L))
  this match {
    case TMyChoice.choice1_PRESENT(choice1) =>
      val size_1_0 = 8L * GetLengthForEncodingSigned(choice1.fst) + 8L
      val size_1_1 = 8L * GetLengthForEncodingSigned(choice1.snd) + 8L
      size_1_0 + size_1_1
    case TMyChoice.choice2_PRESENT(choice2) =>
      8L * GetLengthForEncodingSigned(choice2) + 8L
  }
}.ensuring { (res: Long) => (0L <= res) && (res <= 145L)}
```

And as we can see, the size depends on the choice made.