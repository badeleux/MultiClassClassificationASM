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
;pierwszy parametr to adres do podstawa [esp + 8]
;drugi par to adres do zmiennej [esp + 12]
powerFunction:	
	push ebp
	mov ebp, esp
	push ecx
	push eax
	push ebx
	finit ;inicjalizacja

	mov eax, [esp +20] 
	mov ebx, [esp + 24]


	cmp ecx, 0
	je koniec_przy_zerze
	jnl dodatni

	
	neg ecx ;jesli wykladnik jest ujemny zmien znak
	dec ecx
	FLD1
	FDIV dword [eax]
	cmp ecx, 0
	je koniec
	FLD1
	FDIV dword [eax]
	jmp petla
dodatni:
	dec ecx
	FLD dword [eax]
	cmp ecx, 0
	je koniec
	FLD dword [eax]
petla:
	FMUL st0, st1
		loop petla
	jmp koniec
koniec_przy_zerze:
	fld1 
koniec:
	fstp dword [ebx]  
	
;;;;;;;;;;;epilog:
	pop ebx
	pop eax
	pop ecx     
	mov esp, ebp
	pop ebp                    
	ret                                                                                                                                                                                                               

;makro rezerwujace pamiec w pierwszym parametrze przekazujemy ile elementow tablicy dword chcemy
;drugi parametr to miejsce gdzie zapisac adres do tablicy
%macro zarezerwujPamiec 1
	mov eax, %1
	mov ebx, 4
	mul ebx
	neg eax
	add esp, eax
%endmacro

;funkcja do liczenia funkcji sigmoid 
;pierwszy parametr to tablica z (w ecx liczba elementow) [esp + 8]
;drugi parametr to adres do nowej macierzy [esp+12]
;trzeci par to adres do liczby eulera [esp+16]
sigmoidfunction:
;;;;;;;;;;;;;;;epilog:
	push ebp
	mov ebp, esp
	push eax
	push ebx
	push ecx
	push edx
;;;;;;;;;;;;;;;;;
	mov esi,0
	petla1:
		push ecx
		mov ebx, [esp + 28] ;w ebx adres do macierzy X
		finit
		fld dword [ebx + esi] ; w ecx element tablicy
		fchs ; -z
		fist dword [ebx + esi]
		mov ecx, [ebx + esi]
		mov edx, [esp + 36] ; w edx adres do e
		mov ebx, [esp + 32] ; ebx adres do drugiej tablicy
		lea eax, [ebx + esi]; w eax adres do elementu nowej tablicy
		
;funkcja potegowa:
		push eax		
		push edx
		call powerFunction ; mamy policzone e^-z(?)
		pop edx
		pop eax

		finit
		fld dword [eax] ;ladujemy e^-z
		fld1 ; ladujemy stala 1
		fadd st0, st1 ; 1+ e^-z
		fld1 ; ladujemy stala 1
		fdiv st0, st1 ; 1/(1-e^-z)
		fstp dword  [eax]
		
		pop ecx

		add esi, 4
	loop petla1
	

	pop edx
	pop ecx
	pop ebx
	pop eax

	mov esp, ebp
	pop ebp
	
	ret

;funkcja transponujaca macierz
;ebp+8 adres do pierwszego elementu tablicy gdzie zapisac nowa macierz
;ebp+12 adres do pierwszego element tablicy do transponowania
;ebp+16 liczba wierszy
;ebp+20 liczba kolumn
transposeMatrix:
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi
	push ecx
	push eax
	push edx
	
	mov eax, dword[ebp+20]

	mov ebx, 4
	mul ebx ; liczba kolumn w bajtach
	mov edx, dword[ebp+20]
	mov ecx, dword[ebp + 16]; w ecx liczba wierszy
	mov esi, dword[ebp + 12] ;adres do pierwszy element starej macierzy
	mov edi, dword[ebp + 8]; adres do pierwszy element nowej macierzy
	xor ebx, ebx
	
	copyColumn:
		cld
		push eax
		mov eax, [esi]
		mov [edi], eax
		pop eax		
		add esi, eax 
		add edi, 4
		loop copyColumn	
;go to next column
		dec edx ;w edx jest licznik ile jeszcze pozostalo kolumn
		je return ;jestli 0 to koncz
		inc ebx ;ebx licznik w druga strone o ile przesunac esi
		push eax ; schowac eax
		mov eax, ebx
		mov ecx, 4
		push edx ; trzeba schowac wartosc z edx, gdyz wynik mnozenia zapisywany jest w EDX:EAX
		mul ecx
		pop edx
		mov esi, [ebp + 12] 
		add esi, eax
		mov ecx, [esi]
		pop eax ; odzyskaj eax

		mov ecx, [ebp+16]
		
		jmp copyColumn
		
	return:
		pop edx
		pop eax
		pop ecx
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		ret

;pierwszy parametr to wskaznik na tablice X [esp + 8]
;drugi parametr to liczba wierszy w tablicy X [esp + 12]
;trzeci parametr to liczba kolumn w tablicy X [esp + 16]
;czwarty parametr to wskaznik na tablice Y [esp + 20]
;piaty parametr to liczba elementow wektora Y [esp + 24]
;szosty par. to lambda [esp + 28]
;siodmy par. to wskaznik do allTheta (wyjsciowa tablica) [esp + 32]
trainThetaMatrix:
	;prolog
	push ebp
	mov ebp, esp
		
	wez_GOT ;pobranie adresu got do ebx
	
	mov eax, [esp+12]
	mul dword [esp + 16]
	mov ecx, eax
	zarezerwujPamiec eax  
	mov eax, esp
	
	push dword[ebp + 16]
	push dword[ebp + 12]
	push dword[ebp + 8]
	push eax
	call transposeMatrix
	mov ebx, [eax + 4]
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	wez_GOT	
	mov ecx, [ebp + 24]
	lea edx, [ebx + EULER_NUMBER wrt ..gotoff]

	petla_liczba_klas: ;lecimy od 10->0 
		push ecx
		mov ecx, [ebp + 12] ; do ecx laduje liczba elementow Y 
		petla_Y:
			push ecx
			mov ecx, dword 50 ; liczba iteracji ustawiona na sztywno
			petla_iteracje:
				push ecx

				pop ecx
			loop petla_iteracje
			pop ecx
		loop petla_Y	
		pop ecx
	loop petla_liczba_klas
	
;	push dword edx
;	push dword eax
;	push dword eax
;	call sigmoidfunction
;	mov eax, [eax]

;epilog:
	mov esp, ebp
	pop ebp
	xor	eax, eax	; funkcja zwraca 0 jako brak błędu
	ret

section .data
	EULER_NUMBER dd 0x402DF854

	
