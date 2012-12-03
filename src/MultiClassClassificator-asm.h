
/**
	MultiClassClassificator implemented in ASM
 */
class MultiClassClassificatorASM
{
public:
/**

mnozene Z = X * Y gdzie X jest macierza (n x m), a Y jest macierza (l x m)
@param pierwszy par - wskaznik do tablicy X [ebp + 8] 
@param drugi par - wskaznik do tablicy Y [ebp + 12]
@param trzeci par - wskaznik do tablicy Z [ebp + 16]
@param czwarty par - n - liczba wierszy X [ebp + 20]
@param piaty par - m liczba kolumn X [ebp + 24 ]
@param szosty par - l liczba wierszy Y [ebp + 28]

 */
void multiplyMatrices (pierwszy, drugi, trzeci, czwarty, piaty, szosty)
{
}
/**

makro rezerwujace pamiec w 
@param pierwszym parametrze przekazujemy ile elementow tablicy dword chcemy
@param drugi parametr to miejsce gdzie zapisac adres do tablicy

 */
void powerFunction (pierwszym, drugi)
{
}
/**

makro rezerwujace pamiec w 
@param pierwszym parametrze przekazujemy ile elementow tablicy dword chcemy
@param drugi parametr to miejsce gdzie zapisac adres do tablicy

 */
void powerFunction (pierwszym, drugi)
{
}
/**

@param [ebp+8] X.row
@param [ebp+12] allTheta
@param [ebp+16] predictVector
@param [ebp+20] allTheta_rows
@param [ebp+24] allTheta_cols

 */
void predictUsingThetaMatrix ([ebp, [ebp, [ebp, [ebp, [ebp)
{
}
/**

funkcja do liczenia funkcji sigmoid 
@param pierwszy parametr to tablica z (w ecx liczba elementow) [esp + 8]
@param drugi parametr to adres do nowej macierzy [esp+12]
@param trzeci par to adres do liczby eulera [esp+16]

 */
void sigmoidFunction (pierwszy, drugi, trzeci)
{
}
/**

parametry opisane w tej funkcji należą do struktury, której wskaźnik jest pierwszym parametrem
@param pierwszy parametr to wskaznik na tablice X [ebp]
@param drugi parametr to liczba wierszy w tablicy X [ebp + 4]
@param trzeci parametr to liczba kolumn w tablicy X [ebp + 8]
@param czwarty parametr to wskaznik na tablice Y [ebp + 12]
@param piaty parametr to parametr from [ebp+ 16]
@param szosty par. to parametr to [ebp + 20]
@param siodmy par. to wskaznik do allTheta (wyjsciowa tablica) [ebp + 24]

 */
void trainThetaMatrix (pierwszy, drugi, trzeci, czwarty, piaty, szosty, siodmy)
{
}
/**

funkcja transponujaca macierz
@param ebp+8 adres do pierwszego elementu tablicy gdzie zapisac nowa macierz
@param ebp+12 adres do pierwszego element tablicy do transponowania
@param ebp+16 liczba wierszy
@param ebp+20 liczba kolumn

 */
void transposeMatrix (ebp, ebp, ebp, ebp)
{
}

                                                       };
