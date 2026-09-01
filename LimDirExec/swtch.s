# void swtch(struct context* old, struct context* new);
# Save current context in old and load context from new


.globl swtch
swtch:
  # Save old regs
  movl 4(%esp), %eax #[arg_new arg_old (%esp)->ret_add] so.. save arg_old in eax
  popl 0(%eax)       # save ret_add into struct context old
  movl %esp, 4(%eax)
  movl %ebx, 8(%eax)
  movl %ecx, 12(%eax)
  movl %edx, 16(%eax)
  movl %esi, 20(%eax)
  movl %edi, 24(%eax)
  movl %ebp, 28(%eax)

  # so.. eax --> struct context kind of vector
  # [ret_add, esp, ebx, .., ebp]

  movl 4(%esp), %eax #[arg_new, (%esp)->arg_old]

  movl 8(%eax), %ebx
  movl 12(%eax), %ecx
  movl 16(%eax), %edx
  movl 20(%eax), %esi
  movl 24(%eax), %edi
  movl 28(%eax), %ebp
  movl 4(%eax), %esp # new_ptr == eax --> [ret_add, esp, ebx, .., ebp]
  pushl 0(%eax)      # putting in top stack the ret_add of new context process so going there
  ret                # returning like switching

