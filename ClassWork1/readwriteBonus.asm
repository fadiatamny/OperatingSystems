section .text

global _start

_start:
	
	mov edi, 0
	mov esi, data
	mov edx, 256
	mov eax, 0
	syscall

	mov edi, 1
	mov esi, data
	mov edx, eax   
	mov eax, 1
	syscall

	mov edi, 0
	mov eax, 0x3c
	syscall

section .data

data db 256, 0
