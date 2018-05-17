%include "./assist.inc"
; Author: bishop986 @copyright 
; Introduction:
;	to try some my function

section .data
	errorMsg1: db "[ERROR] Data Format Error, Exit process", 0ah, 0
	msgLen1: equ $-errorMsg1
section .text

global _start

_start:
	nop
	
	nop
	call _exit

.errorread:
	xor rax, rax
	xor rbx, rbx
	xor rcx, rcx
	xor rdx, rdx
	mov eax, 4
	mov ebx, 1
	mov ecx, errorMsg1
	mov edx, msgLen1
	int 80h
	call _exit

_exit:
	mov eax, 1
	mov ebx, 0
	int 80h

_putc:
	xor rax, rax
	xor rbx, rbx
	mov eax, 4
	mov ebx, 1
	mov ecx, char
	mov edx, 1
	int 80h
	ret

section .bss
	char: resb 1
	delim: resb 1
	x: resb 8
	y: resb 8
	num: resb 8
	bufferlen: equ 1024
	buffer: resb bufferlen

