section .bss
buffer resb 32

section .data
minus_sign db '-'

section .text
global _start

print_int:
    cmp rax, 0
    jge .positive
    mov rdx, 1
    mov rsi, minus_sign
    mov rdi, 1
    mov rax, 1
    syscall
    neg rax
.positive:
    mov rcx, buffer
    add rcx, 31
    mov byte [rcx], 10
    dec rcx
    mov rbx, 10
.convert:
    xor rdx, rdx
    div rbx
    add dl, '0'
    mov [rcx], dl
    dec rcx
    test rax, rax
    jnz .convert
    inc rcx
    mov rax, 1
    mov rdi, 1
    mov rsi, rcx
    mov rdx, buffer
    add rdx, 32
    sub rdx, rcx
    syscall
    ret

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 1024
    mov rax, 100
    push rax
    mov rax, 17
    pop rbx
    sub rbx, rax
    mov rax, rbx
    push rax
    mov rax, 3
    pop rbx
    add rax, rbx
    mov [rbp-8], rax
    mov rax, [rbp-8]
    push rax
    mov rax, 10
    pop rbx
    mov rdx, 0
    mov rcx, rax
    mov rax, rbx
    idiv rcx
    push rax
    mov rax, 299
    pop rbx
    add rax, rbx
    mov [rbp-16], rax
    mov rax, [rbp-8]
    call print_int
    mov rax, [rbp-16]
    call print_int
    mov rax, 60
    xor rdi, rdi
    syscall
