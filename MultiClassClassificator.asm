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
sigmoidFunction:
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
	push dword [ebp + 28]
	push eax
	push ebx
	push ecx
	push edx
;;;;;;;;;;;;;;;;;;;;

	mov esi, [ebp + 8]
	mov edi, [ebp + 12]

	mov ecx, [ebp + 24]
	mov ebx, ecx
	and ebx, 15;reszta z dzielenia przez 32 
	shr ecx, 4 ; ile mozemy wykonac petli pobierajac po 32 liczby

	push ebx
	push ecx
	
	lea esp, [esp-20] ; alokacja pamieci na 4 liczby float i wynik ich dodawania
	mov [esp+16], dword 0 ;wyzerowanie sumatora
	
	;sprwadzenie ile jest kolumn i skoczenie do odpowiedniego mnozenia
	mov eax, ecx 
	jz pierwszeMnozenie2

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

	pierwszeMnozenie2:	
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
		mov eax, [ebp + 16] ; adres do kolejnego elementu tablicy Y
		mov edx, [esp+16] ; wynik do zapisu
		mov [eax], edx ; zapisanie liczby do nowej tablicy
		mov [esp + 16], dword 0 ; wyzerowanie sumatora
		
		add eax, 4 ; 
		mov [ebp + 16], eax ; zapisujemy wskaznik na kolejny element tablicy
		
		mov ecx, [ebp - 32]
		mov ebx, [ebp - 28]

		mov eax, [ebp - 8] ; do eax pozostala liczba wierszy macierzy Y
		dec eax
		mov [ebp-8], eax
		je next_rowX ; jesli macierz Y mnozylo teraz ostatni wiersz to przejdz do nastepnego wiersza macierzy X

		mov esi, [ebp + 8]
	
		jecxz pierwszeMnozenie2
		jmp petlaMnozenia1

	next_rowX:
		mov eax, [ebp-4] ; w eax ile jeszcze wierszy macierzy X pozostalo
		dec eax
		mov [ebp-4], eax
		je koniecMnozenia

		;resetujemy licznik wierszy macierzy Y
		mov eax, [ebp + 28]
		mov [ebp - 8], eax

		mov edi, [ebp + 12] ; macierz Y zaczynamy od poczatku
		mov eax, [ebp + 24] ; liczba kolumn macierzy X
		mov edx, dword 4		
		mul edx
		mov edx, [ebp + 8] ; w edx znajduje sie wskaznik na kolejny wiersz
		add eax, edx
		mov esi, eax ; wskaznik na pierwszy element kolejnego wiersza wedruje do esi
		mov [ebp+8], esi ;zapisujemy wskaznik do pamieci
			


		
		mov ecx, [ebp - 32] 
		mov ebx, [ebp - 28]
		jecxz farJumpToMnozenie2
		jmp petlaMnozenia1

	farJumpToMnozenie2:
		jmp pierwszeMnozenie2
		

	koniecMnozenia:
;;;;;;;;;;;;;;;;;;;;
		lea esp, [esp + 28]
		pop edx
		pop ecx
		pop ebx
		pop eax
		lea esp, [esp + 4] ; dealokacja pamieci dla zmiennych lokalnych
		mov esp, ebp
		pop ebp
	ret


;pierwszy par: wskaznik na pierwszy element tablicy [ebp + 12]
;drugi par: dlugosc tablicy w elementach [ebp+8]
wyzerujWektor:
	push ebp
	mov ebp, esp
	push eax
	push ecx
	push edx

	mov edi, [ebp+8]
	mov ecx, [ebp+12]
	petlaZerowania:
		mov [edi], dword 0
		add edi, 4
		loop petlaZerowania
	
		
	pop edx
	pop eax
	pop ecx
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

	;rezerwacja pamieci na transpozycje macierzy X
	mov eax, [esp+12]
	mov edx, [esp+16]
	mul edx
	zarezerwujPamiec eax
	mov eax, esp

	wez_GOT
	mov [ebx + X_t wrt ..gotoff], esp ; zapisanie do xmiennej X_t 

	;transponowanie macierzy X
	push dword[ebp + 16]
	push dword[ebp + 12]
	push dword[ebp + 8]
	push eax
	call transposeMatrix


	wez_GOT	
	mov ecx, [ebp + 24]
	lea edx, [ebx + EULER_NUMBER wrt ..gotoff]

	;allocating memory for initialThetaVector 1x401
	;mov eax, dword[ebp + 16]	
	;zarezerwujPamiec eax
	;mov ebx, esp

	push edx ; w [ebx - 4] znajduje sie adres do liczby e

	petla_liczba_klas: ;lecimy od 10->0 
		push ecx ; licznik petli w [ebx - 8]

		mov eax, [ebp + 12]
		zarezerwujPamiec eax ;rezerwacja pamieci 1x5000
		mov edx, esp ; w edx wskaznik na ten wektor

		mov ecx, dword 50 ; liczba iteracji ustawiona na sztywno
			petla_iteracje:
				push ecx

				;najpierw mnozymy X*initialTheta.t()
				push dword 1
				push dword [ebp+16]
				push dword [ebp+12]
				push edx
				push dword [ebp + 28]
				push dword [ebp+8]
				call multiplyMatrices ;wynik jest w edx
				lea esp, [esp + 24]
	
				;liczenie sigmoid function
				push edx
				push edx ; 
				push dword [ebx - 4] ;liczba eulera
				call sigmoidFunction ; wynik jest w edx
				lea esp, [esp+12] ; kasujemy stos

				;zczytujemy tablice y, jesli element tablicy y == ecx to odejmij 1 od tablicy ktora mamy w edx
				mov ecx, [ebp+12]
				mov esi, [ebp+20] ; wskaznik na y
				mov edi, edx
				petlaOdejmujaca:
					mov eax, [esi]
					cmp eax, [ebx-8]
					jne nextElement
					fld1
					fsubr dword[edi]
					fst dword[edi]
					nextElement:
						add edi, 4
						add esi, 4
				loop petlaOdejmujaca

				;rezerwacja pamieci na mnozenie (sigmoid(somedata) - y).t() * X
				mov eax, [ebp+12]
				zarezerwujPamiec eax
				mov eax, esp

				;pobranie adresu macierzy X_t
				wez_GOT				

				;mnozenie: 
				push dword[ebp + 16] ; liczba wierszy drugiej macierzy
				push dword[ebp + 12] ; liczba kolumn pierwszej macierzy
				push dword 1 ; liczba wierszy pierwszej macierzy
				push eax
				push dword[ebx + X_t wrt ..gotoff]
				push edx
				call multiplyMatrices
				lea esp, [esp + 24]

				;podzielenie przez m i odjecie od initialTheta
				mov esi, eax
				mov edi, [ebp+28]
				mov ecx, [ebp + 16]
				dzielenie:
					fld dword[esi]
					fdiv dword[ebp + 12]
					fsubr dword[esi]
					fstp dword[esi]
					add esi, 4
					add edi, 4
				loop dzielenie

				pop ecx
			loop farJumpToPetlaIteracje
		pop ecx
		;kolejny wektor wytrenowany, przejdz do nastepnego:
		mov eax, [ebp + 16]
		mov edx, dword 4
		mul edx
		add eax,[ebp+28]
		mov [ebp+28], eax
	loop farJumpToPetlaLiczbaKlas

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

farJumpToPetlaIteracje:
	jmp petla_iteracje

farJumpToPetlaLiczbaKlas:
	jmp petla_liczba_klas

section .data
	EULER_NUMBER dd 0x402DF854
	X_t dw 0
	
