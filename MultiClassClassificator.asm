section	.text
extern	_GLOBAL_OFFSET_TABLE_	; zewnętrzny, uzupełniony przez linker
global	info:function		; eksportowana funkcja
;extern	printf			; funkcja zewnętrzna

; makro do pobierania adresu GOT; wynik w EBX.
%imacro	wez_GOT	0

	call	%%zaraz
	%%zaraz:
	pop	ebx
	add	ebx, _GLOBAL_OFFSET_TABLE_ + $$ - %%zaraz wrt ..gotpc
%endmacro

info:
	push ebp
	mov ebp, esp

	mov edx, [ebp + 8]
	mov eax, [edx]
	wez_GOT			; pobieramy adres GOT
	push	ebx		; zachowujemy EBX

	mov	eax, 4		; funkcja pisania do pliku
			; do ECX załaduj ADRES napisu (stad LEA a nie MOV)
	lea	ecx, [ebx + napis wrt ..gotoff]
	mov	ebx, 1		; plik = 1 = standardowe wyjście (ekran)
	mov	edx, napis_dl	; długość napisu
	int	80h		; wyświetl

; a tak uruchamiamy funkcje zewnętrzne:
	pop	ebx		; przywracamy EBX
	lea	ecx, [ebx + napis wrt ..gotoff]	; ECX = adres napisu
	push	ecx		; adres na stos (jak dla funkcji z C)
;	call	printf wrt ..plt	; uruchomienie funkcji
	add	esp, 4		; usunięcie argumentów ze stosu

				; przywracanie rejestrów
	pop	edx
	pop	ecx
	pop	ebx
	pop	eax

	xor	eax, eax	; funkcja zwraca 0 jako brak błędu
	ret
section .data

napis		db "jakas funkcja z dynamicznej biblioteki", 10, 0
napis_dl	equ	$ - napis
