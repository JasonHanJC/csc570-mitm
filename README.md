# csc570-mitm

To compile the project, go to the src folder and make the project.

```bash
cd source
make
```

To run the project:
1. run man-in-the-middle proxy with mode. mode=0: NOMODIFY; mode=1: IGNORE; mode=3: MODIFY

```bash
./mitm-proxy [mode]
```
2. run sever.
```bash
./server
```
3. run client.
```bash
./client
```

To clean the project:
```bash
make clean
```

