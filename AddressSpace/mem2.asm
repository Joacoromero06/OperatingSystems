.file   "mem2.c"              # Nombre del archivo fuente
.text                           # Sección de código ejecutable

.section .rodata               # Read-Only DATA: datos constantes de solo lectura
.LC0:                           # Local Constant 0: etiqueta local generada por GCC
.string "realloc array error"   # Literal almacenado en .rodata

.text
.globl main                     # GLOBAL: hace visible 'main' al linker
.type main, @function           # Declara que 'main' es una función

main:
.LFB6:                          # Local Function Begin: comienzo interno de main
.cfi_startproc                  # Call Frame Information: comienza metadata para unwinding/debugging

    pushq %rbp                  # Guarda el frame pointer anterior
    .cfi_def_cfa_offset 16      # CFI: CFA (Canonical Frame Address) está a RSP+16
    .cfi_offset 6, -16          # CFI: registro 6 = RBP; guardado a -16 respecto del CFA

    movq %rsp, %rbp             # RBP = RSP
                                # Se establece el stack frame de main

    .cfi_def_cfa_register 6     # CFI: ahora el CFA se expresa respecto de RBP

    subq $16, %rsp              # Reserva 16 bytes en el stack para variables locales

    movl $1, %esi               # RSI = 1
    movl $32, %edi              # RDI = 32
    call my_malloc_array         # my_malloc_array(32, 1)

    movq %rax, -8(%rbp)         # guarda el retorno (puntero) en main.p
                                # -8(%rbp) == p

    cmpq $0, -8(%rbp)            # compara p con NULL
    jne .L2                      # si p != NULL, salta a .L2

    leaq .LC0(%rip), %rax       # LEA = Load Effective Address
                                #
                                # RAX = dirección de .LC0
                                #
                                # .LC0(%rip) significa:
                                # dirección = RIP + desplazamiento hacia .LC0
                                #
                                # NO carga el contenido de la string.
                                # Carga SU DIRECCIÓN.

    movq %rax, %rsi              # RSI = "realloc array error"
                                # conceptualmente: RSI = &.LC0

    movl $1, %edi               # EDI = EXIT_FAILURE

    movl $0, %eax               # requisito de llamada variádica (ABI)

    call err@PLT                # llama a err()

.L2:
    movq -8(%rbp), %rax         # RAX = p
    movq %rax, %rdi             # primer argumento = p
    call ex1                    # ex1(p)

    call seg_fault1             # seg_fault1()

    movq -8(%rbp), %rax         # RAX = p
    movq %rax, %rdi             # RDI = p
    call puts@PLT               # puts(p)

    movl $1, %esi               # segundo argumento = 1
    movl $32, %edi              # primer argumento = 32
    call my_malloc_array         # malloc 32 bytes

    movq %rax, %rdx             # RDX = nuevo buffer

    movq -8(%rbp), %rax         # RAX = p
    movq %rax, %rsi             # RSI = p

    movq %rdx, %rdi             # RDI = nuevo buffer

    call ex2                    # ex2(nuevo_buffer, p)

    movq -8(%rbp), %rax         # RAX = p
    movq %rax, %rsi             # RSI = p
    movl $0, %edi               # RDI = NULL
    call ex2                    # ex2(NULL, p)
                                # => UB: memcpy(NULL, ...)

    movl $0, %eax               # return 0
    leave                        # equivalente conceptual:
                                # mov rsp, rbp
                                # pop rbp

    .cfi_def_cfa 7, 8           # CFI: CFA vuelve a depender de RSP
    ret                          # retorna a quien llamó main

.cfi_endproc
.LFE6:                          # Local Function End
.size main, .-main              # tamaño de main


# ============================================================
# my_malloc_array
# ============================================================

.type my_malloc_array, @function

my_malloc_array:
.LFB7:
.cfi_startproc

    pushq %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16

    movq %rsp, %rbp
    .cfi_def_cfa_register 6

    subq $16, %rsp               # espacio local

    movq %rdi, -8(%rbp)          # guarda n
    movq %rsi, -16(%rbp)         # guarda size

    movq -16(%rbp), %rdx         # RDX = size
    movq -8(%rbp), %rax          # RAX = n

    movq %rax, %rsi              # RSI = n
    movl $0, %edi                # RDI = NULL

    call reallocarray@PLT        # reallocarray(NULL, n, size)
                                 # equivalente práctico a malloc(n*size)

    leave
    .cfi_def_cfa 7, 8
    ret

