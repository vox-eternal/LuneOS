<img width="941" height="478" alt="image" src="https://github.com/user-attachments/assets/d89f85f4-73f3-4866-bc2f-4b766d8d5996" />


# LuneOS

**An awesome, minimalistic, low-level bootloader/kernel for x86-64 systems, crafted entirely in Assembly by an Engineer, not a programmer.**  

[![Language: Assembly](https://img.shields.io/badge/Language-Assembly-blueviolet)](https://en.wikipedia.org/wiki/Assembly_language)  
[![Status: Early Prototype](https://img.shields.io/badge/Status-Early_Prototype-red)](https://github.com/lun3ux/LuneOS)  
[![Architecture: x86 (16-bit/32-bit)](https://img.shields.io/badge/Arch-x86-informational)]()  

---

## 📝 Project Manifest

LuneOS is an educational, experimental kernel designed to explore **low-level x86 system programming** from scratch. By writing the kernel entirely in **Assembly**, the project exposes the inner workings of the CPU, bootloader processes, memory management, and hardware interaction.  

* **Objective:** Gain deep understanding of system architecture and build a foundation for a self-hosting OS.  
* **Started:** December 8, 2025  
* **Current Status:** Early prototype, primarily focused on boot sequence and basic kernel routines.  
* **Next Milestone:** Implement a minimal text editor with C compilation capabilities.  

---

## ⚙️ Implemented Features

| Component | Functionality | Status |
| :--- | :--- | :--- |
| **Kernel Entry Point** | Initialize segmentation, stack, and basic hardware interfaces. | ✅ Complete |
| **VGA Text Output** | Low-level character output via BIOS interrupts and direct memory writes. | ✅ Complete |
| **Disk Reading** | Basic disk sector read routines for future filesystem support. | ✅ Complete |
| **C Integration** | Stage2 can execute C code compiled with Watcom for prototyping kernel features. | ✅ Complete |
| **FAT12 Filesystem** | Stage 2 is able to read from FAT12 files for future compatability with other projects. | ✅ Complete |  
| **Bootloader Stage 1 & 2** | Transition from 16-bit Real Mode to 32-bit Protected Mode. | ⚙️ Planned |
| **Switch Cross Compliler** | Switching from WatcomC to GCC for accsess to more libraries. | ⚙️ Planned |


---

## 💾 Core Specifications

* **Goal:** Build a fully functional, self-hosting kernel capable of running and compiling C code.  
* **Target Architecture:** x86 (16-bit/32-bit)  
* **Memory Model:** Segmented memory with flat VGA text-mode output.  
* **License:** Closed Source / Private (personal project)  

---

## 🚀 Future Plans

* Implement a **minimal text editor** that can edit and compile C files inside the kernel environment.  
* Develop a **simple filesystem** for reading and writing files from disk.  
* Add **keyboard input** and basic shell functionality.  
* Explore **protected mode multitasking** and basic process scheduling.  

---

## 📂 Repository Layout

```
LuneOS/
├─ src/
│  ├─ bootloader/
│  │  ├─ stage1/
│  │  └─ stage2/
│  ├─ kernel/
│  └─ include/
├─ build/
├─ docs/
└─ README.md
```

- **bootloader/** – Stages 1 and 2 for boot sequence  
- **kernel/** – Low-level kernel source (ASM + C)  
- **include/** – Header files for ASM/C integration  
- **build/** – Compiled binaries  
- **docs/** – Project documentation  

---

## 🔧 Build Instructions

```bash
# Clean and build all stages
./buildall.sh

# Run in QEMU (for testing)
qemu-system-i386 -fda build/stage1.bin

# Debug
./debug.sh
```

> ⚠️ Requires NASM, Watcom C, and brochs.  

---

## 💡 Notes

- The project is **entirely educational**, designed to explore CPU, memory, and hardware interaction.  
- Coding in Assembly is intentional to **understand the hardware without abstractions**.  
- Future stages may integrate minimal C code for kernel routines while keeping core control in Assembly.  
```
