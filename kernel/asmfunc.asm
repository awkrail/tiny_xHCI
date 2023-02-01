; asmfunc.asm
; System V AMD64 Calling Convertion
; Registers: RDI, RSI, RDX, RCX, R8, R9

bits 64
section .text

global IoOut32
IoOut32:
  mov dx, di
  mov eax, esi
  out dx, eax
  ret

global IoIn32
IoIn32:
  mov dx, di
  in eax, dx
  ret
