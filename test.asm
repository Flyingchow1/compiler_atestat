global _start

section .text
_start:
    mov rax, 60     ; sys_exit
    mov rdi, 2      ; exit code 2
    syscall
