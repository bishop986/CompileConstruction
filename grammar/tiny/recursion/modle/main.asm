; Author: bishop986 @copyright 
; Introduction:
;	code modle

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
	mov ecx, eax
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
	xor rcx, rcx
	xor rdx, rdx

	mov eax, 4
	mov ebx, 1
	mov ecx, char
	mov edx, 1
	int 80h
	ret
_read:
	push rax
	; read from stdin
	mov eax, 3
	mov ebx, 0
	mov ecx, buffer
	mov edx, bufferlen
	int 80h
	mov byte [buffer + 12], 0
	
	;compute the length of str, save it to edx
	xor edx, edx
	mov edx, 0
.begin1:
	mov al, [buffer+edx]
	cmp al, 0ah
	je	.end1
	cmp al, 0
	je	.end1
	mov bl, '-'
	cmp al, bl
	je .right
	cmp al, '0'
	jb  _start.errorread
	cmp al, '9'
	ja  _start.errorread
.right:
	inc dl
	jmp .begin1
.end1:							; finish get the size of str
	xor rax, rax
	xor ebx, ebx
	xor ecx, ecx
	mov qword [num], rax
	mov ecx, 1
.atoi:							; convert str to number, save to memory num
	mov rax, 1
	xor ebx, ebx
	cmp edx, 0
	je .end2
	mov bl, [buffer+edx-1]
	cmp bl, "-"
	je  .end2neg

	xor rax, rax
	sub bl, 48					; transform ascii to number
	mov eax, ebx				; send data of ebx to eax wait for multi
	push rdx
	mul ecx						; a*100+b*10+c = abc
	add qword [num], rax
	pop rdx
	
	xor rax, rax				; iteration multi 10
	mov eax, ecx
	mov ebx, 10
	push rdx
	mul ebx
	pop rdx
	mov ecx, eax
	sub edx, 1
	jmp .atoi
.end2neg:
	cmp edx, 1					; judge if it's the first symbol
	jne _start.errorread
	mov rax, -1
.end2:
	mul qword [num]
	pop rcx
	mov qword [rcx], rax
	ret

_write:
	xor rcx, rcx
	mov rcx, 1
	cmp rax, 0
	jg  .otia
	mov rbx, -1
	mul rbx
	push rax
	push rcx
	mov byte [char], '-'
	call _putc
	pop rcx
	pop rax
.otia:
	xor rbx, rbx
	xor rdx, rdx
	mov rbx, 10	
	div rbx
	add rdx, 48
	mov byte [char], dl
	push rdx
	cmp rax, 0
	je  .end3
	inc rcx
	jmp .otia
.end3:
.output:
	pop rax
	mov byte [char], al
	push rcx
	call _putc
	pop rcx
	dec rcx
	cmp rcx, 0
	jne .output
	mov byte [char], 0ah
	call _putc
	ret

_eqfunc:
	sub rax, rbx
	cmp rax, 0
	je  .isequ
	mov rax, 0
	ret
.isequ:
	mov rax, 1
	ret

_lefunc:
	sub rbx, rax
	cmp rbx, 0
	jg  .isle
	mov rax, 0
	ret
.isle:
	mov rax, 1
	ret

section .bss
	char: resb 1
	delim: resb 1
	num: resb 8
	bufferlen: equ 1024
	buffer: resb bufferlen

