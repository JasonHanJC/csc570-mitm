# csc570-mitm

To compile the project, go to the src folder and make the project.

```bash
cd source
make
```

To run the project:
1. run man-in-the-middle proxy with mode. mode=0: NOMODIFY; mode=1: IGNORE; mode=2: MODIFY

```bash
./MITM
```
2. run sever.
```bash
./Server
```
3. run client.
```bash
./Client
Please use 127.0.0.1 as the targeted server ip when running; MITM will listen to it.
```

To clean the project:
```bash
make clean
```

