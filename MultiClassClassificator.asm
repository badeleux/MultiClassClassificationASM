section	.text
extern	_GLOBAL_OFFSET_TABLE_	; zewnętrzny, uzupełniony przez linker
global	trainThetaMatrix:function
; makro do pobierania adresu GOT; wynik w EBX.
%imacro	wez_GOT	0

	call	%%zaraz
	%%zaraz:
	pop	ebx
	add	ebx, _GLOBAL_OFFSET_TABLE_ + $$ - %%zaraz wrt ..gotpc
%endmacro

;makro do liczenia potegi pierwszy parametr podstawa wykladnik to ecx drugi parametr to gdzie zapisac
%macro power 2	
	finit ;inicjalizacja

	cmp ecx, 0
	je koniec_przy_zerze
			jnl dodatni


	neg ecx ;jesli wykladnik jest ujemny zmien znak
	dec ecx
	FLD1
	FDIV dword [%1]
	cmp ecx, 0
	je koniec
	FLD1
	FDIV dword [%1]
	jmp petla
dodatni:
	dec ecx
	FLD dword [%1]
	cmp ecx, 0
	je koniec
	FLD dword [%1]
petla:
	FMUL st0, st1
		loop petla
	jmp koniec
koniec_przy_zerze:
	mov [%2], dword 1
koniec:
	fstp dword [%2]                                                                                                                                                                                                                                          
%endmacro

;makro rezerwujace pamiec w pierwszym parametrze przekazujemy ile elementow tablicy dword chcemy
;drugi parametr to miejsce gdzie zapisac adres do poczatku tablicy
%macro zarezerwujPamiec 2
	lea eax, [esp-4]
	mov [%2], eax
	mov eax, %1
	mov ebx, 4
	mul ebx
	neg eax
	lea esp, [esp + eax]
%endmacro

;funkcja do liczenia funkcji sigmoid 
;pierwszy parametr to adres pierwszego elementu tablicy w ecx liczba elementow [esp + 8]
;drugi parametr to adres do pierwszego eleemntu nowej macierzy [esp+12]
;trzeci par to adres do liczby eulera [esp+16]
;czwarty par to adres do tempa [esp + 20]
sigmoidfunction:
	push ebp
	mov ebp, esp

	;lea [%2], [esp-4]
	petla:
		push ecx ;schowanie ecx
	;	mov ecx, [%1]
		NEG ecx
	;	power [%3], [%4] ; w 4 mamy policzone e^-z(?)
		pop ecx ;odzyskanie ecx
	;	mov eax, [%4];tutaj wsadzamy element do tablicy
	loop petla
	
	mov esp, ebp
	pop ebp
	
	ret

;pierwszy parametr to wskaznik na tablice X [esp + 8]
;drugi parametr to liczba wierszy w tablicy X [esp + 12]
;trzeci parametr to liczba kolumn w tablicy X [esp + 16]
;czwarty parametr to wskaznik na tablice Y [esp + 20]
;piaty parametr to liczba elementow wektora Y [esp + 24]
trainThetaMatrix:
	;prolog
	push ebp
	mov ebp, esp
	

	wez_GOT
	lea ebx, [ebx + EULER_NUMBER wrt ..gotoff]
	lea edx, [ebx + temp wrt ..gotoff]

	mov eax, [esp+12]
	mul dword [esp + 16]
	zarezerwujPamiec eax, edx 

;epilog:
	mov esp, ebp
	pop ebp
	xor	eax, eax	; funkcja zwraca 0 jako brak błędu
	ret

section .data
EULER_NUMBER dd 0x402DF854
numberTwo db 2
temp dd 0
