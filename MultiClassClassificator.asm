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

	mov eax, [esp +20] ; do eax adres do podstawy 
	mov ebx, [esp + 24] ; do ebx adres do zmiennej gdzie wrzucic wynik


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

;mnozenie Z = X * Y gdzie X jest macierza (n x m), a Y jest macierza (l x m)
;pierwszy par - wskaznik do tablicy X [ebp + 8] 
;drugi par - wskaznik do tablicy Y [ebp + 12]
;trzeci par - wskaznik do tablicy Z [ebp + 16]
;czwarty par - n - liczba wierszy X [ebp + 20]
;piaty par - m liczba kolumn X [ebp + 24 ]
;szosty par - l liczba wierszy Y [ebp + 28]
multiplyMatrices:
	push ebp
	mov ebp, esp
	push dword [ebp + 20] ; pamiec na licznik petli
	push eax
	push ebx
	push ecx
	push edx
;;;;;;;;;;;;;;;;;;;;
	mov esi, [ebp + 8]
	mov edi, [ebp + 12]

	mov ecx, [ebp + 24]
	mov ebx, ecx
	and ebx, 31 ;reszta z dzielenia przez 32 
	shr ecx, 5 ; ile mozemy wykonac petli pobierajac po 32 liczby
	
	push ebx
	push ecx
	
	lea esp, [esp-20] ; alokacja pamieci na 4 liczby float i wynik ich dodawania
	mov [esp+16], dword 0
	petlaMnozenia1:
		movups xmm0, [esi]
		movups xmm1, [edi]
		movups xmm2, [esi+16]
		movups xmm3, [edi+16]
		movups xmm4, [esi+32]
		movups xmm5, [edi+32]
		movups xmm6, [esi+48]
		movups xmm7, [edi+48]
		
		mulps xmm0, xmm1
		mulps xmm2, xmm3
		mulps xmm4, xmm5
		mulps xmm6, xmm7
		
		addps xmm0, xmm2
		addps xmm4, xmm6
		addps xmm0, xmm4		

		movups [esp], xmm0 ;wrzucono 4 liczby do pamieci, teraz je nalezy zsumowac
		movss xmm0, [esp]
		movss xmm1, [esp+4]
		movss xmm2, [esp+8]
		movss xmm3, [esp+12]
		movss xmm4, [esp+16]

		addss xmm0, xmm1
		addss xmm2, xmm3
		addss xmm0, xmm2
		addss xmm0, xmm4

		movss [esp+16], xmm0 ; pod esp+16 jest suma zsumowanym do tej pory komorek
	

		add esi, 64
		add edi, 64
		loop petlaMnozenia1

	mov ecx, ebx
	petlaMnozenia2:
		movss xmm0, [esi]
		movss xmm1, [edi]
		movss xmm2, [esp+16]
		mulss xmm0, xmm1
		addss xmm0,xmm2
		movss [esp+16], xmm0
		
		add esi, 4
		add edi, 4
		loop petlaMnozenia2
	next_rowY:
		mov eax, [ebp + 16]
		mov edx, [esp+16]
		mov [eax], edx ; zapisanie liczby do nowej tablicy
		add eax, 4
		mov [ebp + 16], eax ; zapisujemy wskaznik na kolejny element tablicy
		
		lea esp, [esp + 20] ; kasujemy zaalokowana tablice
		pop ecx
		pop ebx

		mov eax, [ebp + 28] ; do eax liczba wierszy macierzy Y
		dec eax
		je next_rowX ; jesli macierz Y mnozylo teraz ostatni wiersz to przejdz do nastepnego wiersza macierzy X

		mov esi, [ebp + 8]
		jmp petlaMnozenia1

	next_rowX:
		mov eax, [ebp-4] ; w eax ile jeszcze kolumn pozostalo
		dec eax
		mov [ebp-4], eax
		je koniecMnozenia

		mov edi, [ebp + 12] ; macierz Y zaczynamy od poczatku
		mov eax, [ebp + 24] ; liczba kolumn macierzy X
		mov edx, dword 4		
		mul edx
		mov edx, [ebp + 8] ; w eax znajduje sie wskaznik na kolejny wiersz
		add eax, edx
		mov esi, eax ; wskaznik na pierwszy element kolejnego wiersza wedruje do esi
		mov [ebp+8], esi ;zapisujemy wskaznik do pamieci
			


	koniecMnozenia:
;;;;;;;;;;;;;;;;;;;;
		pop edx
		pop ecx
		pop ebx
		pop eax
		lea esp, [esp + 4] ; dealokacja pamieci dla zmiennych lokalnych
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

	;przyklad rezerwacji pamieci	
	mov eax, [esp+12] ; do eax liczba wierszy
	mul dword [esp + 16] ; mnozymy razy liczbe kolumn
	mov ecx, eax ; do ecx liczba elementow do zaalokowania
	zarezerwujPamiec eax  
	mov eax, esp
	
	;przyklad transponowania macierzy
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

;przyklad liczenia sigmoid function	
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
	QWORDTEST dq 0
	
