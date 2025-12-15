bits 16

section _ENTRY class=CODE

extern _cstart_
global entry



entry:
    cli
    ; setup stack
    mov ax, ds
    mov ss, ax
    mov sp, 0
    mov bp, sp
    sti

    ; expect boot drive in dl, send it as argument to cstart function
    xor dh, dh
    push dx
    call _cstart_
    pop dx

    call checkKeyStroke

    cli
    hlt

checkKeyStroke:
    push bx
    push ax
    ; wait for a key to be available
    mov ah, 01h
    int 16h
    cmp al, 01h
    je checkKeyStroke      
                    ; loop when no key is available
    .loop:
                                            ; read the key
    mov ah, 00h                            ; function 00h - read keystroke
    int 16h     
    mov bh, 0
    mov ah, 0x0E                            ; teletype output
    int 0x10
    cmp al, 0x0D       ; Enter key?
    je .enter
    jmp .loop


    .enter:
    ; Carriage Return
    mov al, 0x0D
    mov ah, 0x0E
    int 10h

    ; Line Feed
    mov al, 0x0A
    mov ah, 0x0E
    int 10h

    jmp .loop


    .done:
    pop ax
    pop bx
    ret