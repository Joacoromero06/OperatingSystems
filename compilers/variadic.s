	.file	"variadic.c"
	.text
	.globl	sum
	.type	sum, @function
sum:
.LFB12:
	.cfi_startproc
	subq	$88, %rsp
	.cfi_def_cfa_offset 96
	movq	%rsi, 40(%rsp)
	movq	%rdx, 48(%rsp)
	movq	%rcx, 56(%rsp)
	movq	%r8, 64(%rsp)
	movq	%r9, 72(%rsp)
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	movl	$8, (%rsp)
	leaq	96(%rsp), %rax
	movq	%rax, 8(%rsp)
	leaq	32(%rsp), %rax
	movq	%rax, 16(%rsp)
	testl	%edi, %edi
	jle	.L7
	movq	%rax, %r8
	movl	$0, %ecx
	movl	$0, %esi
	jmp	.L5
	.p2align 5
.L3:
	movq	8(%rsp), %rdx
	leaq	8(%rdx), %rax
	movq	%rax, 8(%rsp)
.L4:
	addl	(%rdx), %esi
	addl	$1, %ecx
	cmpl	%ecx, %edi
	je	.L1
.L5:
	movl	(%rsp), %eax
	cmpl	$47, %eax
	ja	.L3
	movl	%eax, %edx
	addq	%r8, %rdx
	addl	$8, %eax
	movl	%eax, (%rsp)
	jmp	.L4
.L7:
	movl	$0, %esi
.L1:
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L10
	movl	%esi, %eax
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L10:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE12:
	.size	sum, .-sum
	.globl	acum
	.type	acum, @function
acum:
.LFB13:
	.cfi_startproc
	subq	$88, %rsp
	.cfi_def_cfa_offset 96
	movq	%rsi, 40(%rsp)
	movq	%rdx, 48(%rsp)
	movq	%rcx, 56(%rsp)
	movq	%r8, 64(%rsp)
	movq	%r9, 72(%rsp)
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	movl	$8, (%rsp)
	leaq	96(%rsp), %rax
	movq	%rax, 8(%rsp)
	leaq	32(%rsp), %rcx
	movq	%rcx, 16(%rsp)
	jmp	.L12
.L16:
	cmpl	$47, %eax
	ja	.L13
	movl	%eax, %eax
	addq	%rcx, %rax
	addl	$16, %edx
	movl	%edx, (%rsp)
.L14:
	addl	(%rax), %edi
.L12:
	movl	(%rsp), %edx
	cmpl	$47, %edx
	ja	.L15
	leal	8(%rdx), %eax
	movl	%eax, (%rsp)
	movl	%edx, %esi
	cmpl	$0, (%rcx,%rsi)
	jne	.L16
.L17:
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L20
	movl	%edi, %eax
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L15:
	.cfi_restore_state
	movq	8(%rsp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, 8(%rsp)
	cmpl	$0, (%rax)
	je	.L17
.L13:
	movq	8(%rsp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, 8(%rsp)
	jmp	.L14
.L20:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE13:
	.size	acum, .-acum
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB11:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	$3, %ecx
	movl	$2, %edx
	movl	$1, %esi
	movl	$3, %edi
	movl	$0, %eax
	call	sum
	movl	%eax, %r15d
	movl	$3, %ecx
	movl	$2, %edx
	movl	$1, %esi
	movl	$0, %edi
	movl	$0, %eax
	call	acum
	movl	%eax, %r14d
	leaq	.LC0(%rip), %rbx
	movl	%r15d, %esi
	movq	%rbx, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	%r14d, %esi
	movq	%rbx, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE11:
	.size	main, .-main
	.ident	"GCC: (GNU) 16.2.1 20260810"
	.section	.note.GNU-stack,"",@progbits
