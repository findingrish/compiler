global main
extern printf
extern scanf

SECTION .bss

	b5b567  resb  8
	c3bd  resb  8
	d3  resb  8

SECTION .text
main:

	mov rax,11
	mov [b5b567],rax

	xor rax,rax
	mov rax,0
	mov [d3],rax

	xor rax,rax

; starting label of while 0
label0:
	mov rax,[b5b567]
	mov rbx,5
	cmp rax,rbx
	jl label2

; starting label of while body 1
label1:

	mov rsi,c3bd
	mov rdi,msg2
	sub rsp,8
	call scanf
	add rsp,8

	mov rax,[c3bd]
	mov rbx,rax
	push rbx
	mov rax,[d3]
	pop rbx

; arith op, root left right TK_PLUS d3 c3bd

	add rax,rbx

	mov [d3],rax

	xor rax,rax
	mov rax,1
	mov rbx,rax
	push rbx
	mov rax,[b5b567]
	pop rbx

; arith op, root left right TK_MINUS b5b567 1

	sub rax,rbx

	mov [b5b567],rax

	xor rax,rax
	jmp label0


; end of while loop 3
label2:

	mov rsi,[d3]
	mov rdi,msg1
	sub rsp,8
	call printf
	add rsp,8

ret

SECTION .data

	msg1 db '%d',10,0 
	msg2 db '%d',0 
	temp00 dd 0,0
	temp01 dd 0,0
	temp02 dd 0,0
	temp03 dd 0,0
	temp04 dd 0,0
	temp05 dd 0,0
	temp06 dd 0,0
	temp07 dd 0,0
	temp10 dd 0,0
	temp11 dd 0,0
	temp12 dd 0,0
	temp13 dd 0,0
	temp14 dd 0,0
	temp15 dd 0,0
	temp16 dd 0,0
	temp17 dd 0,0