.cfi_endproc
.LFE7:
.size my_malloc_array, .-my_malloc_array


# ============================================================
# seg_fault1
# ============================================================

.section .rodata
.LC1:
.string "hello"

.text
.type seg_fault1, @function

seg_fault1:
.LFB8:
.cfi_startproc

    pushq %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16

    movq %rsp, %rbp
    .cfi_def_cfa_register 6

    subq $16, %rsp                # reserva espacio local

    leaq .LC1(%rip), %rax        # RAX = dirección de "hello"

    movq %rax, -16(%rbp)         # src = &"hello"

    movq -16(%rbp), %rdx         # RDX = src
    movq -8(%rbp), %rax          # RAX = dest
                                  #
                                  # ¡¡¡dest NO FUE INICIALIZADO!!!

    movq %rdx, %rsi              # RSI = src
    movq %rax, %rdi              # RDI = dest

    call strcpy@PLT              # strcpy(dest, src)

    movq -8(%rbp), %rax          # RAX = dest
    movq %rax, %rdi              # RDI = dest
    call puts@PLT                # puts(dest)

    nop                           # no operation

    leave
    .cfi_def_cfa 7, 8
    ret

.cfi_endproc
.LFE8:
.size seg_fault1, .-seg_fault1


# ============================================================
# ex1
# ============================================================

.section .rodata

.LC2:
.string "hola puedo poner 32.\n"
.string ""

.LC3:
.string "Me salgo de 32.\n"

.text
.type ex1, @function

ex1:
.LFB9:
.cfi_startproc

    pushq %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16

    movq %rsp, %rbp
    .cfi_def_cfa_register 6

    subq $16, %rsp                # reserva stack frame

    movq %rdi, -8(%rbp)           # guarda argumento p
                                  # ex1: -8(%rbp) == p

    leaq .LC2(%rip), %rcx         # RCX = dirección de string

    movq -8(%rbp), %rax           # RAX = p

    movl $32, %edx                # size = 32
    movq %rcx, %rsi               # RSI = src
    movq %rax, %rdi               # RDI = p

    call strlcpy@PLT              # strlcpy(p, src, 32)

    leaq .LC3(%rip), %rcx         # RCX = dirección de segunda string

    movq -8(%rbp), %rax           # RAX = p

    movl $32, %edx                # size = 32
    movq %rcx, %rsi               # RSI = src
    movq %rax, %rdi               # RDI = p

    call strlcat@PLT              # strlcat(p, src, 32)

    movq -8(%rbp), %rax           # RAX = p
    movq %rax, %rdi               # RDI = p

    call puts@PLT                 # puts(p)

    nop

    leave
    .cfi_def_cfa 7, 8
    ret

.cfi_endproc
.LFE9:
.size ex1, .-ex1


# ============================================================
# ex2
# ============================================================

.globl ex2
.type ex2, @function

ex2:
.LFB10:
.cfi_startproc

    pushq %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16

    movq %rsp, %rbp
    .cfi_def_cfa_register 6

    subq $16, %rsp

    movq %rdi, -8(%rbp)           # guarda dst
    movq %rsi, -16(%rbp)          # guarda src

    movq -16(%rbp), %rax          # RAX = src
    movq %rax, %rdi               # RDI = src
    call strlen@PLT               # strlen(src)

    leaq 1(%rax), %rdx            # RDX = strlen(src) + 1
                                  # incluye '\0'

    movq -16(%rbp), %rcx          # RCX = src
    movq -8(%rbp), %rax           # RAX = dst

    movq %rcx, %rsi               # RSI = src
    movq %rax, %rdi               # RDI = dst

    call memcpy@PLT               # memcpy(dst, src, strlen(src)+1)

    movq -8(%rbp), %rax           # RAX = dst
    movq %rax, %rdi               # RDI = dst

    call puts@PLT                 # puts(dst)

    nop

    leave
    .cfi_def_cfa 7, 8
    ret

.cfi_endproc
.LFE10:
.size ex2, .-ex2

.ident "GCC: (GNU 16.2.1 20260810)"

.section .note.GNU-stack,"",@progbits
