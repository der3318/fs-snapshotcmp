## ⚖ Snapshot Diff Tool

![c++](https://img.shields.io/badge/c++-standard%2017-blue.svg)
![ide](https://img.shields.io/badge/ide-Visual%20Studio%202019-green.svg)
![flavor](https://img.shields.io/badge/flavor-win32%20x86-brightgreen.svg)
![size](https://img.shields.io/badge/size-73%20KB-yellow.svg)

Quick and portable diff utility to make snapshots of a folder and compare the payload.

![Demo.png](/Demo.png)

 
### ▶️ Getting Started
1. download [fscmp.exe](https://github.com/der3318/fs-snapshotcmp/releases/download/2021.04.25/fscmp.exe)  from release page
2. open `cmd.exe` and cd to where the binary locates
3. run `fscmp.exe --snapshot [TARGET_DIR] [SNAPSHOT_1]` to make a snapshot of `[TARGET_DIR]` and save as `[SNAPSHOT_1]`
4. do something to the folder (e.g., add some files or modify its content)
5. run `fscmp.exe --snapshot [TARGET_DIR] [SNAPSHOT_2]` to make a snapshot of `[TARGET_DIR]` again and save as `[SNAPSHOT_2]`
6. run `fscmp.exe --diff [SNAPSHOT_1] [SNAPSHOT_2]` to see the diff carried by step 4


### 💡 Possible Use Cases

| Senario | Dir to Track |
| :- | :- |
| see added files after installing a software | "C:\Program Files"<br>"C:\Program Files (x86)"<br>"C:\Users\USERNAME\AppData\Roaming"<br>"C:\ProgramData" |
| see changed components after getting Windows service update | "C:\Windows\WinSxS" |


### 🐞 Known Issue

C++ lib `<filesystem>` seems to have decoding issue when it meets some specific characters (espicial Chinese) in Windows filesystem:

> exception: No mapping for the unicode character exists in the target multi-byte code page.


